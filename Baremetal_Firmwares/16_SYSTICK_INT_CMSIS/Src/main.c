#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart2.h"
#include "systick.h"

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
	printf("Hello World!\r\n");
	systick_1hz_interrupt();
	while(1)
	{

	}
}

static void sysTick_callback(void)
{
	GPIOA->ODR ^= LED;
	printf("1sec Interrupt\r\n");
}

void SysTick_Handler(void)
{
	sysTick_callback();
}



