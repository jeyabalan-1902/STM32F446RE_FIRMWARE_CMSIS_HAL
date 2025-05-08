#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart2.h"
#include "systick.h"
#include "tim.h"


char data;

int __io_putchar(int ch)
{
	UART2_Transmit(ch);
	return ch;
}

int main(void)
{
    tim2_PA5_output_compare();
	while(1)
	{
	}
}



