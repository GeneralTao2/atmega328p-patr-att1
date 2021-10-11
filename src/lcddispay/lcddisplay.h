#ifndef lcddisplay_h
#define lcddisplay_h

#include <avr/io.h>



#include "../dio/dio.h"
#include "../signalgateway/signalgateway.h"
#include "../defines.h"
#include "../utils/utils.h"

extern void LCD_Init(void);
extern void LCD_WriteData(uint8_t data);
extern void LCD_FillCurrentCharacters(void);
extern void LCD_Run(void);
extern void LCD_SendChar(uint8_t charact);
extern void LCD_SendCommand(uint8_t command);


#endif