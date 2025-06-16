/*
 * keypad.c
 *
 *  Created on: Jun 16, 2025
 *      Author: kjeyabalan
 */


#include "keypad.h"

const char keymap[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

GPIO_TypeDef* rowPorts[4] = {KEYPAD_R1_GPIO_Port, KEYPAD_R2_GPIO_Port, KEYPAD_R3_GPIO_Port, KEYPAD_R4_GPIO_Port};
uint16_t rowPins[4]      = {KEYPAD_R1_Pin, KEYPAD_R2_Pin, KEYPAD_R3_Pin, KEYPAD_R4_Pin};

GPIO_TypeDef* colPorts[4] = {KEYPAD_C1_GPIO_Port, KEYPAD_C2_GPIO_Port, KEYPAD_C3_GPIO_Port, KEYPAD_C4_GPIO_Port};
uint16_t colPins[4]       = {KEYPAD_C1_Pin, KEYPAD_C2_Pin, KEYPAD_C3_Pin, KEYPAD_C4_Pin};

char scan_keypad(void) {
    for (int row = 0; row < 4; row++) {
        // Set all rows HIGH
        for (int i = 0; i < 4; i++) {
            HAL_GPIO_WritePin(rowPorts[i], rowPins[i], GPIO_PIN_SET);
        }

        // Set current row LOW
        HAL_GPIO_WritePin(rowPorts[row], rowPins[row], GPIO_PIN_RESET);

        // Check columns
        for (int col = 0; col < 4; col++) {
            if (HAL_GPIO_ReadPin(colPorts[col], colPins[col]) == GPIO_PIN_RESET) {
                HAL_Delay(20);  // Debounce
                while (HAL_GPIO_ReadPin(colPorts[col], colPins[col]) == GPIO_PIN_RESET);
                return keymap[row][col];
            }
        }
    }
    return '\0'; // No key pressed
}
