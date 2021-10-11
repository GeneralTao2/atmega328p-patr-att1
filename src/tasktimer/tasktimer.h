#ifndef tasktimer_h
#define tasktimer_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../utils/utils.h"

/*
 * \def: EVENT_WAIT
 * \brief: Shows that a certain timer event hasn't happened yet
 */
#define EVENT_WAIT 0

/*
 * \def: EVENT_ARRIVE
 * \brief: Shows that a certain timer event already happened
 */
#define EVENT_ARRIVE 1

extern uint8_t TT_Event1ms;
extern uint8_t TT_Event5ms;
extern uint8_t TT_Event10ms;
extern uint8_t TT_Event100ms;
extern uint8_t TT_Event1000ms;

extern void TT_Init(void);

#endif