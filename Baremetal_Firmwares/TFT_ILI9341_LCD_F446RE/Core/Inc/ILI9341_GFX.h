/*
 * ILI9341_GFX.h
 *
 *  Created on: Jun 4, 2025
 *      Author: kjeyabalan
 */

#ifndef INC_ILI9341_GFX_H_
#define INC_ILI9341_GFX_H_

#include "stm32f4xx_hal.h"
#include "fonts.h"

#define HORIZONTAL_IMAGE	0
#define VERTICAL_IMAGE		1

void ILI9341_DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
void ILI9341_DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
void ILI9341_DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
void ILI9341_DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
void ILI9341_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawImage(const uint8_t* image, uint8_t orientation);
void ILI9341_DrawSmileEmoji(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t faceColor, uint16_t eyeColor, uint16_t mouthColor);
void ILI9341_DrawNeutralMouth(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t color);
void ILI9341_DrawSmileEmoji_StaticParts(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t faceColor, uint16_t eyeColor);
void ILI9341_DrawSmileMouth(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t color);
#endif /* INC_ILI9341_GFX_H_ */
