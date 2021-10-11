#include "termocontrol.h"

#include "../lm50/lm50.h"
#include "../dio/dio.h"

#define TCR_MAX_TEMPERATURE 10
#define TCR_MIN_TEMPERATURE -10

void TCR_Init(void)
{
    DIO_ConfigurePin(TCR_PIN, CP_D, CP_0, CP_R, CP_OFF, CP_WR);
}

void TCR_Run(void)
{
    int8_t currentTemperature = 0;

    currentTemperature = LM50_GetTemperature();

    if(currentTemperature < TCR_MIN_TEMPERATURE) 
    {
        DIO_PinOn(TCR_PIN);
    } else
    if(currentTemperature > TCR_MAX_TEMPERATURE)
    {
        DIO_PinOff(TCR_PIN);
    }
}