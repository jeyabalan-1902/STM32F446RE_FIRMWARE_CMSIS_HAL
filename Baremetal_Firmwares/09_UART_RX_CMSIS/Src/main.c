#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart2.h"

#define GPIOAEN (1U << 0)
#define PIN5    (1U << 5)

#define LED     PIN5

char rx_buff;

int main(void)
{
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);
	UART2_Init();

	while(1)
	{
		rx_buff = uart2_receive();
		if(rx_buff == '1')
		{
			GPIOA->ODR |= LED;
		}
		else
		{
			GPIOA->ODR &= ~(LED);
		}
	}
}



