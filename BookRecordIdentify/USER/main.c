#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "spi.h"
#include "usart.h"
#include "timer.h"
#include "gpio.h"
#include "exti.h"
#include "string.h"
#include "wifi.h"
#include "rc522.h"

#define Gas 1

#define          BEEP_0()            GPIO_ResetBits( GPIOB, GPIO_Pin_9 )
#define          BEEP_1()            GPIO_SetBits ( GPIOB, GPIO_Pin_9 )

void esp8266_init();		//wifi��ʼ��
//void copy_str(char* des,char* src,unsigned char len);
void SuccessfullyId(); 	//���ʶ��ɹ�
void senddata();
void idread();
void Identification();

unsigned char Read_Gas_CMD[8] = {0x01,0x03,0x00,0x06,0x00,0x01,0x64,0x0B};			//��ȼ���崫��������--ģ��������
unsigned char Get_data_len(char* addr);
unsigned char x = 0;
char AT_send_buf[20];
char Data_buf[50];
char Gas_value_str[7];
unsigned int Gas_value;
int flag = 0;
unsigned char Flag_LED3_ONOFF = 0;
int flag_identity = 0;			//���ʶ���־λ
char IDCard[30];
char BorrowId[3];
char ReturnId[3];
int i = 0;
int num = 0;
int flag_idcard = 0; 		//¼�������Ϣ
int flag_bookrecord = 0;//¼��ͼ����Ϣ
int flag_idcard_ok = 0;	//�ж���Ϣ���Ƿ����
int flag_bookborrow = 0;
int flag_bookreturn = 0;
int flag_book = 0;

int main() 
{
	CT[0]=0x44;
	CT[1]=0x22;
	
	GPIO_Configuration();//GPIO��ʼ��
	delay_init();	    	 			//��ʱ������ʼ��
	Lcd_Init();					//��ʼ��LCD
	EXTIX_Init();					//�����ж��¼�
	Usart1_Init(115200);			//����1��ʼ��
	Usart2_Init(115200);			//����2��ʼ��
	Usart3_Remap_Init(9600);//����3��ӳ��ӿڳ�ʼ�� 
	NVIC_Configuration(); 		//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��	
	TIM3_Int_Init(999,7199);	//������ʱ��3������100ms
	TIM2_PWM_Init(2249,80);		//PWM��ʼ��
	RC522_Init();							//��Ƶģ���ʼ��
	BACK_COLOR=BLACK;		//����Ϊ��ɫ
	POINT_COLOR=YELLOW;	//����Ϊ��ɫ
	LCD_Show_Chinese16x16(90,10,"ͼ��¼�����ʶ��");
	//LCD_ShowString(140,10,"4.5.1");//��ʾӢ���ַ���
	POINT_COLOR=GREEN;//��ɫ	
	LCD_DrawRectangle(0, 40, 320, 240);//�����ο�	
	
	for(x = 0;x < 20;x ++)
		AT_send_buf[x] = 0;
	for(x = 0;x < 50;x ++)
		Data_buf[x] = 0;
	LED1_OFF();
	LCD_ShowString(16*2,16*11,"Card ID =");
	   
	//********************************************
	copy_str(Data_buf,"Hwdaf0702onT",12);
	LCD_ShowString(16*2+8*10,16*8,"202126");
	while(1)
	{
		Connect_wifi();
		//���ʶ��ɹ������ͼ��ݣ�����ͼ�����
		//copy_str(Data_buf,"Hwdaf0702offid100T",18);

		//��������ͼ��ݣ�ˢ��
			//�����ϴ�һ������
			//idread();
			//LCD_ShowString(16*2,16*6,"                ");
		
			//copy_str(&Data_buf[20],"read",4);
		//idread();
		if(Query(USART1_RX_BUF,"Hwcaf0702offokT",USART1_REC_LEN))
		{
			idread();
		}
			if(Flag_timer_2S&&flag_time_serv == 0)
			{
				Flag_timer_2S = 0;
				senddata();
			}			
			//�������ݴ�������֡
		else if(Query(USART1_RX_BUF,"Hwcar0702lighterrorT",USART1_REC_LEN))
		{
			//USART2TxStr("jhhhhhhhhhhhhhh");
			PCout(6) = 1;
		}
		else if(Query(USART1_RX_BUF,"Hwcar0702dengerrorT",USART1_REC_LEN))
		{
			PCout(6) = 1;
		}
		else if(Query(USART1_RX_BUF,"Hwcar0702motorerrorT",USART1_REC_LEN))
		{
			PCout(6) = 1;
		}
		else if(Query(USART1_RX_BUF,"Hwcar0702lightworkT",USART1_REC_LEN))
		{
			PCout(6) = 0;
		}
		else if(Query(USART1_RX_BUF,"Hwcar0702dengworkT",USART1_REC_LEN))
		{
			PCout(6) = 0;
		}
		else if(Query(USART1_RX_BUF,"Hwcar0702motorworkT",USART1_REC_LEN))
		{
			PCout(6) = 0;
		}
	}
	//
	
	return 0;
}

//void copy_str(char* des,char* src,unsigned char len)
//{
//	unsigned char i;
//	for(i = 0;i < len;i ++)
//	{
//		*(des+i) = *(src+i);
//	}
//}

unsigned char Get_data_len(char* addr)//��ȡ�ַ���/������Ч���ݳ���
{
	unsigned len;
	for(len = 0;*(addr+len) != '\0';len ++);
	return len;
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

void SuccessfullyId()
{
	Count_time_10S = 100;
	while(Count_time_10S)
	{
		if(Query(USART1_RX_BUF,"Hwcei0702noT",USART1_REC_LEN))
		{
			CLR_Buf1();
			PCout(6) = 0;
			break;
		}
		
		LED1_ON();
	}
	LED1_OFF();
}

void idread()
{
	if(Flag_timer_2S_read)
	{//2Sʱ�䵽
		RC522_Init();
		Flag_timer_2S_read = 0;
		if(Flag_LED3_ONOFF)
		{
			Flag_LED3_ONOFF = 0;
			LED3_ON();
			LCD_Fill(300,40+20*7,300+16,40+20*7+16,YELLOW);
		}
		else
		{
			Flag_LED3_ONOFF = 1;
			LED3_OFF();
			LCD_Fill(300,40+20*7,300+16,40+20*7+16,BLACK);
		}
	}
	if(Flag_find_card)
	{
		delay_ms(1000);
		Flag_find_card = 0;
	}						
	RC522_Read(0x01);
}

void Identification()
{
	
}


