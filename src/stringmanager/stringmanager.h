#ifndef stringmanager_h
#define stringmanager_h

#include <avr/io.h>

#define STR_ALIGNMENT_LEFT 0
#define STR_ALIGNMENT_RIGHT 1

#define STR_FILLING_ZEROS 0
#define STR_FILLING_SPACES 1
#define STR_FILLING_NONE 2

#define STR_8BIT_STRING_LENGTH 2u

extern void STR_NumberToString(char *str, const uint32_t number);
extern uint8_t STR_StringTo8BitHex(uint8_t const src[], uint8_t *error);
extern uint16_t STR_StringTo16BitHex(const uint8_t src[], uint8_t *error);
extern void STR_8BitHexToString(uint8_t dst[], const uint8_t hex);

extern uint8_t STR_CharToHexDigit(const int8_t charact, uint8_t *error);
extern int8_t STR_HexDigitToChar(const uint8_t hex, uint8_t *error);

extern void STR_WriteStringToArray(const char src[], char dst[], const uint8_t position, const uint8_t length);
extern void STR_WriteNumberToArray(char dst[], const uint8_t position, const uint8_t length, const uint8_t alignment, const uint8_t filling, const uint32_t number);

extern void STR_WriteStringToLCD(const uint8_t lineId, const uint8_t position, const uint8_t length, const char *str);
extern void STR_WriteNumberToLCD(const uint8_t lineId, const uint8_t position, const uint8_t length, const uint8_t alignment, const uint8_t filling, uint32_t number);

extern uint32_t STR_16bitDecToBCD(uint32_t dec);

#endif