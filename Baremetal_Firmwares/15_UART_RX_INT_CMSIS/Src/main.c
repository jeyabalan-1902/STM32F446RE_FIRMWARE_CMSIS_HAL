#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart2.h"

#define GPIOAEN (1U << 0)
#define PIN5    (1U << 5)
#define SR_RXNE     (1U << 5)

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

	while(1)
	{

	}
}

static void uart_callback(void)
{
	printf("interrupt aquired\r\n");
	data = USART2->DR;
	if(data == '1')
	{
		GPIOA->ODR ^= LED;
	}
}


void USART2_IRQHandler(void)
{
	/*check if RXNE is set*/
	if(USART2->SR & SR_RXNE)
	{
		uart_callback();
	}
}


