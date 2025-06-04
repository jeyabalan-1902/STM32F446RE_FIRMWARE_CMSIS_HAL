#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart2.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"


#define GPIOAEN (1U << 0)
#define PIN5    (1U << 5)

#define LED     PIN5

int __io_putchar(int ch)
{
	UART2_Transmit(ch);
	return ch;
}

int main(void)
{
	pc13_exti_init();
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);
	UART2_Init();
	while(1)
	{

	}
}

static void exti_callback(void)
{
	printf("button pressed\r\n");
	GPIOA->ODR ^= LED;
}

void EXTI15_10_IRQHandler(void)
{
	if ((EXTI->PR & LINE13)!= 0)
	{
		/*clear the PR flag*/
		EXTI->PR |= LINE13;
		exti_callback();
	}
}



