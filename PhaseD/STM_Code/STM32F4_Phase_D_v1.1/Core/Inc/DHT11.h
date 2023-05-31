/*
 * DHT11.h
 *
 *  Created on: May 29, 2023
 *      Author: tobyw
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f4xx_hal.h"

uint8_t DHT11_Start (void);
uint8_t DHT11_Read (void);
void microDelay (uint16_t delay);

#endif /* INC_DHT11_H_ */
