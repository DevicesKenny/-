#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "gpio.h"
#include "wifi.h"

#define 	Human_body_infrared		1//�������

unsigned char Read_Human_body_infrared_CMD[7] = {0xCA,0x20,0xFE,0x60,0x01,0x01,0xAC};//��ȡ������⴫������״̬����
unsigned char Flag_LED3_ONOFF = 0;
char Data_buf[50];
unsigned char Flag_Sonser_Device_onoff = 0;
char AT_send_buf[20];
void copy_str(char* des,char* src,unsigned char len);
unsigned char Get_data_len(char* addr);//��ȡ�ַ���/������Ч���ݳ���
void esp8266_init();
void senddata();
void alarm();
int flag_havepeople = 0;
int flag_alarm = 0;

unsigned char x,flag = 1;

int main(void)
{
    GPIO_Configuration();//GPIO��ʼ��
    delay_init();	    	 			//��ʱ������ʼ��
    Lcd_Init();					//��ʼ��LCD
    Usart1_Init(115200);			//����1��ʼ��
    Usart2_Init(115200);			//����2��ʼ��
    Usart3_Remap_Init(9600);//����3��ӳ��ӿڳ�ʼ��
    NVIC_Configuration(); 		//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
    TIM3_Int_Init(999,7199);	//������ʱ��3������100ms
    for(x = 0; x < 20; x ++)
        AT_send_buf[x] = '\0';
    BACK_COLOR=BLACK;		//����Ϊ��ɫ
    POINT_COLOR=YELLOW;	//����Ϊ��ɫ
    LCD_Show_Chinese16x16(90,10,"�������");
    //LCD_ShowString(140,10,"4.5.2");//��ʾӢ���ַ���
    POINT_COLOR=GREEN;//��ɫ
    LCD_DrawRectangle(0, 30, 320, 240);//�����ο�

    LED1_OFF();
    LED2_OFF();
    Buzzer_OFF();

    LCD_Show_Chinese16x16(80,40+18*6,"������⴫����");
    POINT_COLOR=GREEN;//��ɫ
    LCD_Show_Chinese16x16(140,40+20*7,"����");

    while(1)
    {
        Connect_wifi();
        if(Flag_timer_2S)
        {   //2Sʱ�䵽
            Flag_timer_2S = 0;
            if(Flag_LED3_ONOFF)
            {
                Flag_LED3_ONOFF = 0;
                LED3_ON();
                LCD_Fill(20,40+20*7,20+16,40+20*7+16,YELLOW);
            }
            else
            {
                Flag_LED3_ONOFF = 1;
                LED3_OFF();
                LCD_Fill(20,40+20*7,20+16,40+20*7+16,BLACK);
            }
            //���Ͳɼ���������������
            USART3TxData_hex(Read_Human_body_infrared_CMD,7);
        }
				//��ӱչݿ��ݵı�־��ʵ�ֿ��ݱչݵ��л���Ĭ��Ϊ����
				if(Query(USART1_RX_BUF,"Hwchj0502onT",USART1_REC_LEN))
				{
					//USART2TxStr("jkshgueliehuigbilgydis");
					flag_alarm = 0;
					CLR_Buf1();
				}	
			
				else if(Query(USART1_RX_BUF,"Hwchj0503offT",USART1_REC_LEN))
				{
					//USART2TxStr("jkshgueliehuigbilgydis");
					flag_alarm = 1;
					CLR_Buf1();
				}
        ///////////////////////////////////////////////////////////////////
        if(Flag_Usart3_Receive//�������3�����ݽ���
                &&(!Count_Timer3_value_USART3_receive_timeout))//����������ճ�ʱ
        {
            Flag_Usart3_Receive = 0;
            //��ʾ���������ݵ�LCD��
					
            //CA 20 FE 18 02 01 01 AC //״̬�����仯���Զ����ص����ݣ������ڶ����ֽ�,0x00��ʾû�ˣ�0x01��ʾ���ˣ�
            //CA B0 01 01 AC 					//��ѯ״̬����Ӧ�����ݣ������ڶ����ֽ�,0x00��ʾû�ˣ�0x01��ʾ���ˣ�
            if(USART3_RX_BUF[0] == 0xCA)
            {
                if(USART3_RX_BUF[1] == 0x20)
                {
                    if(USART3_RX_BUF[6])
                    {
                        Flag_Sonser_Device_onoff = 1;
                        copy_str(Data_buf,"Hwdhi05011  T",13);
                        POINT_COLOR=YELLOW;	//����Ϊ��ɫ
                        LCD_Show_Chinese16x16(140,40+20*7,"����");
												//USART2TxChar(flag_alarm+'0');	
												flag_havepeople = 1;												
                    }
                    else
                    {
                        Flag_Sonser_Device_onoff = 0;
                        copy_str(Data_buf,"Hwdhi05010  T",13);
                        POINT_COLOR=GREEN;//��ɫ
                        LCD_Show_Chinese16x16(140,40+20*7,"����");
												//USART2TxChar(flag_alarm+'0');	
												flag_havepeople = 0;
                    }
                }
                else if(USART3_RX_BUF[1] == 0xB0)
                {
                    if(USART3_RX_BUF[3])
                    {
                        Flag_Sonser_Device_onoff = 1;
                        copy_str(Data_buf,"Hwdhi05011  T",13);
                        POINT_COLOR=YELLOW;	//����Ϊ��ɫ
                        LCD_Show_Chinese16x16(140,40+20*7,"����");
												//USART2TxChar(flag_alarm+'0');			
												flag_havepeople = 1;											
													
                    }
                    else
                    {
                        Flag_Sonser_Device_onoff = 0;
                        copy_str(Data_buf,"Hwdhi05010  T",13);
                        POINT_COLOR=GREEN;//��ɫ
                        LCD_Show_Chinese16x16(140,40+20*7,"����");
												//USART2TxChar(flag_alarm+'0');	
												flag_havepeople = 0;
                    }
                }
            }
						if(flag_alarm)
						{
							copy_str(&Data_buf[10],"of",2);
						}
						else
							copy_str(&Data_buf[10],"on",2);						
            CLR_Buf3();		
						
						//if(flag_time_serv == 0)
						//if(!Query(USART1_RX_BUF,"busy",USART1_REC_LEN))
						if(flag_time_serv == 0)
							senddata();
						if(flag_alarm&&flag_havepeople)
						{
							alarm();
						}					
				}
			
    }
}

