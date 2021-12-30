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
//START ÈÎÎñ
//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define START_TASK_PRIO			10  ///¿ªÊ¼ÈÎÎñµÄÓÅÏÈ¼¶Îª×îµÍ
//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define START_STK_SIZE			128
//ÈÎÎñÈÎÎñ¶ÑÕ»
OS_STK START_TASK_STK[START_STK_SIZE];
//ÈÎÎñº¯Êý
void start_task(void *pdata);

//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define DS18B20_TASK_PRIO			7
//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define DS18B20_STK_SIZE			64
//ÈÎÎñ¶ÑÕ»
OS_STK DS18B20_TASK_STK[DS18B20_STK_SIZE];
//ÈÎÎñº¯Êý
void DS18B20_task(void *pdata);

//LED1ÈÎÎñ
//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define DHT11_TASK_PRIO			6
//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define DHT11_STK_SIZE			64
//ÈÎÎñ¶ÑÕ»
__align(8) OS_STK DHT11_TASK_STK[DHT11_STK_SIZE];
//ÈÎÎñº¯Êý
void DHT11_task(void *pdata);

//¸¡µã²âÊÔÈÎÎñ
#define USARTSEND_TASK_PRIO			5
//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define USARTSEND_STK_SIZE			128
//ÈÎÎñ¶ÑÕ»
//Èç¹ûÈÎÎñÖÐÊ¹ÓÃprintfÀ´´òÓ¡¸¡µãÊý¾ÝµÄ»°Ò»µãÒª8×Ö½Ú¶ÔÆë
__align(8) OS_STK USARTSEND_TASK_STK[USARTSEND_STK_SIZE]; 
//ÈÎÎñº¯Êý
void USARTSEND_task(void *pdata);

#define DISPLAY_TASK_PRIO			5
//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define DISPLAY_STK_SIZE			128
//ÈÎÎñ¶ÑÕ»
//Èç¹ûÈÎÎñÖÐÊ¹ÓÃprintfÀ´´òÓ¡¸¡µãÊý¾ÝµÄ»°Ò»µãÒª8×Ö½Ú¶ÔÆë
__align(8) OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE]; 
//ÈÎÎñº¯Êý
void DISPLAY_task(void *pdata);

#define USART_TASK_PRIO			4
//ÉèÖÃÈÎÎñ¶ÑÕ»´óÐ¡
#define USART_STK_SIZE			128
//ÈÎÎñ¶ÑÕ»
//Èç¹ûÈÎÎñÖÐÊ¹ÓÃprintfÀ´´òÓ¡¸¡µãÊý¾ÝµÄ»°Ò»µãÒª8×Ö½Ú¶ÔÆë
__align(8) OS_STK USART_TASK_STK[USART_STK_SIZE]; 
//ÈÎÎñº¯Êý
void usart_task(void *pdata);
int flag=1;
u8 buffer[5];
u8 u1buffer[4]={0};   
u8 u2buffer[4]={0};   
OS_EVENT *data1;
typedef struct {
	double hum;
	double temp;
} TempCmd;
static void user_init(void);
static void LCD_Disp(double u1,double u2);
int main(void)
{
	
	delay_init();       //ÑÓÊ±³õÊ¼»¯
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //ÖÐ¶Ï·Ö×éÅäÖÃ
	uart_init(115200);    //´®¿Ú²¨ÌØÂÊÉèÖÃ
	LED_Init();  	//LED³õÊ¼»¯
	EXTIX_Init();
	LED3=1;
	//LED0=1;
	//gpio_config();
	//lcd_init();
	gpio_config();
	lcd_init();
	//user_init();
	//Ds18b20_Init();
	//user_init();
	OSInit();  		//UCOS³õÊ¼»¯
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //´´½¨¿ªÊ¼ÈÎÎñ
	OSStart(); 	//¿ªÊ¼ÈÎÎñ
}

//¿ªÊ¼ÈÎÎñ
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //¿ªÆôÍ³¼ÆÈÎÎñ
	data1=OSMboxCreate(0);
	
	OS_ENTER_CRITICAL();  //½øÈëÁÙ½çÇø(¹Ø±ÕÖÐ¶Ï)
	OSTaskCreate(DHT11_task,(void*)0,(OS_STK*)&DHT11_TASK_STK[DHT11_STK_SIZE-1],DHT11_TASK_PRIO);//´´½¨DHT11
	//OSTaskCreate(DS18B20_task,(void*)0,(OS_STK*)&DS18B20_TASK_STK[DS18B20_STK_SIZE-1],DS18B20_TASK_PRIO);//´´½¨DHT11
