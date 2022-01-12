#include "app.h"
int main(void)
{
	OS_ERR err;
	SystemInit();
	delay_init();
	LED_Init();
	Usart_Init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //ÖÐ¶Ï·Ö×éÅäÖ
	EXTIX_Init();
	KEY_Init();
	LED3=1;
	gpio_config();
	lcd_init();
	DISPLAY_CHAR_init();
	CPU_SR_ALLOC();
	OSInit(&err);
	OS_CRITICAL_ENTER();//½øÈëÁÙ½çÇø´úÂë±£»¤
	/*´´½¨ÈÎÎñ*/
	OSTaskCreate ((OS_TCB        *)&StartTackTCB,
								(CPU_CHAR      *)"start_task",
								(OS_TASK_PTR    )start_task,
								(void          *)0,
								(OS_PRIO        )START_TASK_PRIO,
								(CPU_STK       *)START_TASK_STK,
								(CPU_STK_SIZE   )START_STK_SIZE/10,
								(CPU_STK_SIZE   )START_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	OS_CRITICAL_EXIT();//ÍË³öÁÙ½çÇø´úÂë±£»¤
	OSStart(&err);
	OSTaskDel((OS_TCB*)&StartTackTCB, (OS_ERR*)&err);
	while(1);
}
