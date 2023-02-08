#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"

void GPIO_Configuration(void)//GPIO��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*��ʼ���ṹ��*/   
	GPIO_StructInit(&GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PC�˿�ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ������� 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //��ʼ��PC.15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ������� 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //��ʼ��PC.6

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ������� 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //��ʼ��PC.13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ������� 
	GPIO_Init(GPIOC, &GPIO_InitStructure);      //��ʼ��PC.13
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ������� 
	GPIO_Init(GPIOB, &GPIO_InitStructure);      //��ʼ��PB.9
	
	GPIO_SetBits ( GPIOB, GPIO_Pin_9 );

    // RST
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);

			// CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��

    // SCK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}
