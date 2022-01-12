
#include "watchdog.h"

/**
  ************************************
  * @brief  初始化看门狗
  * @param  prer:0~7有效	rlr:低11位有效
  * @retval Tout=((4*2^prer)*rlr)/40 (ms)
	************************************
*/
void IWDG_Init(uint8_t prer, uint16_t rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prer);
	IWDG_SetReload(rlr);
	IWDG_ReloadCounter();
	IWDG_Enable();
}
