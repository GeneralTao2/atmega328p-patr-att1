#include "signalgateway.h"
#include "../defines.h"

/* Start LED display value */
uint16_t LD_ledDisplayValue = 9876;

int8_t TCK_currentSpeed = 0;

uint16_t ADC_ChannelValues[ADC_CHANNEL_QUANTITY] = {0};

uint8_t LCD_currentCharacters[LCD_CURRENT_CHARACTERS_QUANTITY] = {0};

uint8_t ETL_errorBufferPointer = 0u;
ts_ETL_ErrorLog ETL_errorBuffer[ETL_ERROR_BUFFER_LENGTH];

void GW_Write_LedDispayValue(uint16_t src) {
    LD_ledDisplayValue = src;
}
void GW_Read_LedDispayValue(uint16_t * dst) {
    *dst = LD_ledDisplayValue;
}


void GW_Write_TCK_CurrentSpeed(uint8_t src) {
    TCK_currentSpeed = src;
}
void GW_Read_TCK_CurrentSpeed(uint8_t * dst) {
    *dst = TCK_currentSpeed;
}


void GW_Write_ADC_ChannelValue(uint16_t src, uint8_t idx) {
    ADC_ChannelValues[idx] = src;
}
void GW_Read_ADC_ChannelValue(uint16_t * dst, uint8_t idx) {
    *dst = ADC_ChannelValues[idx];
}


uint8_t* GW_Get_LCD_String(void) 
{
    return LCD_currentCharacters;
}


void GW_Push_ETL_errorBuffer(uint8_t object, uint8_t error, uint8_t data)
{
    if(ETL_errorBufferPointer < ETL_ERROR_BUFFER_LENGTH )
    {
        ETL_errorBuffer[ETL_errorBufferPointer].object = object;
        ETL_errorBuffer[ETL_errorBufferPointer].error = error;
        ETL_errorBuffer[ETL_errorBufferPointer].data = data;
        ETL_errorBufferPointer++;
    }
}

ts_ETL_ErrorLog* GW_Get_ETL_errorBuffer(void)
{
    return ETL_errorBuffer;
}

uint8_t GW_Get_ETL_errorBufferPointer(void)
{
    return ETL_errorBufferPointer;
}

uint8_t GW_Clear_ETL_errorBufferPointer(void)
{
    return ETL_errorBufferPointer = 0u;
}