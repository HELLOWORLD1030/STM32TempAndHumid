#ifndef __DHT11_H__
#define __DHT11_H__
#define DHT11_GPIO_TYPE  GPIOC
#define DHT11_GPIO_PIN   GPIO_Pin_13
#define DHT11_RCC        RCC_APB2Periph_GPIOC
#include "sys.h"

#define DHT11_OUT_H GPIO_SetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_OUT_L GPIO_ResetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_IN    GPIO_ReadInputDataBit(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)


void dht11_gpio_input(void);
void dht11_gpio_output(void);
u16 dht11_scan(void);
u16 dht11_read_bit(void);
u16 dht11_read_byte(void);
u16 dht11_read_data(u8 buffer[4]);
#endif