#ifndef signalgateway_h
#define signalgateway_h

#include <avr/io.h>

extern void GW_Write_LedDispayValue(uint16_t src);
extern void GW_Read_LedDispayValue(uint16_t * dst);

extern void GW_Write_TCK_CurrentSpeed(uint8_t src);
extern void GW_Read_TCK_CurrentSpeed(uint8_t * dst);

extern void GW_Write_ADC_ChannelValue(uint16_t src, uint8_t idx);
extern void GW_Read_ADC_ChannelValue(uint16_t * dst, uint8_t idx);

extern uint8_t* GW_Get_LCD_String(void);

typedef struct ETL_ErrorLog 
{
    uint8_t object;
    uint8_t error;
    uint8_t data;
} ts_ETL_ErrorLog;

extern void GW_Push_ETL_errorBuffer(uint8_t object, uint8_t error, uint8_t data);
extern ts_ETL_ErrorLog* GW_Get_ETL_errorBuffer(void);
uint8_t GW_Get_ETL_errorBufferPointer(void);
uint8_t GW_Clear_ETL_errorBufferPointer(void);

#endif