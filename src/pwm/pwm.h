#ifndef pwm_h
#define pwm_h

#include <avr/io.h>
#include "../utils/utils.h"
#include "../defines.h"

void PWM_Init(void);
void PWM_SetPulseWidth(uint8_t width);

#endif