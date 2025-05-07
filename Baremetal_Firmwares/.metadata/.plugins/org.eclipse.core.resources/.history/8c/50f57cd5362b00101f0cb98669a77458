/*
 * tim.c
 *
 *  Created on: May 3, 2025
 *      Author: kjeyabalan
 */


#include "stm32f4xx.h"

#define TIM2EN    (1U << 0)
#define CR1_CEN   (1U << 0)

/* Prescaler Formula
 *
 * timer frequency = timer clock / (prescaler + 1)
 * prescaler = (timer clock/timer frequency) - 1
 *
 *
 * 1 MHz =  100000   => 1 sec
 * 1 KHz =  1000     => 1 ms
 */


void tim2_1Hz_init(void)
{
	/* enabling apb1 clock access for timer 2*/
	RCC->APB1ENR |= TIM2EN;
	/* set prescaler value*/
	TIM2->PSC = 16000 - 1;    //16000000 / 16000 = 1000
	/*set auto reload value*/
	TIM2->ARR = 1000 - 1;    // 1000 / 1000 = 1
	/*clear counter*/
	TIM2->CNT = 0;
	/*enable timer*/
	TIM2->CR1 |= CR1_CEN;
}
