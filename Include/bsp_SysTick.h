#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "my_register.h"

void SysTick_Init(void);
void Delay_us(vu32 nTime);         // µ¥Î»1us
void TimingDelay_Decrement(void);
void bsp_DelayMS(uint32_t _ulDelayTime);
#endif /* __SYSTICK_H */
