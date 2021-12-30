#include "lcd1602.h"
#include "sys.h"

/*
函数功能配置1602的引脚为输出模式
*/
void gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
	GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 ;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*
函数功能：向1602写命令
入口参数：com 为命令字
时序关系： RS = 0，RW = 0 EN = 高脉冲
*/
void lcd_write_com(u8 com){
	EN_CLR;
	
	delay_us(1);
	RS_CLR;
	delay_us(1);
	RW_CLR;
	delay_us(10);
	
	GPIOC->ODR = (GPIOC->ODR&0xFF00)|com;;
	//GPIO_Write(GPIOC,com);
	delay_us(400);

	EN_SET;
	delay_us(400);
	EN_CLR;

	delay_us(300);
}
/*
	函数功能 ：向1602写一个ascii码
	入口参数：ascii码
	时序关系：RS = 1; RW = 0; EN = 高脉冲
*/
void lcd_write_ascii(u8 ascii){
	EN_CLR;
	
	delay_us(1);
	RS_SET;
	delay_us(1);
	RW_CLR;
	delay_us(10);

	GPIOC->ODR = (GPIOC->ODR&0xFF00)|ascii;
	//GPIO_Write(GPIOC,ascii);
	delay_us(300);
	EN_SET;
	
	delay_us(400);
	EN_CLR;

	delay_us(300);
}
/*
函数功能  向1602指定位置写一个字符
入口参数：x 横坐标 y 纵坐标 data 写入的字符
*/
void lcd_write_char(u8 x,u8 y,u8 data){
	if(y==0){
		lcd_write_com(0x80+x);
	}
	else{
		lcd_write_com(0xc0+x);
	}
	
	lcd_write_ascii(data);
	delay_us(500);
}
/*
函数功能  向1602指定位置写一个字符串
入口参数：x 横坐标 y 纵坐标 *str 写入的字符串
*/
void lcd_write_string(u8 x,u8 y,u8* str){
	if(y==0)
		lcd_write_com(0x80+x);
	else
		lcd_write_com(0xc0+x);
	while(*str){
		lcd_write_ascii(*str);
		str++;
		delay_us(500);
	}
		
}


//清屏
void lcd_clear(){
	lcd_write_com(0x01);
	delay_ms(5);
}

//初始化函数
//参照1602的说明书 有具体介绍
void lcd_init(){

	/*
	delay_ms(100);
	lcd_write_com(0x38); 
	delay_ms(10);
	lcd_write_com(0x38);
	delay_ms(10);
	lcd_write_com(0x38);
	delay_ms(10);
	lcd_write_com(0x38);
	delay_ms(10);
	lcd_write_com(0x08);  //开显示
	delay_ms(10);
	lcd_write_com(0x01); 
	delay_ms(10);
	lcd_write_com(0x06); 
	delay_ms(10);
	lcd_write_com(0x0c); 
	delay_ms(10);
	*/
	gpio_config();
	
	delay_ms(15);
		
	lcd_write_com(0X38);
	lcd_write_com(0X0C);//????????
	lcd_write_com(0X06);//??????1
	lcd_write_com(0X01);//??
	lcd_write_com(0X80);//????????	
}
