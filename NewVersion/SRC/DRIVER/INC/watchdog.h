
#ifndef __WATCHDOG_H
#define __WATCHDOG_H

#include "stm32f10x_wwdg.h"

#define IWDG_FREE()		IWDG->KR=0XAAAA;

void IWDG_Init(uint8_t prer, uint16_t rlr);

#endif
