/*
 * ota_update.h
 *
 *  Created on: June 10, 2025
 *      Author: kjeyabalan
 */

#ifndef INC_OTA_UPDATE_H_
#define INC_OTA_UPDATE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "main.h"

#define BL_GET_VER            0x51
#define BL_GET_HELP           0x52
#define BL_GET_CID            0x53
#define BL_GET_RDP_STATUS     0x54
#define BL_GO_TO_ADDR         0x55
#define BL_FLASH_ERASE        0x56
#define BL_MEM_WRITE          0x57
#define BL_EN_RW_PROTECT      0x58
#define BL_MEM_READ           0x59
#define BL_READ_PAGE_STATUS   0x5A  // Changed from sector to page
#define BL_OTP_READ           0x5B
#define BL_DIS_R_W_PROTECT    0x5C

#define BL_RX_LEN             200

#define BL_ACK                0xA5
#define BL_NACK               0x7F

#define VERIFY_CRC_FAIL       1
#define VERIFY_CRC_SUCCESS    0

#define BL_VERSION            0x10   //version 1.0

// STM32F103C8T6 Flash Memory Layout
#define FLASH_PAGE_SIZE       0x400U   // 1KB per page
#define FLASH_PAGES_COUNT     64       // 64 pages total (64KB)
#define FLASH_SIZE            (64*1024) // 64KB total flash

// STM32F103C8T6 Memory Boundaries
#define FLASH_BASE            0x08000000U
#define FLASH_END             (FLASH_BASE + FLASH_SIZE - 1)
#define SRAM_BASE             0x20000000U
#define SRAM_SIZE             (20*1024)  // 20KB SRAM
#define SRAM_END              (SRAM_BASE + SRAM_SIZE - 1)

#define ADDR_VALID            0x00
#define ADDR_INVALID          0x01

#define INVALID_PAGE          0x04

#define BOOT_CMD              0x50

#define UART_TIMEOUT_MS       3000

// Application start address (after bootloader)
#define APP_START_PAGE        8        // Start from page 8 (8KB reserved for bootloader)
#define APP_START_ADDRESS     (FLASH_BASE + (APP_START_PAGE * FLASH_PAGE_SIZE))

extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

void bootloader_jump_to_application(void);
void bootloader_read_uart_data(void);
void bootloader_handle_read_otp(uint8_t *bl_rx_buffer);
void bootloader_handle_read_page_status(uint8_t *bl_rx_buffer); // Changed from sector to page
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
uint8_t execute_flash_erase(uint8_t page_number, uint8_t number_of_pages); // Changed from sector to page
uint8_t execute_mem_write(uint8_t *pBuffer, uint32_t mem_address, uint32_t len);

uint8_t configure_flash_page_rw_protection(uint16_t page_mask, uint8_t protection_mode, uint8_t disable); // Changed for pages

uint16_t read_OB_rw_protection_status(void);

void bootloader_jump_to_user_app(uint32_t app_start_address);
void jump_to_updated_application(uint32_t app_start_address);

#endif /* INC_OTA_UPDATE_H_ */
