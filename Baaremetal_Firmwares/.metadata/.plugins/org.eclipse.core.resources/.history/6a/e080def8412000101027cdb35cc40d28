#include "stm32f4xx.h"


#define UART2EN     (1U << 17)
#define GPIOAEN     (1U << 0)

int main(void)
{
	RCC->APB1ENR |= UART2EN;    //Enabling APB1 clock for UART2
	RCC->AHB1ENR |= GPIOAEN;    //Enabling AHB1 clock for PORT A, because UART 2 uses PA2 - TX and PA3 - RX

	/* setting PA2 - TX as alternate function mode by using set 1 bit 4 and 5 as configuration 1:0*/
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);

	while(1)
	{

	}
}
