/*
 * exti.c
 *
 *  Created on: May 26, 2025
 *      Author: kjeyabalan
 */


#define GPIOCEN   (1U << 2)
#define SYSCFG_EN (1U << 14)

#include "exti.h"

void pc13_exti_init(void)
{
	/*disable global interrupt*/
	__disable_irq();
	/*Enable clock access for GPIOC*/
	RCC->AHB1ENR |= GPIOCEN;
	/*Setting up the PC13 as input*/
	GPIOC->MODER &= ~(1U << 26);
	GPIOC->MODER &= ~(1U << 27);
	/*Enable clock acces for SYSCFG*/
	RCC->APB2ENR |= SYSCFG_EN;
	/*Select PORTC for EXTI13*/
	SYSCFG->EXTICR[3] |= (1U << 5);
	/*Unmask EXTI13*/
	EXTI->IMR |= (1U << 13);
	/*Select falling edge trigger*/
	EXTI->FTSR |= (1U << 13);
	/*Enable EXTI13 line in NVIC*/
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	/*Enable Global interrupts*/
	__enable_irq();
}
