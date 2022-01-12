	
#include "app.h"
typedef struct {
	int hum;
	int temp;
} TempCmd;
short tempvaule;
u8 buffer[5];
u8 u1buffer[4]={0};   
u8 u2buffer[4]={0}; 
int MaxTemp=120;
int MinTemp=10;
int MaxHum=90;
int MinHum=10;
OS_Q KEY_Msg;
void LCD_Disp(int u1,int u2);
/*开始任务*/
OS_TCB	StartTackTCB;
CPU_STK	START_TASK_STK[START_STK_SIZE];

//DS18B20任务
OS_TCB DS18B20TCB;
CPU_STK DS18B20_STK[DS18B20_STK_SIZE];


OS_TCB DHT11TCB;
CPU_STK DHT11_STK[DHT11_STK_SIZE];


OS_TCB DISPLAY_SCOPETCB;
CPU_STK DISPLAY_SCOPE_STK[DISPLAY_SCOPE_STK_SIZE];


/*定时器1回调函数*/
OS_TMR tmr1;

/**DS18B20任务**/
void DS18B20_task(volatile void *p_arg)
{
	TempCmd tempCmd;
	uint8_t cnt = 0;
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSSchedLock(&err);
		if (cnt == 0)
		{
			DS18B20_Init();
			cnt = 1;
		}
		tempCmd.temp = DS18B20_Get_Temp();
		if (dht11_read_data(buffer) == 0)
        {
            tempCmd.hum = buffer[0] + buffer[1] / 10.0;
        }
		if(tempCmd.temp/10>MaxTemp||tempCmd.temp/10<MinTemp){
			LED4=!LED4;
		}
		if(tempCmd.hum>MaxHum||tempCmd.hum<MinHum){
			LED3 =!LED3;
		}
		
		printf("hum = %.2f,temp = %d\r\n",tempCmd.hum,tempCmd.hum);
		
		OSSchedUnlock(&err);
		OSQPost((OS_Q*		)&KEY_Msg,		
					(void*		)&tempCmd,
					(OS_MSG_SIZE)1,
					(OS_OPT		)OS_OPT_POST_FIFO,
					(OS_ERR*	)&err);

		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
void DHT11_task(volatile void *p_arg)
{
	TempCmd *temp;
	OS_ERR err;
	OS_MSG_SIZE size;
	
	while(1)
	{
		temp=OSQPend((OS_Q*			)&KEY_Msg,   
					(OS_TICK		)0,
                    (OS_OPT			)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*	)&size,		
                    (CPU_TS*		)0,
                    (OS_ERR*		)&err);
		LCD_Disp((*temp).temp,(*temp).hum);//??u1?u2
		OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
        
		
		
	
}

//定时器1回调函数
void tmr1_callback_led(void *p_tmr, void *p_arg)
{
	IWDG_FREE();
}
void DISPLAY_SCOPE_task(volatile void *p_arg){
	OS_ERR err;
	while(1){
	lcd_write_char(8,0,MinTemp/10+0X30);
	lcd_write_char(9,0,MinTemp%10+0x30);
	lcd_write_char(11,0,MaxTemp/100+0X30);
	lcd_write_char(12,0,MaxTemp/10%10+0x30);
	lcd_write_char(13,0,MaxTemp%10+0x30);
	lcd_write_char(8,1,MinHum/10+0X30);
	lcd_write_char(9,1,MinHum%10+0x30);
	lcd_write_char(11,1,MaxHum/10+0x30);
	lcd_write_char(12,1,MaxHum%10+0x30);
	OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	
	}

}

//开始任务控制函数
void start_task(volatile void *p_arg)
{
  OS_ERR err;
  CPU_SR_ALLOC();
  p_arg = p_arg;
  CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif

	OS_CRITICAL_ENTER();//进入临界区代码保护
	OSQCreate ((OS_Q*		)&KEY_Msg,	//????
                (CPU_CHAR*	)"KEY Msg",	//??????
                (OS_MSG_QTY	)1,	//??????,?????1
                (OS_ERR*	)&err);		//???
	/*创建任务DS18B20*/
	OSTaskCreate ((OS_TCB        *)&DS18B20TCB,
								(CPU_CHAR      *)"DS18B20",
								(OS_TASK_PTR    )DS18B20_task,
								(void          *)0,
								(OS_PRIO        )DS18B20_TASK_PRIO,
								(CPU_STK       *)DS18B20_STK,
								(CPU_STK_SIZE   )DS18B20_STK_SIZE/10,
								(CPU_STK_SIZE   )DS18B20_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
	/*创建任务DS18B20*/
								
	OSTaskCreate ((OS_TCB        *)&DHT11TCB,
								(CPU_CHAR      *)"DHT11",
								(OS_TASK_PTR    )DHT11_task,
								(void          *)0,
								(OS_PRIO        )DHT11_TASK_PRIO,
								(CPU_STK       *)DHT11_STK,
								(CPU_STK_SIZE   )DHT11_STK_SIZE/10,
								(CPU_STK_SIZE   )DHT11_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
								
								
								
	OSTaskCreate ((OS_TCB        *)&DISPLAY_SCOPETCB,
								(CPU_CHAR      *)"DISPLAY_SCOPE",
								(OS_TASK_PTR    )DISPLAY_SCOPE_task,
								(void          *)0,
								(OS_PRIO        )DISPLAY_SCOPE_TASK_PRIO,
								(CPU_STK       *)DISPLAY_SCOPE_STK,
								(CPU_STK_SIZE   )DISPLAY_SCOPE_STK_SIZE/10,
								(CPU_STK_SIZE   )DISPLAY_SCOPE_STK_SIZE,
								(OS_MSG_QTY     )0,
								(OS_TICK        )0,
								(void          *)0,
								(OS_OPT         )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
								(OS_ERR        *)&err);
								
	/*创建软件定时器1*/
	OSTmrCreate	((OS_TMR         			 *)&tmr1,
							 (CPU_CHAR             *)"tmr1_led",
							 (OS_TICK               )0,//初始延迟为0*10ms
							 (OS_TICK               )1,//定时周期为50*10ms
							 (OS_OPT                )OS_OPT_TMR_PERIODIC,//周期模式
							 (OS_TMR_CALLBACK_PTR   )tmr1_callback_led,
							 (void                 *)0,
							 (OS_ERR               *)&err);
	OS_CRITICAL_EXIT();//退出临界区代码保护
	OSTmrStart(&tmr1,&err);
}
void DISPLAY_CHAR_init(){
	lcd_write_string(0,0,(u8*)"T=");
	lcd_write_string(4,0,(u8*)".");
	lcd_write_string(6,0,(u8*)"C");
	lcd_write_string(0,1,(u8*)"H=");
	
	lcd_write_string(5,1,(u8*)"%");
	
}
void LCD_Disp(int u1,int u2){
	int count1=0;
	while(u1!=0){
		u1buffer[count1]=u1%10;
		//printf("temp1:%d\r\n",tempCmd.temp%10);
		u1/=10;
		count1++;
	}
		int count2=0;
		while(u2!=0){
			u2buffer[count2]=u2%10;
		//printf("hum1:%d\r\n",tempCmd.hum%10);
		u2/=10;
			count2++;
	}
	lcd_write_char(2,0,u1buffer[2]+0X30);
	lcd_write_char(3,0,u1buffer[1]+0X30);
	lcd_write_char(5,0,u1buffer[0]%10+0X30);
	//lcd_write_char(5,1,u2buffer[1]+0X30);
	lcd_write_char(2,1,u2buffer[1]+0X30);
	lcd_write_char(3,1,u2buffer[0]+0X30);
	
}

