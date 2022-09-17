/*
 * VNH3SP30.h
 *
 *  Created on: 21 oct. 2021
 *      Author: Usuario
 */

#ifndef MODULES_LPC4337_M4_DRIVERS_DEVICES_INC_VNH3SP30_H_
#define MODULES_LPC4337_M4_DRIVERS_DEVICES_INC_VNH3SP30_H_

#include "stdint.h"

#define CH0 1  /*Board connector: DAC pin*/
#define CH1 2  /*Board connector: CH1 pin*/
#define CH2 4  /*Board connector: CH2 pin*/
#define CH3 8  /*Board connector: CH3 pin*/

void VNH3SP30_begin(pwm_out_t * pwmPin, gpio_t inaPin, gpio_t inbPin, gpio_t diagPin, int8_t csPin);
uint8_t VNH3SP30_setSpeed(int speed);
uint8_t VNH3SP30_brake(int brakePower);
uint8_t VNH3SP30_status();
uint16_t VNH3SP30_motorCurrent();

#endif /* MODULES_LPC4337_M4_DRIVERS_DEVICES_INC_VNH3SP30_H_ */
