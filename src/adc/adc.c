#include "adc.h"

#include "../signalgateway/signalgateway.h"
#include "../defines.h"

uint8_t ADC_Configs[ADC_QUANTITY] = {0};

#define ADC_CFG_MUX 0
#define ADC_CFG_REF 3
#define ADC_CFG_STATE 4

#define ADC_CFG_MUX_MASK (7 << ADC_CFG_MUX)

#define ADC_ADMUX_ACTIVE_MUX_MASK 0x07 
#define ADC_ADMUX_WHOLE_MUX_MASK 0x1F 

te_ADC_Pins ADC_oldLastChannel = 0u;
te_ADC_Pins ADC_lastChannel = 0u;
te_ADC_Pins ADC_currentChannel = 0u;

void ADC_MuxToConfig(const te_ADC_Pins adc, const uint8_t mux);
void ADC_RefToConfig(const te_ADC_Pins adc, const uint8_t state);
void ADC_SelectChannel(const te_ADC_Pins adc);
uint16_t ADC_ReadCurrentChannelValue(void);
void ADC_IncrCurrentChannel(void);

void ADC_MuxToConfig(const te_ADC_Pins adc, const uint8_t mux) 
{
	U_BitSeqToConfig(&ADC_Configs[adc], ADC_CFG_MUX, ADC_CFG_MUX_MASK, mux);
}

uint8_t ADC_MuxFromConfig(const te_ADC_Pins adc) 
{
	uint8_t retVal = 0u;
	retVal = U_BitSeqFromConfig(&ADC_Configs[adc], ADC_CFG_MUX, ADC_CFG_MUX_MASK);
	return retVal;
}

void ADC_RefToConfig(const te_ADC_Pins adc, const uint8_t state) 
{
	U_BitToConfig(&ADC_Configs[adc], ADC_CFG_REF, ADC_REF_EXT, ADC_REF_INT, state);
}

uint8_t ADC_RefFromConfig(const te_ADC_Pins adc) 
{
	uint8_t retVal = CP_R;
	retVal = U_BitFromConfig(&ADC_Configs[adc], ADC_CFG_REF, ADC_REF_EXT, ADC_REF_INT);
	return retVal;
}

void ADC_StateToConfig(const te_ADC_Pins adc, const uint8_t state) 
{
	U_BitToConfig(&ADC_Configs[adc], ADC_CFG_STATE, ADC_OFF, ADC_ON, state);
}

uint8_t ADC_StateFromConfig(const te_ADC_Pins adc) 
{
	uint8_t retVal = CP_R;
	retVal = U_BitFromConfig(&ADC_Configs[adc], ADC_CFG_STATE, ADC_OFF, ADC_ON);
	return retVal;
}

void ADC_SelectChannel(const te_ADC_Pins adc) 
{
	uint8_t mux = ADC0_MUX;
	uint8_t ref = ADC_REF_INT;

	mux = ADC_MuxFromConfig(adc);
	ref = ADC_RefFromConfig(adc);

	/* Set ADC channel */
	ADMUX &= ~(ADC_ADMUX_WHOLE_MUX_MASK);
	ADMUX |= mux & (ADC_ADMUX_ACTIVE_MUX_MASK);

	/* Set ADC ref */
	if(ref == ADC_REF_EXT)
	{
		CLR_BIT(ADMUX, REFS1);
		CLR_BIT(ADMUX, REFS0);
	} else 
	{
		SET_BIT(ADMUX, REFS1);
		SET_BIT(ADMUX, REFS0);
	}
}

void ADC_ConfigurePin(const te_ADC_Pins adc, const uint8_t mux, const uint8_t ref, const uint8_t state) 
{
    ADC_MuxToConfig(adc, mux);
	ADC_RefToConfig(adc, ref);
	ADC_StateToConfig(adc, state);
}

void ADC_PreInit(void) 
{
	ADMUX = 0;
	ADCSRA = 0;
#ifdef ATMEGA32
	SFIOR = 0;
#endif
}

void ADC_PostInit(void) {
	ADCSRA |= (1 << ADEN);
}

uint16_t ADC_ReadCurrentChannelValue(void) 
{
	uint16_t retChannelValue = 0u;
	retChannelValue = ADCL;
	retChannelValue += (ADCH << (BYTE_LENTH) );
	return retChannelValue;
}

void ADC_IncrCurrentChannel(void) 
{
	ADC_currentChannel++;
	if(ADC_currentChannel >= ADC_QUANTITY) 
	{
		ADC_currentChannel = 0u;
	}
}

void ADC_NextChannel(void) 
{
	uint16_t tmpChannelValue = 0u;
	uint8_t state = ADC_OFF;
	uint8_t idx = 0u;

	/* Firstly we save n-1 counter in n-2 counter */
	ADC_oldLastChannel = ADC_lastChannel;
	/* Then save n counter in n-1 counter */
	ADC_lastChannel = ADC_currentChannel;

	/* Next ADC channel */
	ADC_IncrCurrentChannel();
	state = ADC_StateFromConfig(ADC_currentChannel);

	/* Next we jump over disabled ADC channels */
	for(idx = 0; (idx < ADC_QUANTITY) && (state == ADC_OFF); idx++) 
	{
		ADC_IncrCurrentChannel();
		state = ADC_StateFromConfig(ADC_currentChannel);
	}
	
	/* Read ADC value */
	tmpChannelValue = ADC_ReadCurrentChannelValue();
	/* And store it inside array in  the GW */
	GW_Write_ADC_ChannelValue(tmpChannelValue, ADC_oldLastChannel);

	/* Start conversion of last ADMUX settings */
	ADCSRA |= (1<<ADSC);
	/* And configure ADMUX for new current channel */
	ADC_SelectChannel(ADC_currentChannel);
	
}

/* ===================================== */
#include "../stringmanager/stringmanager.h"

uint16_t ADC_ReadVolts(const te_ADC_Pins adc);
void ADC_Show(void);

void ADC_Init(void)
{
	ADC_PreInit();
	ADC_ConfigurePin(ADC_POT, ADC0_MUX, ADC_REF_EXT, ADC_ON);
	ADC_ConfigurePin(ADC_LM50, ADC1_MUX, ADC_REF_EXT, ADC_ON);
	ADC_PostInit();
}

void ADC_Run(void)
{
	ADC_NextChannel();
	ADC_Show();
}

uint16_t ADC_ReadVolts(const te_ADC_Pins adc) 
{
	uint16_t tmpChannelValue = 0u;
	uint8_t ref = ADC_REF_EXT;
	uint16_t retValue = 0u;

	GW_Read_ADC_ChannelValue(&tmpChannelValue, adc);
	ref = ADC_RefFromConfig(adc);

	if(ref == ADC_REF_EXT)
	{
		retValue = ADC_ADC_TO_VOLTS(ADC_REF_EXT_VOLT, tmpChannelValue);
	} else 
	{
		retValue = ADC_ADC_TO_VOLTS(ADC_REF_INT_VOLT, tmpChannelValue);
	}
	return retValue;
}

void ADC_Show(void) 
{
	
}
