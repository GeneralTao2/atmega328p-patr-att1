#include "motorencoder.h"
#include "../dio/dio.h"

uint8_t MEC_pinPreviusState;
uint32_t MEC_pulsePeriodCounetr;
uint32_t MEC_pulsePeriod;
uint8_t MEC_firstCalculationMade = D_FALSE;

void MEC_Init(void)
{
    DIO_ConfigurePin(MEC_PIN, CP_B, CP_0, CP_R, CP_ON, CP_RD);
    MEC_pinPreviusState = DIO_ReadPin(MEC_PIN);
    MEC_pulsePeriod = 0;
    MEC_pulsePeriodCounetr = 0u;
}

void MEC_CountPulsePeriod(void)
{
    uint8_t newPinState = CP_PASS;

    MEC_pulsePeriodCounetr++;
    newPinState = DIO_ReadPin(MEC_PIN);
    if(newPinState != MEC_pinPreviusState) 
    {
        if(MEC_firstCalculationMade == D_TRUE) 
        {
            MEC_pulsePeriod = MEC_pulsePeriodCounetr;
        } else
        {
            MEC_firstCalculationMade = D_TRUE;
        }
        MEC_pulsePeriodCounetr = 0u;
    }
    MEC_pinPreviusState = newPinState;
}

uint32_t MEC_GetPulsePeriod(void) 
{
    return MEC_pulsePeriod;
}