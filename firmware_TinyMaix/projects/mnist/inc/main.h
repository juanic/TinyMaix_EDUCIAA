/*! @mainpage Aplicación Principal
 *
 * \section genDesc General Description
 *
 * This section describes how the program works.
 *
 *
 * \section hardConn Hardware Connection
 *
 * |   ADXL35		|   EDU-CIAA	|
 * |:--------------:|:--------------|
 * | 	GND 	 	| 	GNDA		|
 * | 	ZOUT	 	| 	CH3 		|
 * | 	YOUT	 	| 	CH2 		|
 * | 	XOUT	 	| 	CH1			|
 * | 	VCC 	 	| 	VDDA		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 10/06/2022 | Document creation		                         |

 *
 * @author JIC
 *
 */

#ifndef _BLINKING_H
#define _BLINKING_H


/*==================[inclusions]=============================================*/

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"
int main(void);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[external functions declaration]=========================*/
/** @fn void Switch2()
 * @brief Función para detección de movimiento
 * @param[in] ax Valores de aceleración en X
 * @param[in] ay Valores de aceleración en Y
 * @param[in] az Valores de aceleración en Z
 * @return True o False
 */
bool motionDetected(float ax, float ay, float az);

/*==================[end of file]============================================*/


#endif /* #ifndef _BLINKING_H */

