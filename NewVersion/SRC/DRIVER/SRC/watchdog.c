
#include "watchdog.h"

/**
  ************************************
  * @brief  ��ʼ�����Ź�
  * @param  prer:0~7��Ч	rlr:��11λ��Ч
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
