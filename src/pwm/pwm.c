#include "pwm.h"

void PWM_Init(void) 
{
    DDRB |= 1 << PB1;

    /* Clear all registers to be used */
    TCCR1A = 0u;
    OCR1A = 0u;
    TIMSK1 = 0u;
    TCNT1 = 0u;
    TIFR1 = 0u;

    /* Set Timer/Counter0 Fast PWM, 8-bit mode of operation */
    CLR_BIT(TCCR1B, WGM13);
    SET_BIT(TCCR1B, WGM12);
    CLR_BIT(TCCR1A, WGM11);
    SET_BIT(TCCR1A, WGM10);

    /* Clear OC1A/OC1B on Compare Match when 
       up-counting. Set OC1A/OC1B on Compare Match when downcounting. */
    SET_BIT(TCCR1A, COM1A1);
    CLR_BIT(TCCR1A, COM1A0);


    /* Set Timer/Counter0 clock prescaler to 1024 so we get around 60Hz PWM */
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
}

void PWM_SetPulseWidth(uint8_t width) 
{
    OCR1A = width;
}