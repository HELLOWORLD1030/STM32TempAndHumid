/**
  ************************************
  * @file    sys.c
  * @author  wszdxmh
  * @version V3.5.0
  * @date    2015.08.08
  * @brief   中断分组函数
	************************************
*/
#include "sys.h"

void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/**
  ************************************
  * @brief  NVIC分组初始化函数
  * @param  None
  * @retval None
	************************************
*/
void NVIC_Configuration(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}
