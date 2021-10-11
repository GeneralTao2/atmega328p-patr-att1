#include "defines.h"
#include "tasktimer/tasktimer.h"
#include "button/button.h"
#include "signalgateway/signalgateway.h"
#include "lcddispay/lcddisplay.h"
#include "stringmanager/stringmanager.h"
#include "blinker/blinker.h"
#include "debounce/debounce.h"
#include "pwm/pwm.h"
#include "motorencoder/motorencoder.h"
#include "pid/pid.h"
#include "adc/adc.h"
#include "motorcontroller/motorcontroller.h"
#include "termocontrol/termocontrol.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	DIO_Init();
	LCD_Init();
    TT_Init();
	BLK_Init();
	PWM_Init();
	MEC_Init();
	ADC_Init();
	TCR_Init();

    sei();
    while (1) 
    {	
		if(TT_Event1ms == EVENT_ARRIVE) 
		{
			LCD_Run();
			MEC_CountPulsePeriod();

			TT_Event1ms = EVENT_WAIT;
		}
		if(TT_Event5ms == EVENT_ARRIVE) 
		{
			
			TT_Event5ms = EVENT_WAIT;
		}
		if(TT_Event10ms == EVENT_ARRIVE) 
		{
			DBC_Debounce();
			ADC_Run();
			MCR_Run();
			

			TT_Event10ms = EVENT_WAIT;
		}
		if(TT_Event100ms == EVENT_ARRIVE) 
		{	
			LCD_FillCurrentCharacters();
			MCR_Show();
			TCR_Run();
			
			TT_Event100ms = EVENT_WAIT;
		}
		if(TT_Event1000ms == EVENT_ARRIVE) 
		{
			BLK_Blink();
			TT_Event1000ms = EVENT_WAIT;
		}
    }
}


