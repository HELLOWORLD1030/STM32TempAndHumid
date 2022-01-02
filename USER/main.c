#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
#include "exti.h"
#include "key.h"
#include "DS18B20.h"
#include "DHT11.h"
#include "LCD1602.h"
#include<stdio.h>

//START ����
//�����������ȼ�
#define START_TASK_PRIO			10  ///��ʼ��������ȼ�Ϊ���
//���������ջ��С
#define START_STK_SIZE			128
//���������ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);

//�����������ȼ�
#define DS18B20_TASK_PRIO			7
//���������ջ��С
#define DS18B20_STK_SIZE			64
//�����ջ
OS_STK DS18B20_TASK_STK[DS18B20_STK_SIZE];
//������
void DS18B20_task(void *pdata);

//�����������ȼ�
#define DHT11_TASK_PRIO			6
//���������ջ��С
#define DHT11_STK_SIZE			64
//�����ջ
__align(8) OS_STK DHT11_TASK_STK[DHT11_STK_SIZE];
//������
void DHT11_task(void *pdata);

//�����������
#define USARTSEND_TASK_PRIO			5
//���������ջ��С
#define USARTSEND_STK_SIZE			128
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK USARTSEND_TASK_STK[USARTSEND_STK_SIZE]; 
//������
void USARTSEND_task(void *pdata);

#define DISPLAY_TASK_PRIO			5
//���������ջ��С
#define DISPLAY_STK_SIZE			128
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE]; 
//������
void DISPLAY_task(void *pdata);

#define USART_TASK_PRIO			4
//���������ջ��С
#define USART_STK_SIZE			128
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK USART_TASK_STK[USART_STK_SIZE]; 
//������
void usart_task(void *pdata);
#define DISPLAY_SCOPE_TASK_PRIO			8
//���������ջ��С
#define DISPLAY_SCOPE_STK_SIZE			128
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK DISPLAY_SCOPE_STK[DISPLAY_SCOPE_STK_SIZE]; 
//������
void DISPLAY_SCOPE_task(void *pdata);
int MaxTemp=120;
int MinTemp=10;
u8 buffer[5];
u8 u1buffer[4]={0};   
u8 u2buffer[4]={0};   
OS_EVENT *data1;
typedef struct {
	double hum;
	double temp;
} TempCmd;
TempCmd *temp;
static void user_init(void);
static void LCD_Disp(double u1,double u2);
int MaxHum=90;
int MinHum=10;
int main(void)
{
	
	delay_init();       //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	uart_init(115200);    //���ڲ���������
	LED_Init();  	//LED��ʼ��
	EXTIX_Init();
	KEY_Init();
	LED3=1;
	gpio_config();
	lcd_init();
	OSInit();  		//UCOS��ʼ��
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //������ʼ����
	OSStart(); 	//��ʼ����
}

//��ʼ����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //����ͳ������
	data1=OSMboxCreate(0);
	
	OS_ENTER_CRITICAL();  //�����ٽ���(�ر��ж�)
	OSTaskCreate(DHT11_task,(void*)0,(OS_STK*)&DHT11_TASK_STK[DHT11_STK_SIZE-1],DHT11_TASK_PRIO);//����DHT11
	//OSTaskCreate(DS18B20_task,(void*)0,(OS_STK*)&DS18B20_TASK_STK[DS18B20_STK_SIZE-1],DS18B20_TASK_PRIO);//����DHT11
  //OSTaskCreate(USARTSEND_task,(void*)0,(OS_STK*)&USARTSEND_TASK_STK[USARTSEND_STK_SIZE-1],USARTSEND_TASK_PRIO);//
	OSTaskCreate(DISPLAY_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO);//�
	OSTaskCreate(usart_task,(void*)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);//
	OSTaskCreate(DISPLAY_SCOPE_task,(void*)0,(OS_STK*)&DISPLAY_SCOPE_STK[DISPLAY_SCOPE_STK_SIZE-1],DISPLAY_SCOPE_TASK_PRIO);//
	OSTaskSuspend(START_TASK_PRIO);//����ʼ����
	OS_EXIT_CRITICAL();  //�˳��ٽ���(���ж�)
}
 

void DS18B20_task(void *pdata)
{
	int DS18B20_Temp;
	while(1)
	{
		
		DS18B20_Temp=Ds18b20ReadTemp();//��ȡ�¶�
		LED3=!LED3;
		
		
	}
}

