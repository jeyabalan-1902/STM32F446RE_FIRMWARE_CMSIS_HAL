/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BL_GET_VER            0x51
#define BL_GET_HELP           0x52
#define BL_GET_CID            0x53
#define BL_GET_RDP_STATUS     0x54
#define BL_GO_TO_ADDR         0x55
#define BL_FLASH_ERASE        0x56
#define BL_MEM_WRITE          0x57
#define BL_EN_RW_PROTECT      0x58
#define BL_MEM_READ           0x59
#define BL_READ_SECTOR_STATUS 0x5A
#define BL_OTP_READ           0x5B
#define BL_DIS_R_W_PROTECT    0x5C

#define BL_RX_LEN             200

#define BL_ACK                0xA5
#define BL_NACK               0x7F

#define VERIFY_CRC_FAIL       1
#define VERIFY_CRC_SUCCESS    0

#define BL_VERSION            0x10   //version 1.0


#define FLASH_SECTOR2_BASE_ADDRESS 0x08008000U

#define ADDR_VALID 0x00
#define ADDR_INVALID 0x01

#define INVALID_SECTOR 0x04

/*Some Start and End addresses of different memories of STM32F446xx MCU */
/*Change this according to your MCU */
#define SRAM1_SIZE            112*1024     // STM32F446RE has 112KB of SRAM1
#define SRAM1_END             (SRAM1_BASE + SRAM1_SIZE)
#define SRAM2_SIZE            16*1024     // STM32F446RE has 16KB of SRAM2
#define SRAM2_END             (SRAM2_BASE + SRAM2_SIZE)
#define FLASH_SIZE             512*1024     // STM32F446RE has 512KB of SRAM2
#define BKPSRAM_SIZE           4*1024     // STM32F446RE has 4KB of SRAM2
#define BKPSRAM_END            (BKPSRAM_BASE + BKPSRAM_SIZE)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t bl_rx_buffer[BL_RX_LEN];

uint8_t supported_commands[] = {
                               BL_GET_VER ,
                               BL_GET_HELP,
                               BL_GET_CID,
                               BL_GET_RDP_STATUS,
                               BL_GO_TO_ADDR,
                               BL_FLASH_ERASE,
                               BL_MEM_WRITE,
							   BL_READ_SECTOR_STATUS} ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */
void bootloader_jump_to_application(void);
void bootloader_read_uart_data(void);
void bootloader_handle_read_otp(uint8_t *bl_rx_buffer);
void bootloader_handle_read_sector_status(uint8_t *bl_rx_buffer);
void bootloader_handle_mem_read(uint8_t *bl_rx_buffer);
void bootloader_handle_dis_rw_protect(uint8_t *bl_rx_buffer);
void bootloader_handle_endis_rw_protect(uint8_t *bl_rx_buffer);
void bootloader_handle_mem_write_cmd(uint8_t *bl_rx_buffer);
void bootloader_handle_flash_erase_cmd(uint8_t *bl_rx_buffer);
void bootloader_handle_go_cmd(uint8_t *bl_rx_buffer);
void bootloader_handle_getrdp_cmd(uint8_t *bl_rx_buffer);
void bootloader_handle_getcid_cmd(uint8_t *bl_rx_buffer);
void bootloader_handle_gethelp_cmd(uint8_t *bl_rx_buffer);
void bootloader_handle_getver_cmd(uint8_t *bl_rx_buffer);
void bootloader_send_ack(uint8_t command_code, uint8_t follow_len);
void bootloader_send_nack(void);
uint8_t bootloader_verify_crc(uint8_t *pData, uint32_t len, uint32_t crc_host);
void bootloader_uart_write_data(uint8_t *pBuffer, uint32_t len);
uint8_t get_bootloader_version(void);

uint16_t get_mcu_chip_id(void);
uint8_t get_flash_rdp_level(void);
uint8_t verify_address(uint32_t go_address);
uint8_t execute_flash_erase(uint8_t sector_number , uint8_t number_of_sector);
uint8_t execute_mem_write(uint8_t *pBuffer, uint32_t mem_address, uint32_t len);

