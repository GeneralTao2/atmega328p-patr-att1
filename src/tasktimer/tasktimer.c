#include "tasktimer.h"

#include "../defines.h"
/*
 * \def: PRESCALER
 * \brief: Frequency prescaler, to be used for timer
 */
#define PRESCALER 64

/*
 * \def: COMPARE_FREQ
 * \brief: The frequency of interrupt actuation, in mHz
 */
#define COMPARE_FREQ 1000

/*
 * \def: OCR_1_MS
 * \brief: The Output Compare Register value calculation,
 *      required for achive interrupt actuation with frequency 
 *      COMPARE_FREQ
 */
#define OCR_1_MS ( (F_CPU / PRESCALER) / COMPARE_FREQ )

/*
 * \def: PERIOD_1MS_PRESCALER
 * \brief: The number of interrupt actuations, needed for 
 *      trigger event with 1 ms period
 */
#define PERIOD_1MS_PRESCALER 1

/*
 * \def: PERIOD_5MS_PRESCALER
 * \brief: The number of 1 ms event triggerings, needed for 
 *      trigger event with 5 ms period
 */
#define PERIOD_5MS_PRESCALER 5

/*
 * \def: PERIOD_10MS_PRESCALER
 * \brief: The number of 1 ms event triggerings, needed for 
 *      trigger event with 10 ms period
 */
#define PERIOD_10MS_PRESCALER 10

/*
 * \def: PERIOD_100MS_PRESCALER
 * \brief: The number of 10 ms event triggerings, needed for 
 *      trigger event with 100 ms period
 */
#define PERIOD_100MS_PRESCALER 10

/*
 * \def: PERIOD_100MS_PRESCALER
 * \brief: The number of 100 ms event triggerings, needed for 
 *      trigger event with 1000 ms period
 */
#define PERIOD_1000MS_PRESCALER 10

/*
 * \def: uint8_t period1msCntr
 * \brief: The counter, that counts the number of interrupt actuations
 *      up to PERIOD_1MS_PRESCALER
 */
uint8_t period1msCntr =     0u;

/*
 * \def: uint8_t period5msCntr
 * \brief: The counter, that counts the number of 1 ms event triggerings
 *      up to PERIOD_5MS_PRESCALER
 */
uint8_t period5msCntr =     0u;

/*
 * \def: uint8_t period10msCntr
 * \brief: The counter, that counts the number of 1 ms event triggerings
 *      up to PERIOD_10MS_PRESCALER
 */
uint8_t period10msCntr =    0u;

/*
 * \def: uint8_t period100msCntr
 * \brief: The counter, that counts the number of 10 ms event triggerings
 *      up to PERIOD_100MS_PRESCALER
 */
uint8_t period100msCntr =   0u;

/*
 * \def: uint8_t period1000msCntr
 * \brief: The counter, that counts the number of 100 ms event triggerings
 *      up to PERIOD_1000MS_PRESCALER 
 */
uint8_t period1000msCntr =  0;

/*
 * \def: uint8_t TT_Event1ms
 * \brief: The flag, that shows 1 ms period event state. Can be EVENT_WAIT or EVENT_ARRIVE
 */
uint8_t TT_Event1ms =     EVENT_WAIT;

/*
 * \def: uint8_t TT_Event5ms
 * \brief: The flag, that shows 5 ms period event state. Can be EVENT_WAIT or EVENT_ARRIVE
 */
uint8_t TT_Event5ms =     EVENT_WAIT;

/*
 * \def: uint8_t TT_Event10ms
 * \brief: The flag, that shows 10 ms period event state. Can be EVENT_WAIT or EVENT_ARRIVE
 */
uint8_t TT_Event10ms =    EVENT_WAIT;

/*
 * \def: uint8_t TT_Event100ms
 * \brief: The flag, that shows 100 ms period event state. Can be EVENT_WAIT or EVENT_ARRIVE
 */
uint8_t TT_Event100ms =   EVENT_WAIT;

