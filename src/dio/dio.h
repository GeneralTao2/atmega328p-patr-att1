#ifndef dio_h
#define dio_h

#include <avr/io.h>
#include "../utils/utils.h"
#include "../defines.h"

/**
 * \def: CP_LOW
 * \brief: Used at HW level - shows the low state of register's pin
 */
#ifndef CP_LOW
#define CP_LOW 0
#endif

/**
 * \def: CP_HIGH
 * \brief: Used at HW level - shows the low state of register's pin
 */
#ifndef CP_HIGH
#define CP_HIGH 1
#endif

/**
 * \def: CP_IN
 * \brief: Shows the data direction, input mode
 */
#ifndef CP_IN
#define CP_IN 0
#endif

/*
 * \def: CP_OUT
 * \brief: Shows the data direction, output mode
 */
#ifndef CP_OUT
#define CP_OUT 1
#endif

/*
 * \def: CP_[A-D]
 * \brief: SW index of HW PORT, DDR or PIN register
 */
#ifndef CP_PORT
#define CP_PORT
#ifdef ATMEGA32
#define CP_A 0
#define CP_B 1
#define CP_C 2
#define CP_D 3
#endif
#ifdef ATMEGA328P
#define CP_B 0
#define CP_C 1
#define CP_D 2
#endif
#endif

/*
 * \def: CP_[0-7]
 * \brief: SW index of a register bit
 */
#ifndef CP_PIN
#define CP_PIN
#define CP_0 0
#define CP_1 1
#define CP_2 2
#define CP_3 3
#define CP_4 4
#define CP_5 5
#define CP_6 6
#define CP_7 7
#endif

/*
 * \def: CP_ON
 * \brief: Used at user level - shows the on state of a digital pin
 */
#ifndef CP_ON
#define CP_ON 1
#endif

/*
 * \def: CP_OFF
 * \brief: Used at user level - shows the off state of a digital pin
 */
#ifndef CP_OFF
#define CP_OFF 0
#endif

/*
 * \def: CP_RD
 * \brief: Shows the access of reading of a digital pin
 */
#ifndef CP_RD
#define CP_RD 100
#endif

/*
 * \def: CP_WR
 * \brief: Shows the access of writing of a digital pin
 */
#ifndef CP_WR
#define CP_WR 101
#endif

/*
 * \def: CP_R
 * \brief: Shows the regular control type
 */
#ifndef CP_R
#define CP_R 200
#endif

/*
 * \def: CP_I
 * \brief: Shows the invers control type
 */
#ifndef CP_I
#define CP_I 201
#endif

/*
 * \def: CP_ACT
 * \brief: Shows the active state
 */
#ifndef CP_ACT
#define CP_ACT 1
#endif
/*
 * \def: CP_ACT
 * \brief: Shows the passive state
 */
#ifndef CP_PASS
#define CP_PASS 0
#endif

/*
 * \def: te_DIO_Pins
 * \brief: Enumeration of digital pins, to be used. Each enum element
 * 		is an identifire of certain digital pin. So you can work with
 * 		functions by identifires.
 * 		PIN_QUANTITY - is digital pins quantity, so cannot be used as
 * 		argument of function.
 */
typedef enum {
	/* LCD Display */
	LCD_D0,
	LCD_D1,
	LCD_D2,
	LCD_D3,
	LCD_D4,
	LCD_D5,
	LCD_D6,
	LCD_D7,
	LCD_RS,
	LCD_RW,
	LCD_E,
	LCD_BL,
	MEC_PIN,
	TCR_PIN,
	/* Leds */
	LED_0,
	LED_1,
	LED_2,
	/* Buttons */
	BUTTON_0,
	BUTTON_1,
	/* Led for run indication */
	LED_RUN,
	/* te_DIO_Pins element's quantity */
	PIN_QUANTITY,
} te_DIO_Pins;

/*
 * DIO level
 */ 
extern uint8_t HL_ReadPIN(uint8_t port);

extern void DIO_Init(void);
extern void DIO_ConfigurePin(te_DIO_Pins digitalPin, uint8_t port, uint8_t pin, uint8_t controlType, uint8_t initState, uint8_t access);
extern void DIO_PinOn(te_DIO_Pins digitalPin);
extern void DIO_PinOff(te_DIO_Pins digitalPin);
extern void DIO_TogglePin(te_DIO_Pins dio);
extern void DIO_WritePin(te_DIO_Pins dio, uint8_t state);
extern uint8_t DIO_ReadPin(te_DIO_Pins digitalPin);
extern uint8_t DIO_ReadDeboucedPin(te_DIO_Pins dio);

#endif



