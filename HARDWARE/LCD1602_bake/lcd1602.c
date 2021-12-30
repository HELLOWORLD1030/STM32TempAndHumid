#include "lcd1602.h"
#include "sys.h"

/*
������������1602������Ϊ���ģʽ
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
�������ܣ���1602д����
��ڲ�����com Ϊ������
ʱ���ϵ�� RS = 0��RW = 0 EN = ������
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
	�������� ����1602дһ��ascii��
	��ڲ�����ascii��
	ʱ���ϵ��RS = 1; RW = 0; EN = ������
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
��������  ��1602ָ��λ��дһ���ַ�
��ڲ�����x ������ y ������ data д����ַ�
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
��������  ��1602ָ��λ��дһ���ַ���
��ڲ�����x ������ y ������ *str д����ַ���
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


//����
void lcd_clear(){
	lcd_write_com(0x01);
	delay_ms(5);
}

//��ʼ������
//����1602��˵���� �о������
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
	lcd_write_com(0x08);  //����ʾ
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