uint8_t configure_flash_sector_rw_protection(uint8_t sector_details, uint8_t protection_mode, uint8_t disable);

uint16_t read_OB_rw_protection_status(void);

void bootloader_jump_to_user_app(uint32_t app_start_address);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  printf("Application jump to bootloader mode....\n\r");
  bootloader_read_uart_data();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : button_Pin */
  GPIO_InitStruct.Pin = button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void bootloader_read_uart_data(void)
{
	uint8_t rcv_len = 0;
	while(1)
	{
		memset(bl_rx_buffer, 0, 200);
		HAL_UART_Receive(&huart3, &bl_rx_buffer, 1, HAL_MAX_DELAY);
		rcv_len = bl_rx_buffer[0];
		HAL_UART_Receive(&huart3, &bl_rx_buffer[1], rcv_len, HAL_MAX_DELAY);
		switch(bl_rx_buffer[1])
		{
			case BL_GET_VER:
				bootloader_handle_getver_cmd(bl_rx_buffer);
				break;
			case BL_GET_HELP:
				bootloader_handle_gethelp_cmd(bl_rx_buffer);
				break;
			case BL_GET_CID:
				bootloader_handle_getcid_cmd(bl_rx_buffer);
				break;
			case BL_GET_RDP_STATUS:
				bootloader_handle_getrdp_cmd(bl_rx_buffer);
		 	    break;
			case BL_GO_TO_ADDR:
				bootloader_handle_go_cmd(bl_rx_buffer);
				break;
			case BL_FLASH_ERASE:
				bootloader_handle_flash_erase_cmd(bl_rx_buffer);
				break;
			case BL_MEM_WRITE:
				bootloader_handle_mem_write_cmd(bl_rx_buffer);
				break;
			case BL_EN_RW_PROTECT:
				bootloader_handle_endis_rw_protect(bl_rx_buffer);
				break;
			case BL_MEM_READ:
				bootloader_handle_mem_read(bl_rx_buffer);
				break;
			case BL_READ_SECTOR_STATUS:
				bootloader_handle_read_sector_status(bl_rx_buffer);
				break;
			case BL_OTP_READ:
				bootloader_handle_read_otp(bl_rx_buffer);
				break;
			default:
				break;

		}
	}
}

void bootloader_jump_to_user_app(uint32_t app_start_address)
{
    // Function pointer to the application's reset handler
    void (*app_reset_handler)(void);

    // Step 1: Read the MSP value from the application start address
    uint32_t msp_value = *(volatile uint32_t*)app_start_address;

    // Step 2: Read the reset handler address
    uint32_t reset_handler_address = *(volatile uint32_t*)(app_start_address + 4);

    printf("app reset handler addr: 0x%08lx\n", reset_handler_address);

    HAL_RCC_DeInit();
	HAL_DeInit();
	__disable_irq();
    __set_MSP(msp_value);

    SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	NVIC_SystemReset();

    // Step 4: Set the function pointer and jump
    app_reset_handler = (void*) (reset_handler_address | 0x1);  // Ensure Thumb bit is set
    app_reset_handler();
}

void bootloader_handle_read_otp(uint8_t *bl_rx_buffer)
{

}

void bootloader_handle_read_sector_status(uint8_t *bl_rx_buffer)
{
	uint16_t status;
	printf("BL_DEBUG_MSG:bootloader_handle_read_sector_protection_status\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");
		bootloader_send_ack(bl_rx_buffer[0],2);
		status=read_OB_rw_protection_status();
		printf("BL_DEBUG_MSG: nWRP status: %#x\n",status);
		bootloader_uart_write_data((uint8_t*)&status,2);
	}
	else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_mem_read(uint8_t *bl_rx_buffer)
{


}