/*
 * \def: uint8_t TT_Event1000ms
 * \brief: The flag, that shows 1000 ms period event state. Can be EVENT_WAIT or EVENT_ARRIVE
 */
uint8_t TT_Event1000ms =  EVENT_WAIT;

/**
 * void TT_Init(void) 
 * \brief: 
 * 		Initializes timer		 
 * \description: 
 * 		This function Initializes task timer, 8-bit Timer/Counter0
 * \return value:
 * 		No return value
 */

void TT_Init(void) 
{
#ifdef ATMEGA32
    /* Clear all registers to be used */
    TCCR0 = 0u;
    OCR0 = 0u;
    TIMSK = 0u;
    TCNT0 = 0u;
    OCR0 = 0u;
    TIFR = 0u;

    /* Set Timer/Counter0 CTC mode of operation */
    SET_BIT(TCCR0, WGM01);
    CLR_BIT(TCCR0, WGM00);

    /* Set Timer/Counter0 normal port operation, OC0 disconnected */
    CLR_BIT(TCCR0, COM01);
    CLR_BIT(TCCR0, COM00);

    /* Set Timer/Counter0 Output Compare Match Interrupt Enable */
    SET_BIT(TIMSK, OCIE0);
    SET_BIT(TIMSK, TOIE0);

    /* Write OCR_1_ms to OCR0, representing 1 ms compare preriod */
    OCR0 = OCR_1_MS;

    /* Set Timer/Counter0 clock prescaler */
    TCCR0 |= (0 << CS02) | (1 << CS01) | (1 << CS00);
#endif
#ifdef ATMEGA328P
    /* Clear all registers to be used */
    TCCR0A = 0u;
    OCR0A = 0u;
    TIMSK0 = 0u;
    TCNT0 = 0u;
    TIFR0 = 0u;

    /* Set Timer/Counter0 CTC mode of operation */
    CLR_BIT(TCCR0A, WGM02);
    SET_BIT(TCCR0A, WGM01);
    CLR_BIT(TCCR0A, WGM00);

    /* Set Timer/Counter0 normal port operation, OC0 disconnected */
    CLR_BIT(TCCR0A, COM0A1);
    CLR_BIT(TCCR0A, COM0A0);

    /* Set Timer/Counter0 Output Compare Match Interrupt Enable */
    SET_BIT(TIMSK0, OCIE0A);
    SET_BIT(TIMSK0, TOIE0);

    /* Write OCR_1_ms to OCR0, representing 1 ms compare preriod */
    OCR0A = OCR_1_MS;

    /* Set Timer/Counter0 clock prescaler */
    TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
#endif
}

/*
 * \def: ISR(TIMER0_COMP_vect) 
 * \brief: Interrupt function, what actuates on Timer/Counter0 compare match
 */
#ifdef ATMEGA32
ISR(TIMER0_COMP_vect) 
#endif
#ifdef ATMEGA328P
ISR(TIMER0_COMPA_vect) 
#endif
{
    period1msCntr++;
    if (period1msCntr >= PERIOD_1MS_PRESCALER)
	{
		TT_Event1ms = EVENT_ARRIVE;
        period5msCntr++;
        period10msCntr++;
        period1msCntr = 0;
	}

    if (period5msCntr >= PERIOD_5MS_PRESCALER)
	{
		TT_Event5ms = EVENT_ARRIVE;
        period5msCntr = 0;
	}

	if (period10msCntr >= PERIOD_10MS_PRESCALER)
	{
		TT_Event10ms = EVENT_ARRIVE;
        period100msCntr++;
        period10msCntr = 0;
	}

	if (period100msCntr >= PERIOD_100MS_PRESCALER)
	{
		TT_Event100ms = EVENT_ARRIVE;
        period1000msCntr++;
        period100msCntr = 0;
	}

    if (period1000msCntr >= PERIOD_1000MS_PRESCALER)
	{
		TT_Event1000ms = EVENT_ARRIVE;
        period1000msCntr = 0;
	}
}