void DHT11_task(void *pdata)
{
	u8 ll=1;
	TempCmd tempCmd;
	while(1){
		//Temp=Ds18b20ReadTemp();//��ȡ�¶�
        if (dht11_read_data(buffer) == 0)
        {
            tempCmd.hum = buffer[0] + buffer[1] / 10.0;
            tempCmd.temp = buffer[2] + buffer[3] / 10.0;
        }
		if(tempCmd.temp>MaxTemp||tempCmd.temp<MinTemp){
			LED4=!LED4;
		}
		tempCmd.hum=rand() % 51 + 13;
		OSMboxPost(data1,&tempCmd);
		OSTimeDly(300);
		
	}
}
void USARTSEND_task(void *pdata)
{
	
	u8 error;
	OS_CPU_SR cpu_sr=0;
	static float float_num=0.01;
	while(1)
	{
		temp=OSMboxPend(data1,0,&error);
		OS_ENTER_CRITICAL();	//�����ٽ���(�ر��ж�)
		printf("temperature:%.2f,humidness:%.2f \n", (*temp).temp, (*temp).hum);
		OS_EXIT_CRITICAL();		//�˳��ٽ���(���ж�)
		OSTimeDly(100);
	}
		
		
}
void usart_task(void *pdata){
	OS_CPU_SR cpu_sr=0;
	int len;
	int t;
	char* Test="open";
	char* close="close";
	char rec[50];
	while(1){
		if(USART_RX_STA&0x8000){
				len=USART_RX_STA&0x3fff;
				for(t=0;t<len;t++){
					rec[t]=(char)USART_RX_BUF[t];
				}
				rec[t]='\0';
			OS_ENTER_CRITICAL();	//�����ٽ���(�ر��ж�)
			if(strcmp(rec,Test)==0){
				
			};
			if(strcmp(rec,close)==0){
				
		 	}
			OS_EXIT_CRITICAL();		//�˳��ٽ���(���ж�)
			USART_RX_STA=0;
		}
		OSTimeDly(200);
	
	}


}
void DISPLAY_task(void *pdata){
	
	
	u8 error;
	
	OS_CPU_SR cpu_sr=0;
	static float float_num=0.01;
	user_init();
	while(1)
	{
		temp=OSMboxPend(data1,0,&error);
		//temp=OSMboxAccept(data1);
		LCD_Disp((*temp).temp,(*temp).hum);//??u1?u2
		OSTimeDly(100);
	}

}
static void user_init(){
	lcd_write_string(0,0,(u8*)"T=");
	lcd_write_string(4,0,(u8*)".");
	lcd_write_string(6,0,(u8*)"C");
	lcd_write_string(0,1,(u8*)"H=");
	
	lcd_write_string(5,1,(u8*)"%");
	
}
static void LCD_Disp(double u1,double u2){
	int n2=(int)u2;
	int count2=0;
	int n=(int)u1;
	double little=u1-(double)n;
	int count=0;
	while(n!=0){
		u1buffer[count]= n%10;
		n/=10;
		count++;
	}
	
	while(n2!=0){
		u2buffer[count2]= n2%10;
		n2/=10;
		count2++;
	}

	lcd_write_char(2,0,u1buffer[count-1]+0X30);
	lcd_write_char(3,0,u1buffer[count-2]+0X30);
	lcd_write_char(5,0,(little*10)+0X30);
	//lcd_write_char(5,1,u2buffer[1]+0X30);
	lcd_write_char(2,1,u2buffer[count2-1]+0X30);
	lcd_write_char(3,1,u2buffer[count2-2]+0X30);
	
}
void DISPLAY_SCOPE_task(void* pdata){
	while(1){
	lcd_write_char(8,0,MinTemp/10+0X30);
	lcd_write_char(9,0,MinTemp%10+0x30);
	lcd_write_char(11,0,MaxTemp/100+0X30);
	lcd_write_char(12,0,MaxTemp/10%10+0x30);
	lcd_write_char(13,0,MaxTemp%10+0x30);
	lcd_write_char(8,1,MinHum/10+0X30);
	lcd_write_char(9,1,MinHum%10+0x30);
	lcd_write_char(11,1,MaxHum/10+0x30);
	lcd_write_char(12,1,MaxHum%10+0x30);
	OSTimeDly(100);
	
	}

}




