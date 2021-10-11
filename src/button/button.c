#include "button.h"
#include "../signalgateway/signalgateway.h"
#include "../defines.h"

/* Led Display */
#define BTN_INCR_LED_DISPLAY_VALUE BUTTON_0
#define BTN_DECR_LED_DISPLAY_VALUE BUTTON_1
#define BTN_RESET_LED_DISPLAY_VALUE BUTTON_2

/* ticker */
#define BTN_TCK_SPEED_1_FRWD BUTTON_3
#define BTN_TCK_SPEED_2_FRWD BUTTON_4
#define BTN_TCK_SPEED_1_BACK BUTTON_5
#define BTN_TCK_SPEED_2_BACK BUTTON_6

/* Min max value of LED display */
#define BTN_MAX_VALUE (int16_t)9999
#define BTN_MIN_VALUE (int16_t)0

static void BTN_IncrLedValue(void);
static void BTN_DecrLedValue(void);

//#define BTNS

void BTN_Init(void)
{

#ifdef BTNS
    /* LED display */
    DIO_ConfigurePin(BTN_INCR_LED_DISPLAY_VALUE,    CP_D, CP_0, CP_R, CP_ON, CP_RD);
	DIO_ConfigurePin(BTN_DECR_LED_DISPLAY_VALUE,    CP_D, CP_1, CP_R, CP_ON, CP_RD);
	DIO_ConfigurePin(BTN_RESET_LED_DISPLAY_VALUE,   CP_D, CP_2, CP_R, CP_ON, CP_RD);

    /* Ticker */
    DIO_ConfigurePin(BTN_TCK_SPEED_1_FRWD, CP_D, CP_3, CP_R, CP_ON, CP_RD);
	DIO_ConfigurePin(BTN_TCK_SPEED_2_FRWD, CP_D, CP_4, CP_R, CP_ON, CP_RD);
	DIO_ConfigurePin(BTN_TCK_SPEED_1_BACK, CP_D, CP_5, CP_R, CP_ON, CP_RD);
	DIO_ConfigurePin(BTN_TCK_SPEED_2_BACK, CP_D, CP_6, CP_R, CP_ON, CP_RD);
#endif
}

static void BTN_IncrLedValue(void) 
{
    uint16_t displayValue = 0;
    GW_Read_LedDispayValue(&displayValue);
    displayValue++;
    if(displayValue >= BTN_MAX_VALUE) 
    {
        displayValue = BTN_MIN_VALUE;
    }
    GW_Write_LedDispayValue(displayValue);
}

static void BTN_DecrLedValue(void) 
{
    uint16_t displayValue = 0;
    GW_Read_LedDispayValue(&displayValue);
    displayValue--;
    if(displayValue < BTN_MIN_VALUE) 
    {
        displayValue = BTN_MAX_VALUE;
    }
    GW_Write_LedDispayValue(displayValue);
}

void BTN_HandleButtons(void) 
{
 /*   uint16_t tmpDisplayValue = 0;
	
    if(DIO_ReadPin(BUTTON_0) == CP_ACT) 
    {
        GW_Read_ADC_ChannelValue(&tmpDisplayValue, ADC_POT1);
	    GW_Write_LedDispayValue(tmpDisplayValue);
    } else if(DIO_ReadPin(BUTTON_1) == CP_ACT) 
    {
        GW_Read_ADC_ChannelValue(&tmpDisplayValue, ADC_POT2);
	    GW_Write_LedDispayValue(tmpDisplayValue);
    } else if(DIO_ReadPin(BUTTON_2) == CP_ACT) 
    {
        GW_Read_ADC_ChannelValue(&tmpDisplayValue, ADC_3_3);
	    GW_Write_LedDispayValue(tmpDisplayValue);
    } else {

    }*/
    
#ifdef BTNS
    int8_t tmpSpeed = 0;

    /* Led Dispay */
    /* Led display value incrementation */
    if(DIO_ReadPin(BTN_INCR_LED_DISPLAY_VALUE) == CP_ACT) 
    {
        BTN_IncrLedValue();
    }

    /* Led display value decrementation */
    if(DIO_ReadPin(BTN_DECR_LED_DISPLAY_VALUE) == CP_ACT) 
    {
        BTN_DecrLedValue();
    }

    /* Ticker */
    if(DIO_ReadPin(BTN_TCK_SPEED_1_FRWD) == CP_ACT) 
    {
        tmpSpeed = TCK_SPEED_1_FRWD;
    } else if(DIO_ReadPin(BTN_TCK_SPEED_2_FRWD) == CP_ACT) 
    {
        tmpSpeed = TCK_SPEED_2_FRWD;
    } else if(DIO_ReadPin(BTN_TCK_SPEED_1_BACK) == CP_ACT) 
    {
        tmpSpeed = TCK_SPEED_1_BACK;
    } else if(DIO_ReadPin(BTN_TCK_SPEED_2_BACK) == CP_ACT) 
    {
        tmpSpeed = TCK_SPEED_2_BACK;
    } else {
        tmpSpeed = TCK_SPEED_0;
    }

    GW_Write_TCK_CurrentSpeed(tmpSpeed); 
#endif
}