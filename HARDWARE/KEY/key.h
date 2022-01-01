#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//读取按键2
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define KEY5  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 

#define KEY1_PRES	1		//KEY0  
#define KEY2_PRES	2		//KEY1
#define KEY3_PRES	3		//KEY2
#define KEY4_PRES	4		//KEY4
#define KEY5_PRES	5		//KEY1
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
