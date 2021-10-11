#ifndef pid_h
#define pid_h

#include <avr/io.h>
#include "../utils/utils.h"
#include "../defines.h"

uint8_t PID_Calculate(int32_t setPintRPM, int32_t actualRPM);
void PID_Show(void);

#endif