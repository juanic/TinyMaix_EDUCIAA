/*
 * cyclesCounter.h
 *
 *  Created on: 26 ene. 2022
 *      Author: Usuario
 */

#ifndef MODULES_LPC4337_M4_DRIVERS_MICROCONTROLLER_INC_CYCLESCOUNTER_H_
#define MODULES_LPC4337_M4_DRIVERS_MICROCONTROLLER_INC_CYCLESCOUNTER_H_

/*==================[inclusions]=============================================*/

#include "stdint.h"
#include "lpc_types.h"
/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

// TODO: Check CPU core speed using SystemCoreClock
#define EDU_CIAA_NXP_CLOCK_SPEED	204000000

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[ISR external functions definition]======================*/

/*==================[external functions definition]==========================*/

bool cyclesCounterConfig( uint32_t clockSpeed );

uint32_t cyclesCounterRead( void );

void cyclesCounterReset( void );

float cyclesCounterToUs( uint32_t cycles );

float cyclesCounterToMs( uint32_t cycles );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

#endif /* MODULES_LPC4337_M4_DRIVERS_MICROCONTROLLER_INC_CYCLESCOUNTER_H_ */
