#ifndef __LCD1602_H
#define __LCD1602_H


#include "stm32f10x.h"

#define RS_SET GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define RS_CLR GPIO_ResetBits(GPIOC,GPIO_Pin_8)

#define RW_CLR GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define RW_SET GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define EN_CLR GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define EN_SET GPIO_SetBits(GPIOB,GPIO_Pin_1)


void lcd_write_char(u8 x,u8 y,u8 data);
void lcd_write_string(u8 x,u8 y,u8* str);
void lcd_init(void);
void gpio_config(void);

#endif
