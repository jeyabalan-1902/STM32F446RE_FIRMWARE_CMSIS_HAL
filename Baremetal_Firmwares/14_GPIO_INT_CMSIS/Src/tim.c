/*
 * tim.c
 *
 *  Created on: May 3, 2025
 *      Author: kjeyabalan
 */


#include "stm32f4xx.h"

#include "tim.h"

#define TIM2EN     (1U << 0)
#define TIM3EN     (1U << 1)
#define CR1_CEN    (1U << 0)

#define OCM_TOGGLE ((1U << 4) | (1U << 5))
#define OCM_EN     (1U << 0)

#define GPIOAEN (1U << 0)
#define PIN5    (1U << 5)

#define LED     PIN5
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

void tim2_PA5_output_compare(void)
{
	/*Enable AHB1 clock for PA5*/
	RCC->AHB1ENR |= GPIOAEN;
	/*setting up the PA5 mode as alternate function mode*/
	GPIOA->MODER |= (1U << 11);
	GPIOA->MODER &= ~(1U << 10);

	/* setting up PA5 alternate function type to TIM2_CH1*/
	GPIOA->AFR[0] |= (1U << 20);

	/* enabling apb1 clock access for timer 2*/
	RCC->APB1ENR |= TIM2EN;
	/* set prescaler value*/
	TIM2->PSC = 16000 - 1;    //16000000 / 16000 = 1000
	/*set auto reload value*/
	TIM2->ARR = 1000 - 1;    // 1000 / 1000 = 1

	/*set output compare toggle mode */
	TIM2->CCMR1 |= OCM_TOGGLE;
	/*Enable tim2 ch1 in compare mode*/
    TIM2->CCER |= OCM_EN;
	/*clear counter*/
	TIM2->CNT = 0;
	/*enable timer*/
	TIM2->CR1 |= CR1_CEN;
}

void tim3_input_capture(void)
{
	/*enable clock access for GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;
	/*Set PA6 mode to alternate function*/
	GPIOA->MODER |= (1U << 13);
	GPIOA->MODER &= ~(1U << 12);
	/*set PA6 alternate function type to TIM_CH1 (AF02).*/
	GPIOA->AFR[0] |= (1U << 25);
	/*Enable clock acces for tim3*/
	RCC->APB1ENR |= TIM3EN;
	/*set prescaler*/
	TIM3->PSC = 16000 -1;
	/*set CH1 to input capture mode*/
	TIM3->CCMR1 |= (1U << 0);
	/*set CH1 to capture at rising edge*/
	TIM3->CCER = (1U << 0);
	/* Enable TIM3*/
	TIM3->CR1 |= CR1_CEN;
}

