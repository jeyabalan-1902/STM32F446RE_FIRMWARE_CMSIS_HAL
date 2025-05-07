/*
 * systick.c
 *
 *  Created on: Apr 28, 2025
 *      Author: kjeyabalan
 */


#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL       16000
#define CTRL_ENABLE            (1U << 0)
#define CTRL_CLKSRC            (1U << 2)
#define CTRL_COUNTFLAG      (1U << 16)

void systickDelayMs(int delay)
{
	/*reload with no of clocks per millisecond*/
	SysTick->LOAD = SYSTICK_LOAD_VAL;
	/*clear systick current value register*/
	SysTick->VAL = 0;
	/*enable systick and select internal clock source*/
	SysTick->CTRL |= CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0; i < delay; i++)
	{
		/*wait until count flag is set*/
		while(((SysTick->CTRL & CTRL_COUNTFLAG) == 0)){}
	}
	SysTick->CTRL = 0;

}
