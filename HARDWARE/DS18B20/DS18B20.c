#include "stdio.h"
#include "sys.h"
#include "DS18B20.h"
#include "delay.h"



int Ds18b20_Init(){
		int i=0;
		RCC->APB2ENR|=(1<<3);//����GPIOʱ��
		GPIOB->ODR |= 1<<7;//��GPIOB_PB7��Ϊ�ߵ�ƽ
		DS18B20_OutPut_Mode();//��PB7�������ģʽ
		PBout(7)=0;//���0 ����
		delay_us(642);//642 �鿴ʱ��ͼ�ó� ����480С��960΢��
		PBout(7)=1;//���1 ���� Mark1
		delay_us(30);//��ʱ��ͼ�ó� ����15С��60΢��
		
		DS18B20_InPut_Mode();//��PB7��������ģʽ
		while(PBin(7)==1){//�յ��͵�ƽ����ڣ�����ѭ��5ms���߲����� ��ʱ��ͼӦ������60-240΢�� 
					delay_ms(1);
					i++;
					if(i>5){
							return 0;//�����ڷ���0 Mark2
					}
		}
		return 1;//���ڷ���1
		//ʱ��ͼ���Կ�����Mark1��Mark2֮�䲻�ܶ���300΢�룬���������
}


void Ds18b20WriteByte(uchar dat){
	int i;
	DS18B20_OutPut_Mode();
	for(i=0;i<8;i++){
		PBout(7)=0;	//����дʱ������
		delay_us(15);//д��ʱ�����ʹ���15΢�� 
		PBout(7)=dat&0x01;//д��0��1
		delay_us(60);//д��1��0ʱ����Ҫ����60΢��ļ�϶ 
		PBout(7)=1;//�����߻ָ���д״̬
		dat>>=1;//һ��8λ����һλ����һλ���ݷ������ұ�
	}
}
uchar Ds18b20ReadByte(){
	int j;
	uchar dat,byte;
	for(j=8;j>0;j--){
	DS18B20_OutPut_Mode();
	PBout(7)=0;//����ʱ��
	delay_us(1);//�����ӳ�1΢��
	PBout(7)=1;
	delay_us(10);//�������׼���׶�10΢��
	DS18B20_InPut_Mode();
	dat=PBin(7);
	byte=(byte>>1)|(dat<<7);
	delay_us(45);//�ӳ�45΢�����1λ
	DS18B20_OutPut_Mode();
	PBout(7)=1;//��������Ϊ����һλ��׼��
	}
	return byte;
}

void Ds18b20ChangeTemp(){
	Ds18b20_Init();
	delay_ms(1);
	Ds18b20WriteByte(0xcc);//����ROMֱ�ӷ����¶�ת������
	Ds18b20WriteByte(0x44);//����ָ��RAM��Ϊ0x44Ϊ�¶ȱ任
}

void Ds18b20ReadTempCom(){
	Ds18b20_Init();
	delay_ms(1);
	Ds18b20WriteByte(0xcc);//����ROMֱ�ӷ����¶�ת������
	Ds18b20WriteByte(0xbe);//����ָ��RAM��Ϊ0xBEΪ����ʱ�Ĵ���
}

int Ds18b20ReadTemp(void){
	int temp=0;
	uchar tml,tmh;	
	Ds18b20ChangeTemp();
	Ds18b20ReadTempCom();
	tml=Ds18b20ReadByte();//����8λ����
	tmh=Ds18b20ReadByte();//����8λ����
	temp=tmh;
	temp<<=8;
	temp|=tml;//ƴ��Ϊ16λ����
	return temp;//����16λ����
}
