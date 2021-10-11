#include "motorcontroller.h"

#include "../pwm/pwm.h"
#include "../motorencoder/motorencoder.h"
#include "../pid/pid.h"
#include "../adc/adc.h"
#include "../motorcontroller/motorcontroller.h"
#include "../signalgateway/signalgateway.h"
#include "../stringmanager/stringmanager.h"

#define PID_60MS 60000
#define PID_ONE_DIGIT_AFTER_COMMA 10
#define PID_HALFPULSES_PER_REVOLUTION 24

#define PID_HALFPULSE_PERIOD_TO_RPM(period) ((PID_60MS * PID_ONE_DIGIT_AFTER_COMMA) / ((period) * PID_HALFPULSES_PER_REVOLUTION))
#define PID_CRUTCH_FOR_ACTUAL_RPM(rpm) (((int32_t)(rpm) * 87) / 100)

uint16_t MCR_setPint = 0u;
uint32_t MEC_pulsePeriod = 0u;
uint32_t MEC_actualRPM = 0u;
int32_t MEC_fixedActualRPM = 0u;


void MCR_Run(void)
{
    uint8_t pulseWidth = 0u;

    GW_Read_ADC_ChannelValue(&MCR_setPint, ADC_POT);
    MEC_pulsePeriod = MEC_GetPulsePeriod();
    MEC_actualRPM = PID_HALFPULSE_PERIOD_TO_RPM(MEC_pulsePeriod);
    MEC_fixedActualRPM = PID_CRUTCH_FOR_ACTUAL_RPM(MEC_actualRPM);
    pulseWidth = PID_Calculate(MCR_setPint, MEC_fixedActualRPM);

    PWM_SetPulseWidth(pulseWidth);
}

void MCR_Show(void)
{
    STR_WriteStringToLCD(LCD_LINE_1, 0, 10, "Setpoint: ");
    STR_WriteNumberToLCD(LCD_LINE_1, 10, 4, STR_ALIGNMENT_RIGHT, STR_FILLING_NONE, MCR_setPint);
    STR_WriteStringToLCD(LCD_LINE_2, 0, 8, "Actual: ");
    STR_WriteNumberToLCD(LCD_LINE_2, 10, 4, STR_ALIGNMENT_RIGHT, STR_FILLING_NONE, MEC_actualRPM);
}