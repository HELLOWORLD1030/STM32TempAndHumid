#ifndef __LCD_1602_H__
#define	__LCD_1602_H__
#include "sys.h"
#include "delay.h"

#define		POWER_5V0
//#define		POWER_3V3

//IO Definitions
#define		RS		PCout(8)
#define		RW		PCout(9)
#define		EN		PBout(1)

#define		RS_Pin		GPIO_Pin_8
#define		RW_Pin		GPIO_Pin_9
#define		EN_Pin		GPIO_Pin_1

typedef enum
{
	cmd,
	data
}Write_Mode;
extern void LCD1602Configuration(void);
extern void LCD_Show_Str(uint8_t x, uint8_t y, uint8_t *str);
extern void LCD_ClearScreen(void);
#endif

