#ifndef adc_h
#define adc_h

#include <avr/io.h>
#include "../dio/dio.h"
#include "../utils/utils.h"

#define ADC0_MUX 0
#define ADC1_MUX 1
#define ADC2_MUX 2
#define ADC3_MUX 3
#define ADC4_MUX 4
#define ADC5_MUX 5
#define ADC6_MUX 6
#define ADC7_MUX 7

#define ADC_REF_EXT 0
#define ADC_REF_INT 1

#define ADC_OFF 0
#define ADC_ON 1

#define ADC_REF_INT_VOLT 2560
#define ADC_ADC_MAX_VALUE 1024
#define ADC_MILLIVOLTSIN_IN_1VOLT 1000
//#define ADC_ADC_TO_VOLTS(refVolt, adcValue) ( ( ( (adcValue)*(refVolt) ) / ADC_ADC_MAX_VALUE ) / ADC_MILLIVOLTSIN_IN_1VOLT )
#define ADC_ADC_TO_VOLTS(refVolt, adcValue) ( (uint16_t)( ( (uint32_t)(adcValue)*(refVolt) ) / ADC_ADC_MAX_VALUE ) )

#define ADC_VOLTS_TO_ADC(millivolts) ( ( ((uint32_t)ADC_ADC_MAX_VALUE * (millivolts)) / ADC_MILLIVOLTSIN_IN_1VOLT ) / 5 )

typedef enum {
    ADC_POT,
    ADC_LM50,
    ADC_QUANTITY,
} te_ADC_Pins;

extern void ADC_PreInit(void);
extern void ADC_ConfigurePin(const te_ADC_Pins adc, const uint8_t mux, const uint8_t ref, uint8_t const state);
extern void ADC_PostInit(void);
extern void ADC_NextChannel(void);

extern uint8_t ADC_MuxFromConfig(const te_ADC_Pins adc);
extern uint8_t ADC_RefFromConfig(const te_ADC_Pins adc);
extern uint8_t ADC_StateFromConfig(const te_ADC_Pins adc);
extern void ADC_StateToConfig(const te_ADC_Pins adc, const uint8_t state);
 
/* ===================================== */

#define ADC_REF_EXT_VOLT 5000

extern void ADC_Init(void);
extern void ADC_Run(void);
extern uint16_t ADC_ReadVolts(const te_ADC_Pins adc);
extern void ADC_Show(void);

#endif