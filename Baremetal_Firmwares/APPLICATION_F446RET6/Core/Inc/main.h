/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define KEYPAD_R1_Pin GPIO_PIN_0
#define KEYPAD_R1_GPIO_Port GPIOB
#define KEYPAD_R2_Pin GPIO_PIN_1
#define KEYPAD_R2_GPIO_Port GPIOB
#define KEYPAD_R3_Pin GPIO_PIN_2
#define KEYPAD_R3_GPIO_Port GPIOB
#define KEYPAD_R4_Pin GPIO_PIN_3
#define KEYPAD_R4_GPIO_Port GPIOB
#define KEYPAD_C1_Pin GPIO_PIN_4
#define KEYPAD_C1_GPIO_Port GPIOB
#define KEYPAD_C2_Pin GPIO_PIN_5
#define KEYPAD_C2_GPIO_Port GPIOB
#define KEYPAD_C3_Pin GPIO_PIN_6
#define KEYPAD_C3_GPIO_Port GPIOB
#define KEYPAD_C4_Pin GPIO_PIN_7
#define KEYPAD_C4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