void bootloader_handle_dis_rw_protect(uint8_t *bl_rx_buffer)
{
    uint8_t status = 0x00;
    printf("BL_DEBUG_MSG:bootloader_handle_dis_rw_protect\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
        printf("BL_DEBUG_MSG:checksum success !!\n");
        bootloader_send_ack(bl_rx_buffer[0],1);

        status = configure_flash_sector_rw_protection(0,0,1);

        printf("BL_DEBUG_MSG: flash erase status: %#x\n",status);

        bootloader_uart_write_data(&status,1);

	}
	else
	{
        printf("BL_DEBUG_MSG:checksum fail !!\n");
        bootloader_send_nack();
	}
}

void bootloader_handle_endis_rw_protect(uint8_t *bl_rx_buffer)
{
	uint8_t status = 0x00;
	printf("BL_DEBUG_MSG:bootloader_handle_endis_rw_protect\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");
		bootloader_send_ack(bl_rx_buffer[0],1);
		status = configure_flash_sector_rw_protection(bl_rx_buffer[2] , bl_rx_buffer[3],0);
		printf("BL_DEBUG_MSG: flash erase status: %#x\n",status);
		bootloader_uart_write_data(&status,1);

	}else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_mem_write_cmd(uint8_t *bl_rx_buffer)
{
	uint8_t write_status = 0x00;
	uint8_t chksum =0, len=0;
	len = bl_rx_buffer[0];
	uint8_t payload_len = bl_rx_buffer[6];
	uint32_t mem_address = *((uint32_t *) ( &bl_rx_buffer[2]) );
	chksum = bl_rx_buffer[len];
	printf("BL_DEBUG_MSG:bootloader_handle_mem_write_cmd\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;


	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");

		bootloader_send_ack(bl_rx_buffer[0],1);

		printf("BL_DEBUG_MSG: mem write address : %#x\n",mem_address);

		if( verify_address(mem_address) == ADDR_VALID )
		{
			printf("BL_DEBUG_MSG: valid mem write address\n");
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			write_status = execute_mem_write(&bl_rx_buffer[7],mem_address, payload_len);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			bootloader_uart_write_data(&write_status,1);
		}
		else
		{
			printf("BL_DEBUG_MSG: invalid mem write address\n");
			write_status = ADDR_INVALID;
			bootloader_uart_write_data(&write_status,1);
		}
	}
	else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_flash_erase_cmd(uint8_t *bl_rx_buffer)
{
	uint8_t erase_status = 0x00;
	printf("BL_DEBUG_MSG:bootloader_handle_flash_erase_cmd\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");
		bootloader_send_ack(bl_rx_buffer[0],1);
		printf("BL_DEBUG_MSG:initial_sector : %d  no_ofsectors: %d\n",bl_rx_buffer[2],bl_rx_buffer[3]);

		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,1);
		erase_status = execute_flash_erase(bl_rx_buffer[2] , bl_rx_buffer[3]);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0);

		printf("BL_DEBUG_MSG: flash erase status: %#x\n",erase_status);

		bootloader_uart_write_data(&erase_status,1);

	}else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_go_cmd(uint8_t *bl_rx_buffer)
{
	uint32_t go_address=0;
	uint8_t addr_valid = ADDR_VALID;
	uint8_t addr_invalid = ADDR_INVALID;

	printf("BL_DEBUG_MSG:bootloader_handle_go_cmd\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");

		bootloader_send_ack(bl_rx_buffer[0],1);

		//extract the go address
		go_address = *((uint32_t *)&bl_rx_buffer[2] );
		printf("BL_DEBUG_MSG:GO addr: %#x\n",go_address);

		if( verify_address(go_address) == ADDR_VALID )
		{
			bootloader_uart_write_data(&addr_valid,1);
            bootloader_jump_to_user_app(go_address);

		}
		else
		{
			printf("BL_DEBUG_MSG:GO addr invalid ! \n");
			bootloader_uart_write_data(&addr_invalid,1);
		}

	}else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_getrdp_cmd(uint8_t *bl_rx_buffer)
{
	uint8_t rdp_level = 0x00;
	printf("BL_DEBUG_MSG:bootloader_handle_getrdp_cmd\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");
		bootloader_send_ack(bl_rx_buffer[0],1);
		rdp_level = get_flash_rdp_level();
		printf("BL_DEBUG_MSG:RDP level: %d %#x\n",rdp_level,rdp_level);
		bootloader_uart_write_data(&rdp_level,1);

	}else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_getcid_cmd(uint8_t *bl_rx_buffer)
{
	uint16_t bl_cid_num = 0;
	printf("BL_DEBUG_MSG:bootloader_handle_getcid_cmd\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");
		bootloader_send_ack(bl_rx_buffer[0],2);
		bl_cid_num = get_mcu_chip_id();
		printf("BL_DEBUG_MSG:MCU id : %d %#x !!\n",bl_cid_num, bl_cid_num);
		bootloader_uart_write_data((uint8_t *)&bl_cid_num,2);

	}else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_gethelp_cmd(uint8_t *bl_rx_buffer)
{
	printf("BL_DEBUG_MSG:bootloader_handle_gethelp_cmd\n");
	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;
	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;

	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
	{
		printf("BL_DEBUG_MSG:checksum success !!\n");
		bootloader_send_ack(bl_rx_buffer[0],sizeof(supported_commands));
		bootloader_uart_write_data(supported_commands,sizeof(supported_commands) );

	}else
	{
		printf("BL_DEBUG_MSG:checksum fail !!\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_getver_cmd(uint8_t *bl_rx_buffer)
{
	uint8_t bl_version;
	printf("bootloader handle get version command\n");
	uint32_t command_packet_len = bl_rx_buffer[0] + 1;
	uint32_t host_crc = *((uint32_t *) (bl_rx_buffer + command_packet_len - 4));
	if (! bootloader_verify_crc(&bl_rx_buffer[0], command_packet_len-4, host_crc))
	{
		printf("checksum success !!\n");
		bootloader_send_ack(bl_rx_buffer[0], 1);
		bl_version = get_bootloader_version();
		printf("BL VERSION: %d %#x\n", bl_version, bl_version);
		bootloader_uart_write_data(&bl_version, 1);
	}
	else
	{
		printf("checksum failed !!\n");
		bootloader_send_nack();
	}
}

void bootloader_send_ack(uint8_t command_code, uint8_t follow_len)
{
	uint8_t ack_buf[2];
	ack_buf[0] = BL_ACK;
	ack_buf[1] = follow_len;
	HAL_UART_Transmit(&huart3, ack_buf, 2, HAL_MAX_DELAY);
}

void bootloader_send_nack(void)
{
	uint8_t nack = BL_NACK;
	HAL_UART_Transmit(&huart3, &nack, 1, HAL_MAX_DELAY);
}

uint8_t bootloader_verify_crc(uint8_t *pData, uint32_t len, uint32_t crc_host)
{
	uint32_t uwCRCValue = 0xff;
	__HAL_CRC_DR_RESET(&hcrc);
	for (uint32_t i = 0; i < len; i++)
	{
		uint32_t i_data = pData[i];
		uwCRCValue = HAL_CRC_Accumulate(&hcrc, &i_data, 1);
	}
	if(uwCRCValue == crc_host)
	{
		return VERIFY_CRC_SUCCESS;
	}
	return VERIFY_CRC_FAIL;
}

void bootloader_uart_write_data(uint8_t *pBuffer, uint32_t len)
{
	HAL_UART_Transmit(&huart3, pBuffer, len, HAL_MAX_DELAY);
}

uint8_t get_bootloader_version(void)
{
	return (uint8_t)BL_VERSION;
}

#define PRINTF   int __io_putchar(int ch)
PRINTF
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

uint16_t get_mcu_chip_id(void)
{
/*
	The STM32F446xx MCUs integrate an MCU ID code. This ID identifies the ST MCU partnumber
	and the die revision. It is part of the DBG_MCU component and is mapped on the
	external PPB bus (see Section 33.16 on page 1304). This code is accessible using the
	JTAG debug pCat.2ort (4 to 5 pins) or the SW debug port (two pins) or by the user software.
	It is even accessible while the MCU is under system reset. */
	uint16_t cid;
	cid = (uint16_t)(DBGMCU->IDCODE) & 0x0FFF;
	return  cid;

}


/*This function reads the RDP ( Read protection option byte) value
 *For more info refer "Table 9. Description of the option bytes" in stm32f446xx RM
 */
uint8_t get_flash_rdp_level(void)
{

	uint8_t rdp_status=0;
#if 0
	FLASH_OBProgramInitTypeDef  ob_handle;
	HAL_FLASHEx_OBGetConfig(&ob_handle);
	rdp_status = (uint8_t)ob_handle.RDPLevel;
#else

	 volatile uint32_t *pOB_addr = (uint32_t*) 0x1FFFC000;
	 rdp_status =  (uint8_t)(*pOB_addr >> 8) ;
#endif

	return rdp_status;

}

//verify the address sent by the host .
uint8_t verify_address(uint32_t go_address)
{
	//so, what are the valid addresses to which we can jump ?
	//can we jump to system memory ? yes
	//can we jump to sram1 memory ?  yes
	//can we jump to sram2 memory ? yes
	//can we jump to backup sram memory ? yes
	//can we jump to peripheral memory ? its possible , but dont allow. so no
	//can we jump to external memory ? yes.

//incomplete -poorly written .. optimize it
	if ( go_address >= SRAM1_BASE && go_address <= SRAM1_END)
	{
		return ADDR_VALID;
	}
	else if ( go_address >= SRAM2_BASE && go_address <= SRAM2_END)
	{
		return ADDR_VALID;
	}
	else if ( go_address >= FLASH_BASE && go_address <= FLASH_END)
	{
		return ADDR_VALID;
	}
	else if ( go_address >= BKPSRAM_BASE && go_address <= BKPSRAM_END)
	{
		return ADDR_VALID;
	}
	else
		return ADDR_INVALID;
}

 uint8_t execute_flash_erase(uint8_t sector_number , uint8_t number_of_sector)
{
    //we have totally 8 sectors in STM32F446RE mcu .. sector[0 to 7]
	//number_of_sector has to be in the range of 0 to 7
	// if sector_number = 0xff , that means mass erase !
	//Code needs to modified if your MCU supports more flash sectors
	FLASH_EraseInitTypeDef flashErase_handle;
	uint32_t sectorError;
	HAL_StatusTypeDef status;


	if( number_of_sector > 8 )
		return INVALID_SECTOR;

	if( (sector_number == 0xff ) || (sector_number <= 7) )
	{
		if(sector_number == (uint8_t) 0xff)
		{
			flashErase_handle.TypeErase = FLASH_TYPEERASE_MASSERASE;
		}else
		{
		    /*Here we are just calculating how many sectors needs to erased */
			uint8_t remanining_sector = 8 - sector_number;
            if( number_of_sector > remanining_sector)
            {
            	number_of_sector = remanining_sector;
            }
			flashErase_handle.TypeErase = FLASH_TYPEERASE_SECTORS;
			flashErase_handle.Sector = sector_number; // this is the initial sector
			flashErase_handle.NbSectors = number_of_sector;
		}
		flashErase_handle.Banks = FLASH_BANK_1;

		/*Get access to touch the flash registers */
		HAL_FLASH_Unlock();
		flashErase_handle.VoltageRange = FLASH_VOLTAGE_RANGE_3;  // our mcu will work on this voltage range
		status = (uint8_t) HAL_FLASHEx_Erase(&flashErase_handle, &sectorError);
		HAL_FLASH_Lock();

		return status;
	}


	return INVALID_SECTOR;
}

/*This function writes the contents of pBuffer to  "mem_address" byte by byte */
//Note1 : Currently this function supports writing to Flash only .
//Note2 : This functions does not check whether "mem_address" is a valid address of the flash range.
uint8_t execute_mem_write(uint8_t *pBuffer, uint32_t mem_address, uint32_t len)
{
    uint8_t status=HAL_OK;

    //We have to unlock flash module to get control of registers
    HAL_FLASH_Unlock();

    for(uint32_t i = 0 ; i <len ; i++)
    {
        //Here we program the flash byte by byte
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,mem_address+i,pBuffer[i] );
    }

    HAL_FLASH_Lock();

    return status;
}


/*
Modifying user option bytes
To modify the user option value, follow the sequence below:
1. Check that no Flash memory operation is ongoing by checking the BSY bit in the
FLASH_SR register
2. Write the desired option value in the FLASH_OPTCR register.
3. Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
4. Wait for the BSY bit to be cleared.
*/
uint8_t configure_flash_sector_rw_protection(uint8_t sector_details, uint8_t protection_mode, uint8_t disable)
{
    //First configure the protection mode
    //protection_mode =1 , means write protect of the user flash sectors
    //protection_mode =2, means read/write protect of the user flash sectors
    //According to RM of stm32f446xx TABLE 9, We have to modify the address 0x1FFF C008 bit 15(SPRMOD)

	 //Flash option control register (OPTCR)
    volatile uint32_t *pOPTCR = (uint32_t*) 0x40023C14;

	  if(disable)
		{

			//disable all r/w protection on sectors

			//Option byte configuration unlock
			HAL_FLASH_OB_Unlock();

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			//clear the 31st bit (default state)
			//please refer : Flash option control register (FLASH_OPTCR) in RM
			*pOPTCR &= ~(1 << 31);

			//clear the protection : make all bits belonging to sectors as 1
			*pOPTCR |= (0xFF << 16);

			//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
			*pOPTCR |= ( 1 << 1);

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			HAL_FLASH_OB_Lock();

			return 0;

		}

	   if(protection_mode == (uint8_t) 1)
    {
           //we are putting write protection on the sectors encoded in sector_details argument

			//Option byte configuration unlock
			HAL_FLASH_OB_Unlock();

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			//here we are setting just write protection for the sectors
			//clear the 31st bit
			//please refer : Flash option control register (FLASH_OPTCR) in RM
			*pOPTCR &= ~(1 << 31);

			//put write protection on sectors
			*pOPTCR &= ~ (sector_details << 16);

			//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
			*pOPTCR |= ( 1 << 1);

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			HAL_FLASH_OB_Lock();
		}

		else if (protection_mode == (uint8_t) 2)
    {
	  	//Option byte configuration unlock
			HAL_FLASH_OB_Unlock();

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			//here wer are setting read and write protection for the sectors
			//set the 31st bit
			//please refer : Flash option control register (FLASH_OPTCR) in RM
			*pOPTCR |= (1 << 31);

			//put read and write protection on sectors
            *pOPTCR &= ~(0xff << 16);
			*pOPTCR |= (sector_details << 16);

			//Set the option start bit (OPTSTRT) in the FLASH_OPTCR register
			*pOPTCR |= ( 1 << 1);

			//wait till no active operation on flash
			while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

			HAL_FLASH_OB_Lock();
    }

		return 0;
}

uint16_t read_OB_rw_protection_status(void)
{
    //This structure is given by ST Flash driver to hold the OB(Option Byte) contents .
	FLASH_OBProgramInitTypeDef OBInit;

	//First unlock the OB(Option Byte) memory access
	HAL_FLASH_OB_Unlock();
	//get the OB configuration details
	HAL_FLASHEx_OBGetConfig(&OBInit);
	//Lock back .
	HAL_FLASH_Lock();
	return (uint16_t)OBInit.WRPSector;

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
