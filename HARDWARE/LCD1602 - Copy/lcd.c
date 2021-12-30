#include "lcd.h"



void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LCD_RS_CLK|LCD_DB0_CLK, ENABLE);  //使能引脚时钟
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=LCD_RS_PIN|LCD_RW_PIN|LCD_E_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LCD_RS_PORT,&GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Pin=LCD_DB0_PIN|LCD_DB1_PIN|LCD_DB2_PIN|LCD_DB3_PIN|LCD_DB4_PIN|LCD_DB5_PIN|LCD_DB6_PIN|LCD_DB7_PIN;	
	GPIO_Init(LCD_DB0_PORT,&GPIO_InitStruct);
}

void delay(u32 i)
{
	i=i+10;
	while(i--);
}


void Busy_Wait(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=LCD_DB0_PIN|LCD_DB1_PIN|LCD_DB2_PIN|LCD_DB3_PIN|LCD_DB4_PIN|LCD_DB5_PIN|LCD_DB6_PIN|LCD_DB7_PIN;	
	GPIO_Init(LCD_DB0_PORT,&GPIO_InitStruct);
	
	RS_L;
	RW_H;
	E_L;
	delay(5);
	E_H;
	while(GPIO_ReadInputDataBit(LCD_DB0_PORT,LCD_DB7_PIN)==1);
	E_L;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=LCD_DB0_PIN|LCD_DB1_PIN|LCD_DB2_PIN|LCD_DB3_PIN|LCD_DB4_PIN|LCD_DB5_PIN|LCD_DB6_PIN|LCD_DB7_PIN;	
	GPIO_Init(LCD_DB0_PORT,&GPIO_InitStruct);
}

void LCD_WRITE_CMD(u8 cmd)
{
	Busy_Wait();
	E_L;
	RS_L;
	RW_L;
	E_H;
	GPIO_Write(LCD_DB0_PORT, cmd);
	E_L;
	delay(5);
	E_H;
	RW_H;
}

void LCD_WRITE_DAT(u8 dat)
{
	Busy_Wait();
	E_L;
	RS_H;
	RW_L;
	E_H;
	GPIO_Write(LCD_DB0_PORT, dat);
	E_L;
	delay(5);
	E_H;
	RW_H;
}




void LCD_INIT(void)
{
	LCD_GPIO_Config();
	E_L;
	RS_L;
	RW_L;
	delay(5);
	LCD_WRITE_CMD(0x38);
	LCD_WRITE_CMD(0x0c);
	LCD_WRITE_CMD(0x06);
	LCD_WRITE_CMD(0x01);
}











