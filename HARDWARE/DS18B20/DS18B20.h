#ifndef _DS18B20_H
#define _DS18B20_H

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif
//ΪʲôҪ������������ģʽ����ΪDS18B20�ǵ����ߣ�һ���߽�����������������Ҫ��ͣ���л�ģʽ
#define DS18B20_OutPut_Mode() {GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= 0x30000000;}//���ģʽ
#define DS18B20_InPut_Mode()  {GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= 0x80000000;}//����ģʽ

int Ds18b20_Init(void);
int Ds18b20ReadTemp(void);
#endif