//OSTaskCreate(USARTSEND_task,(void*)0,(OS_STK*)&USARTSEND_TASK_STK[USARTSEND_STK_SIZE-1],USARTSEND_TASK_PRIO);//´´½¨¸¡µã²âÊÔÈÎÎñ
	OSTaskCreate(DISPLAY_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO);//´
	OSTaskCreate(usart_task,(void*)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);//
	OSTaskSuspend(START_TASK_PRIO);//¹ÒÆð¿ªÊ¼ÈÎÎñ
	OS_EXIT_CRITICAL();  //ÍË³öÁÙ½çÇø(¿ªÖÐ¶Ï)
}
 

void DS18B20_task(void *pdata)
{
	int DS18B20_Temp;
	while(1)
	{
		DS18B20_Temp=Ds18b20ReadTemp();//»ñÈ¡ÎÂ¶È
		LED3=!LED3;
		OSTimeDly(50);
	}
}

void DHT11_task(void *pdata)
{
	u8 ll=1;
	TempCmd tempCmd;
	while(1){
		//Temp=Ds18b20ReadTemp();//»ñÈ¡ÎÂ¶È
        if (dht11_read_data(buffer) == 0)
        {
            tempCmd.hum = buffer[0] + buffer[1] / 10.0;
            tempCmd.temp = buffer[2] + buffer[3] / 10.0;
        }
		LED3=!LED3;
		//LCD_Disp(tempCmd.temp,tempCmd.hum);//??u1?u2
		if(tempCmd.temp>20){
			LED4=!LED4;
		}
		OSMboxPost(data1,&tempCmd);
		OSTimeDly(300);
		
	}
}
void USARTSEND_task(void *pdata)
{
	TempCmd *temp;
	u8 error;
	OS_CPU_SR cpu_sr=0;
	static float float_num=0.01;
	while(1)
	{
		temp=OSMboxPend(data1,0,&error);
		OS_ENTER_CRITICAL();	//½øÈëÁÙ½çÇø(¹Ø±ÕÖÐ¶Ï)
		printf("temperature:%.2f,humidness:%.2f \n", (*temp).temp, (*temp).hum);
		OS_EXIT_CRITICAL();		//ÍË³öÁÙ½çÇø(¿ªÖÐ¶Ï)
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
			OS_ENTER_CRITICAL();	//½øÈëÁÙ½çÇø(¹Ø±ÕÖÐ¶Ï)
			if(strcmp(rec,Test)==0){
				flag=1;
			};
			if(strcmp(rec,close)==0){
				flag=0;
			}
			OS_EXIT_CRITICAL();		//ÍË³öÁÙ½çÇø(¿ªÖÐ¶Ï)
			USART_RX_STA=0;
		}
		OSTimeDly(200);
	
	}


}
void DISPLAY_task(void *pdata){
	
	TempCmd *temp;
	u8 error;
	
	OS_CPU_SR cpu_sr=0;
	static float float_num=0.01;
	user_init();
	while(1)
	{
		temp=OSMboxPend(data1,0,&error);
		LCD_Disp((*temp).temp,(*temp).hum);//??u1?u2
		OSTimeDly(100);
	}

}
static void user_init(){
	lcd_write_string(0,0,(u8*)"Temp=");
	lcd_write_string(7,0,(u8*)".");
	lcd_write_string(9,0,(u8*)"C");
	lcd_write_string(0,1,(u8*)"Hum=");
	
	lcd_write_string(7,1,(u8*)"%");
	
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
	/*
	u1buffer[0] = u1intptr/1000;
	u1buffer[1] = (int)u1intptr%1000/100;
	u1buffer[2]=little;
	
	u2little=modf(u2,&u2intptr);
 	u2buffer[0] = u2intptr/1000;
	u2buffer[1] = (int)u2intptr%1000/100;
	*/
	//lcd_write_char(6,0,u1buffer[0]+0X30);
	lcd_write_char(5,0,u1buffer[count-1]+0X30);
	lcd_write_char(6,0,u1buffer[count-2]+0X30);
	lcd_write_char(8,0,(little*10)+0X30);
	//lcd_write_char(5,1,u2buffer[1]+0X30);
	lcd_write_char(4,1,u2buffer[count2-1]+0X30);
	lcd_write_char(5,1,u2buffer[count2-2]+0X30);
	
}




