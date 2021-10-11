#ifndef lm50_h
#define lm50_h

#include <avr/io.h>
#include "../utils/utils.h"
#include "../defines.h"


#define LM50_MAX_TEMP 125
#define LM50_MIN_TEMP -40

#define LM50_MIN_VOLTAGE 100 /* /1000 */
#define LM50_MAX_VOLTAGE 1750 /* /1000 */

#define LM50_MIN_ADC ADC_VOLTS_TO_ADC(LM50_MIN_VOLTAGE)
#define LM50_MAX_ADC ADC_VOLTS_TO_ADC(LM50_MAX_VOLTAGE)

int8_t LM50_GetTemperature(void);

#endif