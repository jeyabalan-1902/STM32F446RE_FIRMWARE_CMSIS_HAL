/*
 * tim.h
 *
 *  Created on: May 3, 2025
 *      Author: kjeyabalan
 */

#ifndef TIM_H_
#define TIM_H_

void tim2_1Hz_init(void);
void tim2_PA5_output_compare(void);
void tim3_input_capture(void);

#define SR_CC1IF (1U << 1)
#define SR_UIF   (1U << 0)
#endif /* TIM_H_ */
