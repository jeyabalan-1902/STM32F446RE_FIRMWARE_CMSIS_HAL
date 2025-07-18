#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart2.h"
#include "systick.h"
#include "tim.h"

#define GPIOAEN (1U << 0)
#define PIN5    (1U << 5)

#define LED     PIN5

char data;

int __io_putchar(int ch)
{
	UART2_Transmit(ch);
	return ch;
}

int main(void)
{
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);
	UART2_Init();
	tim2_1Hz_interrupt_init();

	while(1)
	{

	}
}

static void tim2_callback(void)
{
	GPIOA->ODR ^= LED;
	printf("TIM2 interrupt 1 sec\r\n");
}


void TIM2_IRQHandler(void)
{
	/*clear the interuupt flag*/
	TIM2->SR &= ~(SR_UIF);
	tim2_callback();
}


