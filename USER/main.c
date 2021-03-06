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

//START 任务
//设置任务优先级
#define START_TASK_PRIO			10  ///开始任务的优先级为最低
//设置任务堆栈大小
#define START_STK_SIZE			128
//任务任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//设置任务优先级
#define DS18B20_TASK_PRIO			7
//设置任务堆栈大小
#define DS18B20_STK_SIZE			64
//任务堆栈
OS_STK DS18B20_TASK_STK[DS18B20_STK_SIZE];
//任务函数
void DS18B20_task(void *pdata);

//设置任务优先级
#define DHT11_TASK_PRIO			6
//设置任务堆栈大小
#define DHT11_STK_SIZE			64
//任务堆栈
__align(8) OS_STK DHT11_TASK_STK[DHT11_STK_SIZE];
//任务函数
void DHT11_task(void *pdata);

//浮点测试任务
#define USARTSEND_TASK_PRIO			5
//设置任务堆栈大小
#define USARTSEND_STK_SIZE			128
//任务堆栈
//如果任务中使用printf来打印浮点数据的话一点要8字节对齐
__align(8) OS_STK USARTSEND_TASK_STK[USARTSEND_STK_SIZE]; 
//任务函数
void USARTSEND_task(void *pdata);

#define DISPLAY_TASK_PRIO			5
//设置任务堆栈大小
#define DISPLAY_STK_SIZE			128
//任务堆栈
//如果任务中使用printf来打印浮点数据的话一点要8字节对齐
__align(8) OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE]; 
//任务函数
void DISPLAY_task(void *pdata);

#define USART_TASK_PRIO			4
//设置任务堆栈大小
#define USART_STK_SIZE			128
//任务堆栈
//如果任务中使用printf来打印浮点数据的话一点要8字节对齐
__align(8) OS_STK USART_TASK_STK[USART_STK_SIZE]; 
//任务函数
void usart_task(void *pdata);
#define DISPLAY_SCOPE_TASK_PRIO			8
//设置任务堆栈大小
#define DISPLAY_SCOPE_STK_SIZE			128
//任务堆栈
//如果任务中使用printf来打印浮点数据的话一点要8字节对齐
__align(8) OS_STK DISPLAY_SCOPE_STK[DISPLAY_SCOPE_STK_SIZE]; 
//任务函数
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
	
	delay_init();       //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	uart_init(115200);    //串口波特率设置
	LED_Init();  	//LED初始化
	EXTIX_Init();
	KEY_Init();
	LED3=1;
	gpio_config();
	lcd_init();
	OSInit();  		//UCOS初始化
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //创建开始任务
	OSStart(); 	//开始任务
}

//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //开启统计任务
	data1=OSMboxCreate(0);
	
	OS_ENTER_CRITICAL();  //进入临界区(关闭中断)
	OSTaskCreate(DHT11_task,(void*)0,(OS_STK*)&DHT11_TASK_STK[DHT11_STK_SIZE-1],DHT11_TASK_PRIO);//创建DHT11
	//OSTaskCreate(DS18B20_task,(void*)0,(OS_STK*)&DS18B20_TASK_STK[DS18B20_STK_SIZE-1],DS18B20_TASK_PRIO);//创建DHT11
  //OSTaskCreate(USARTSEND_task,(void*)0,(OS_STK*)&USARTSEND_TASK_STK[USARTSEND_STK_SIZE-1],USARTSEND_TASK_PRIO);//
	OSTaskCreate(DISPLAY_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[DISPLAY_STK_SIZE-1],DISPLAY_TASK_PRIO);//?
	OSTaskCreate(usart_task,(void*)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);//
	OSTaskCreate(DISPLAY_SCOPE_task,(void*)0,(OS_STK*)&DISPLAY_SCOPE_STK[DISPLAY_SCOPE_STK_SIZE-1],DISPLAY_SCOPE_TASK_PRIO);//
	OSTaskSuspend(START_TASK_PRIO);//挂起开始任务
	OS_EXIT_CRITICAL();  //退出临界区(开中断)
}
 

void DS18B20_task(void *pdata)
{
	int DS18B20_Temp;
	while(1)
	{
		
		DS18B20_Temp=Ds18b20ReadTemp();//获取温度
		LED3=!LED3;
		
		
	}
}

void DHT11_task(void *pdata)
{
	u8 ll=1;
	TempCmd tempCmd;
	while(1){
		//Temp=Ds18b20ReadTemp();//获取温度
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
		OS_ENTER_CRITICAL();	//进入临界区(关闭中断)
		printf("temperature:%.2f,humidness:%.2f \n", (*temp).temp, (*temp).hum);
		OS_EXIT_CRITICAL();		//退出临界区(开中断)
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
			OS_ENTER_CRITICAL();	//进入临界区(关闭中断)
			if(strcmp(rec,Test)==0){
				
			};
			if(strcmp(rec,close)==0){
				
		 	}
			OS_EXIT_CRITICAL();		//退出临界区(开中断)
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