void copy_str(char* des,char* src,unsigned char len)
{
    unsigned char i;
    for(i = 0; i < len; i ++)
    {
        *(des+i) = *(src+i);
    }
}
unsigned char Get_data_len(char* addr)//��ȡ�ַ���/������Ч���ݳ���
{
    unsigned len;
    for(len = 0; *(addr+len) != '\0'; len ++);
    return len;
}

void esp8266_init()
{
    //����WiFiģ��
    USART2TxStr("����ģ��...\r\n");
    USART1TxStr("AT+RST\r\n");
    LCD_ShowString(1,40,"Restart wifi mode...");
    delay_ms(1000);//ֵ���Ϊ1864
    delay_ms(1000);//ֵ���Ϊ1864
    delay_ms(1000);//ֵ���Ϊ1864
    delay_ms(1000);//ֵ���Ϊ1864
    delay_ms(1000);//ֵ���Ϊ1864
    delay_ms(1000);//ֵ���Ϊ1864
    CLR_Buf1();     //�������1���ջ���
    Flag_usart1_receive_OK = 0;
    //���ù���ģʽ
    USART2TxStr("���ù���ģʽ...\r\n");
    USART1TxStr("AT+CWMODE=3\r\n");
    wait_OK();//�ȴ����յ�OK�ַ���
    LCD_ShowString(1,40+18*1,"AT+CWMODE=3");
    //������֪WiFi
    USART2TxStr("������֪WiFi...\r\n");
    USART1TxStr("AT+CWJAP=\"WIFI_100\",\"12345678\"\r\n");
    wait_OK();//�ȴ����յ�OK�ַ���
    LCD_ShowString(1,40+18*2,"AT+CWJAP=\"WIFI_100\",\"12345678\"");
    //������ģ������
    USART2TxStr("������ģ������...\r\n");
    USART1TxStr("AT+CIPMUX=1\r\n");
    wait_OK();//�ȴ����յ�OK�ַ���
    LCD_ShowString(1,40+18*3,"AT+CIPMUX=1");
    //���ӷ�����
    USART2TxStr("���ӷ�����...\r\n");
    USART1TxStr("AT+CIPSTART=4,\"TCP\",\"192.168.1.200\",6000\r\n");
    wait_OK();//�ȴ����յ�OK�ַ���
    LCD_ShowString(1,40+18*4,"AT+CIPSTART=4,\"TCP\",\"192.168.1.200\",");
    LCD_ShowString(1,40+18*5,"6000");
}

void senddata()
{
    //ͨ��WIFI�������ݸ�������
    copy_str(AT_send_buf,"AT+CIPSEND=4,0",14);//�����ַ���
    x = Get_data_len(Data_buf);//��ȡ�ַ���/������Ч���ݳ���
    if(x < 10)
    {
        AT_send_buf[13] = x % 10 + '0';
        AT_send_buf[14] = '\r';
        AT_send_buf[15] = '\n';
    }
    else
    {
        AT_send_buf[13] = x % 100 / 10 + '0';
        AT_send_buf[14] = x % 10  + '0';
        AT_send_buf[15] = '\r';
        AT_send_buf[16] = '\n';
    }
    USART2TxStr(AT_send_buf);
    USART1TxStr(AT_send_buf);//�����ַ�������
    Count_Timer3_value_USART3_receive_timeout2 = 30;
    POINT_COLOR=YELLOW;	//����Ϊ��ɫ
    LCD_ShowString(120+16*6,40+18*6,"send len... ");
    while(flag)
    {
        for(x = 0; x < USART1_RX_STA; x ++)
        {
            if(USART1_RX_BUF[x] == '>')
            {   //�ȴ��յ����ں��ַ���
                flag = 0;
                break;
            }
        }
        if(Count_Timer3_value_USART3_receive_timeout2 == 0)
        {
            flag = 0;
            flag_time_serv = 1;
            break;
        }
    }
    if(flag_time_serv == 0)
    {
        flag = 1;
        USART2TxStr(Data_buf);
        USART1TxStr(Data_buf);//�����ַ���
        LCD_ShowString(120+16*6,40+18*6,"send data...");
        wait_OK();//�ȴ����յ�OK�ַ���
        LCD_ShowString(120+16*6,40+18*6,"Done !      ");
        CLR_Buf1();     //�������1���ջ���
        CLR_Buf2();     //�������2���ջ���
        for(x = 0; x < 20; x ++)
            AT_send_buf[x] = 0;
        //copy_str(Data_buf,"HwdFF0T",7);
    }
}

void alarm()
{
	Count_timer_8S = 80;
	Buzzer_ON();
	while(Count_timer_8S)
	{
		if(Query(USART1_RX_BUF,"Hwchj0502onT",USART1_REC_LEN))
		{
			flag_alarm = !flag_alarm;
			CLR_Buf1();
			break;
		}	
		LED1_ON();
		delay_ms(50);
		LED1_OFF();
		delay_ms(50);
	}
	Buzzer_OFF();
}

