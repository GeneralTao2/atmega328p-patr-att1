#include "lm50.h"

#include "../adc/adc.h"
#include "../signalgateway/signalgateway.h"
#include "../stringmanager/stringmanager.h"
int8_t LM50_GetTemperature(void)
{
    uint16_t lm50_ADC_output = 0u;
    int16_t temperature = 0;
    GW_Read_ADC_ChannelValue(&lm50_ADC_output, ADC_LM50);
    temperature = U_Map(lm50_ADC_output, LM50_MIN_ADC, LM50_MAX_ADC, LM50_MIN_TEMP, LM50_MAX_TEMP);
    return temperature;
}