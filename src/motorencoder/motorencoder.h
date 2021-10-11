#ifndef motorencoder_h
#define motorencoder_h

#include <avr/io.h>
#include "../utils/utils.h"
#include "../defines.h"

void MEC_Init(void);
void MEC_CountPulsePeriod(void);
uint32_t MEC_GetPulsePeriod(void);

#endif