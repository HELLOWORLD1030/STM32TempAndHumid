#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


#define LCD_RS_PORT  GPIOB
#define LCD_RS_CLK   RCC_APB2Periph_GPIOB
#define LCD_RS_PIN   GPIO_Pin_12  

#define LCD_RW_PORT  GPIOB
#define LCD_RW_CLK   RCC_APB2Periph_GPIOB
#define LCD_RW_PIN   GPIO_Pin_13 

#define LCD_E_PORT  GPIOB
#define LCD_E_CLK   RCC_APB2Periph_GPIOB
#define LCD_E_PIN   GPIO_Pin_14  




#define LCD_DB0_PORT  GPIOA
#define LCD_DB0_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB0_PIN   GPIO_Pin_0 

#define LCD_DB1_PORT  GPIOA
#define LCD_DB1_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB1_PIN   GPIO_Pin_1 

#define LCD_DB2_PORT  GPIOA
#define LCD_DB2_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB2_PIN   GPIO_Pin_2 

#define LCD_DB3_PORT  GPIOA
#define LCD_DB3_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB3_PIN   GPIO_Pin_3 

#define LCD_DB4_PORT  GPIOA
#define LCD_DB4_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB4_PIN   GPIO_Pin_4 

#define LCD_DB5_PORT  GPIOA
#define LCD_DB5_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB5_PIN   GPIO_Pin_5 

#define LCD_DB6_PORT  GPIOA
#define LCD_DB6_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB6_PIN   GPIO_Pin_6 

#define LCD_DB7_PORT  GPIOA
#define LCD_DB7_CLK   RCC_APB2Periph_GPIOA
#define LCD_DB7_PIN   GPIO_Pin_7 


#define RS_H  GPIO_SetBits(LCD_RS_PORT,LCD_RS_PIN)
#define RS_L  GPIO_ResetBits(LCD_RS_PORT,LCD_RS_PIN)

#define RW_H  GPIO_SetBits(LCD_RW_PORT,LCD_RW_PIN)
#define RW_L  GPIO_ResetBits(LCD_RW_PORT,LCD_RW_PIN)

#define E_H  GPIO_SetBits(LCD_E_PORT,LCD_E_PIN)
#define E_L  GPIO_ResetBits(LCD_E_PORT,LCD_E_PIN)


void LCD_INIT(void);
void LCD_WRITE_DAT(u8 dat);
void LCD_WRITE_CMD(u8 cmd);


#endif
