#include "blinker.h"
#include "../dio/dio.h"

void BLK_Init(void) 
{
    /* New flash load indicator */
	DIO_ConfigurePin(LED_RUN, CP_C, CP_2, CP_I, CP_OFF, CP_WR);
}

void BLK_Blink(void) 
{
    DIO_TogglePin(LED_RUN);
}