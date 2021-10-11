/*
debounce.h. Snigelens version of Peter Dannegger’s debounce routines.
Debounce up to eight buttons on one port. $Rev: 577 $
*/
#ifndef DEBOUNCE_H
#define DEBOUNCE_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "defines.h"

/* Decrease 2 bit vertical counter where mask = 1. */
/* Set counters to binary 11 where mask = 0. */
#define VC_DEC_OR_SET(vcounter, mask) \
    vcounter.lowBit = ~(vcounter.lowBit & mask); \
    vcounter.highBit = vcounter.lowBit ^ (vcounter.highBit & mask)

typedef struct DBC_VerticalCounter {
    uint8_t lowBit;
    uint8_t highBit;
} ts_DBC_VerticalCounter;

typedef struct DBC_PortConfigs {
    uint8_t enablingState;
    /* Variable to tell that the button is pressed (and debounced). */
    uint8_t buttonsDown;
    /* Eight vertical two bit counters for number of equal states */
    ts_DBC_VerticalCounter vcounter;
    /* Keeps track of current (debounced) state */
    uint8_t button_state;

} ts_DBC_PortConfigs;

/* Return non-zero if a button matching mask is pressed. */
uint8_t DBC_ReadDebouncedPin(uint8_t port, uint8_t pin);

/* Activate specific port to be debounced. */
void DBC_InitPortDebouncing(uint8_t port);

/* Call this function every 10 ms or so. */
void DBC_Debounce(void);

#endif