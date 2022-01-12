
#ifndef __APP_H
#define __APP_H

#include <string.h>
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
#include "exti.h"
#include "key.h"
#include "DS18B20.h"
#include "DHT11.h"
#include "LCD1602.h"
#include "watchdog.h"

#define LED PBout(3)

/**@name 开始任务
*@{
*/
#define START_TASK_PRIO	3
#define START_STK_SIZE	128
extern OS_TCB	StartTackTCB;
extern CPU_STK	START_TASK_STK[START_STK_SIZE];
void start_task(volatile void *p_arg);
/**@}
*/

/**@name DS18B20任务
*@{
*/
#define DS18B20_TASK_PRIO 6
#define DS18B20_STK_SIZE  128
extern OS_TCB DS18B20TCB;
extern CPU_STK DS18B20_STK[DS18B20_STK_SIZE];
void DS18B20_task(volatile void *p_arg);
/**@}
*/


/**@name DHT11任务
*@{
*/
#define DHT11_TASK_PRIO 7
#define DHT11_STK_SIZE  128
extern OS_TCB DHT11TCB;
extern CPU_STK DHT11_STK[DHT11_STK_SIZE];
void DHT11_task(volatile void *p_arg);
/**@}
*/
/**@name DHT11任务
*@{
*/
#define DISPLAY_SCOPE_TASK_PRIO 8
#define DISPLAY_SCOPE_STK_SIZE  128
extern OS_TCB DISPLAY_SCOPETCB;
extern CPU_STK DISPLAY_SCOPE_STK[DHT11_STK_SIZE];
void DISPLAY_SCOPE_task(volatile void *p_arg);
/**@}
*/

/**@name 定时器任务
*@{
*/
/*定时器1回调函数*/
extern OS_TMR tmr1;
void tmr1_callback_led(void *p_tmr, void *p_arg);
/**@}
*/
void DISPLAY_CHAR_init();
#endif
