/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Jeyabalan K
 * @brief          : Baremetal registers for LED Toggling
 ******************************************************************************
 */


/**
 * LED Pin :  PA5
 */

#include <stdint.h>

#define PERIPHERAL_BASE     (0x40000000UL)
#define AHB1_OFFSET         (0x00020000UL)
#define AHB1_BASE           (PERIPHERAL_BASE + AHB1_OFFSET)

#define GPIOA_OFFSET        (0x0000UL)
#define GPIOA_BASE          (AHB1_BASE + GPIOA_OFFSET)

#define RCC_OFFSET          (0x3800UL)
#define RCC_BASE            (AHB1_BASE + RCC_OFFSET)

#define AHB1ENR_OFFSET      (0x30UL)
#define RCC_AHB1ENR         (*(volatile unsigned int *)(RCC_BASE + AHB1ENR_OFFSET))        //RCC Clock enable regiater for AHB1

#define GPIO_MODER_OFFSET   (0x00UL)
#define GPIOA_MODER         (*(volatile unsigned int *)(GPIOA_BASE + GPIO_MODER_OFFSET))   //GPIO Mode register : input/output

#define GPIO_ODR_OFFSET     (0x14UL)
#define GPIOA_ODR           (*(volatile unsigned int *)(GPIOA_BASE + GPIO_ODR_OFFSET))     //output data register

#define GPIOAEN             (1U << 0)

#define PIN5                (1U << 5)
#define LED_PIN             PIN5

void delay(uint32_t t)
{
	while(t--);
}

int main(void)
{
	RCC_AHB1ENR |= GPIOAEN;                // Enable clock for GPIOA
	GPIOA_MODER |= (1U << 10);             // Set PA5 as output
	GPIOA_MODER &= ~(1U << 11);

	while(1)
	{
		GPIOA_ODR |= LED_PIN;              // Set PA5 high
		delay(1000000);
		GPIOA_ODR &= ~(LED_PIN);
		delay(1000000);
	}
}
