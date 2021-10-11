#include "dio.h"
#include "../debounce/debounce.h"


/*
 * ====================== Defines
 */
/*
 * HAL (Hardware Abstraction Layer)
 */
/**
 * \def: HAL_CFG_PORT
 * \brief: The bit position of config variable, from where index of port begins to settle down
 */
#define HAL_CFG_PORT 0

/**
 * \def: HAL_CFG_PIN
 * \brief: The bit position of config variable, from where index of pin begins to settle down
 */
#define HAL_CFG_PIN 3

/**
 * \def: HAL_CFG_TYPE
 * \brief: The bit position of config variable, where control type settles down
 */
#define HAL_CFG_TYPE 6

/**
 * \def: HAL_CFG_ACCESS
 * \brief: The bit position of config variable, where pin access settles down
 */
#define HAL_CFG_ACCESS 7


/**
 * \def: HAL_CFG_PORT_MASK
 * \brief: The bit mask of a place in a config variable, where port settles down
 */
#define HAL_CFG_PORT_MASK (7 << HAL_CFG_PORT) /* 7 - 0b00000111 */

/**
 * \def: HAL_CFG_PIN_MASK
 * \brief: The bit mask of a place in a config variable, where pin settles down
 */
#define HAL_CFG_PIN_MASK (7 << HAL_CFG_PIN)	/* 7 - 0b00000111 */


/*
 * ====================== Global variables
 */
/*
 * HAL (Hardware Abstraction Layer)
 */
/**
 * \def: HAL_Config[PIN_QUANTITY]
 * \brief: The array of DIO pins configuration. Array can be accessed only by enums,
 * 		declared in te_DIO_Pins enum type. So, each HAL_Config array cell correspondes
 * 		for its own enum. PIN_QUANTITY is only to define array length, so you cannot
 * 		access array by PIN_QUANTITY.
 */
uint8_t HAL_Config[PIN_QUANTITY] = { 0 };

/*
 * ====================== Function prototypes
 */
/*
 * HL (Hardware Layer) 
 */
static void HL_SetPORTBit(uint8_t port, uint8_t pin);
static void HL_ClearPORTBit(uint8_t port, uint8_t pin);
static void HL_TogglePORTBit(uint8_t port, uint8_t pin);
static void HL_SetDDRBit(uint8_t ddr, uint8_t pin); 
static void HL_ClearDDRBit(uint8_t ddr, uint8_t pin);
static uint8_t HL_ReadPINBit(uint8_t port, uint8_t pin);

/*
 * HAL (Hardware Abstraction Layer). Function declarations. 
 */
static void HAL_WritePin(te_DIO_Pins dio, uint8_t state);
static void HAL_WritePinDirection(te_DIO_Pins dio, uint8_t direction);
static void HAL_TogglePin(te_DIO_Pins dio);
static void HAL_InitWRPin(te_DIO_Pins dio, uint8_t initState);
static void HAL_InitRDPin(te_DIO_Pins dio, uint8_t pull);
static uint8_t HAL_ReadPin(te_DIO_Pins dio);
static uint8_t HAL_ReadDebouncedPin(te_DIO_Pins dio);


static void HAL_PortToConfig(te_DIO_Pins dio, uint8_t port);
static uint8_t HAL_PortFromConfig(te_DIO_Pins dio);

static void HAL_PinToConfig(te_DIO_Pins dio, uint8_t pin);
static uint8_t HAL_PinFromConfig(te_DIO_Pins dio);

static void HAL_TypeToConfig(te_DIO_Pins dio, uint8_t type);
static uint8_t HAL_TypeFromConfig(te_DIO_Pins dio);

/**
 * \def: DIO_ASCESS_USING
 * \brief: Define DIO_ASCESS_USING if you want to use HAL_AccessFromConfig
 */
#ifdef DIO_ASCESS_USING
static void HAL_AccessToConfig(te_DIO_Pins dio, uint8_t access);
static uint8_t HAL_AccessFromConfig(te_DIO_Pins dio);
#endif

/*
 * ====================== Function definitions
 */
void DIO_Init(void)
{
#ifdef ATMEGA32
    DDRA = 0u;
#endif
    DDRB = 0u;
    DDRC = 0u;
    DDRD = 0u;
#ifdef ATMEGA32
    PORTA = 0u;
#endif
    PORTB = 0u;
    PORTC = 0u;
    PORTD = 0u;
}
/*
 * HL (Hardware Layer) 
 */
/**
 * void HL_SetPORTBit(uint8_t port, uint8_t pin)
 * \brief: 
 * 		Sets the PORT register's bit mask		 
 * \param[in]:	port
 * 		An index CP_[A-D] of register PORT[A-D] to be modified 
 *              pin
 * 		A pin index, that represents a bit of port to be set
 * \description: 
 * 		This function writes 1 to the PORT[A-D] register's PORT[A-D][0-7] bit 
 * \return value:
 * 		No return value
 */
static void HL_SetPORTBit(uint8_t port, uint8_t pin) 
{
    uint8_t bitMask = 0u;
    bitMask = U_bitMasks[pin];
    switch(port) 
    {
#ifdef ATMEGA32
        case CP_A: 
            PORTA |= bitMask;
            break;
#endif
        case CP_B:
            PORTB |= bitMask;
            break;
        case CP_C:
            PORTC |= bitMask;
            break;
        case CP_D:
            PORTD |= bitMask;
            break;
        default:
            break;
	}
}

/**
 * void HL_ClearPORTBit(uint8_t port, uint8_t pin)
 * \brief: 
 * 		Clears the PORT register's bit mask		 
 * \param[in]:	port
 * 		An index CP_[A-D] of register PORT[A-D] to be modified 
 *              pin
 * 		A pin index, that represents a bit of port to be cleared
 * \description: 
 * 		This function writes 0 to the PORT[A-D] register's PORT[A-D][0-7] bit 
 * \return value:
 * 		No return value
 */
static void HL_ClearPORTBit(uint8_t port, uint8_t pin) 
{
    uint8_t bitMask = 0u;
    bitMask = ~U_bitMasks[pin];
    switch(port) 
    {
#ifdef ATMEGA32
        case CP_A:
            PORTA &= bitMask;
            break;
#endif
        case CP_B:
            PORTB &= bitMask;
            break;
        case CP_C:
            PORTC &= bitMask;
            break;
        case CP_D:
            PORTD &= bitMask;
            break;
        default:
            break;
	}
}

/**
 * void HL_TogglePORTBit(uint8_t port, uint8_t pin)
 * \brief: 
 * 		Toggle register's bit mask		 
 * \param[in]:	port
 * 		An index CP_[A-D] of register PORT[A-D] to be modified 
 *              pin
 * 		A pin index, that represents a bit of port to be toggle
 * \description: 
 * 		This function toggles the PORT[A-D] register's PORT[A-D][0-7] bit 
 *      0 to 1 or otherwise
 * \return value:
 * 		No return value
 */
static void HL_TogglePORTBit(uint8_t port, uint8_t pin) 
{
    uint8_t bitMask = 0u;
    bitMask = U_bitMasks[pin];
    switch(port) 
    {
#ifdef ATMEGA32
        case CP_A:
            PORTA ^= bitMask;
            break;
#endif
        case CP_B:
            PORTB ^= bitMask;
            break;
        case CP_C:
            PORTC ^= bitMask;
            break;
        case CP_D:
            PORTD ^= bitMask;
            break;
        default:
            break;
	}
}

/**
 * void HL_SetDDRBit(uint8_t port, uint8_t pin)
 * \brief: 
 * 		Sets the DDR register's bit	mask	 
 * \param[in]:	port
 * 		An index CP_[A-D] of register DDR[A-D] to be modified 
 *              pin
 * 		A pin index, that represents a bit of port to be set
 * \description: 
 * 		This function writes 1 to the DDR[A-D] register's DDR[A-D][0-7] bit 
 * \return value:
 * 		No return value
 */
static void HL_SetDDRBit(uint8_t port, uint8_t pin) 
{
    uint8_t bitMask = 0u;
    bitMask = U_bitMasks[pin];
    switch(port) 
    {
#ifdef ATMEGA32
        case CP_A:
            DDRA |= bitMask;
            break;
#endif
        case CP_B:
            DDRB |= bitMask;
            break;
        case CP_C:
            DDRC |= bitMask;
            break;
        case CP_D:
            DDRD |= bitMask;
            break;
        default:
            break;
	}
}

/**
 * void HL_ClearDDRBit(uint8_t port, uint8_t pin)
 * \brief: 
 * 		Clears the DDR register's bit mask		 
 * \param[in]:	port
 * 		An index CP_[A-D] of register DDR[A-D] to be modified 
 *              pin
 * 		A pin index, that represents a bit of port to be cleared
 * \description: 
 * 		This function writes 1 to the DDR[A-D] register's DDR[A-D][0-7] bit 
 * \return value:
 * 		No return value
 */
static void HL_ClearDDRBit(uint8_t port, uint8_t pin) 
{
    uint8_t bitMask = 0u;
    bitMask = ~U_bitMasks[pin];
    switch(port) 
    {
#ifdef ATMEGA32
        case CP_A:
            DDRA &= bitMask;
            break;
#endif
        case CP_B:
            DDRB &= bitMask;
            break;
        case CP_C:
            DDRC &= bitMask;
            break;
        case CP_D:
            DDRD &= bitMask;
            break;
        default:
            break;
	}
}

/*todo description*/
uint8_t HL_ReadPIN(uint8_t port) 
{
    uint8_t retValue = 0x00;

    switch(port) 
    {
#ifdef ATMEGA32
        case CP_A:
            retValue = PINA;
            break;
#endif
        case CP_B:
            retValue = PINB;
            break;
        case CP_C:
            retValue = PINC;
            break;
        case CP_D:
            retValue = PIND;
            break;
        default:
            break;
	}

    return retValue;
}

/**
 * uint8_t HL_ReadPINBit(uint8_t port, uint8_t pin)
 * \brief: 
 * 		Returns PIN register's bit		 
 * \param[in]:	port
 * 		An index CP_[A-D] of register DDR[A-D] 
 *              pin
 * 		A pin index, that represents a bit of port to be read
 * \description: 
 * 		This function reads and returns the DDR[A-D] register's DDR[A-D][0-7] bit 
 * \return value:
 * 		Will return CP_HIGH - bit is 1, or CP_LOW - bit os 0
 */
static uint8_t HL_ReadPINBit(uint8_t port, uint8_t pin) 
{
    uint8_t bitValue = CP_LOW;
    uint8_t retValue = CP_LOW;
    uint8_t bitMask = 0u;
    uint8_t PIN_Reg = 0x00;
    bitMask = U_bitMasks[pin];

    PIN_Reg = HL_ReadPIN(port);
    bitValue = PIN_Reg & bitMask;

    if(bitValue != CP_LOW) {
		retValue = CP_HIGH;
	} else {
		retValue = CP_LOW;
	}
    return retValue;
}


/*
 * HAL (Hardware Abstraction Layer)  
 */
/**
 * void HAL_WritePin(te_DIO_Pins dio, uint8_t state)
 * \brief: 
 * 		Writes a state to a digital pin	 
 * \param[in]:	dio
 * 		A digital pin to be writen 
 *              state
 * 		A state to be writen to digital pin
 * \description: 
 * 		If digital pin configured to CP_OUT, then this function supplies 
 *      digital pin with high voltage, if state is CP_HIGH, or low voltage, 
 *      if state is CP_LOW.
 *      If digital pin configured to CP_IN, then connects Pull-Up resistor,
 *      if state is CP_HIGH, or disconnect, if state is CP_LOW
 * \return value:
 * 		No return value
 */
static void HAL_WritePin(te_DIO_Pins dio, uint8_t state) 
{
	uint8_t port = 0u;
	uint8_t pin = 0u;

    port = HAL_PortFromConfig(dio);
	pin = HAL_PinFromConfig(dio);

	if(state == CP_LOW) 
    {
        HL_ClearPORTBit(port, pin);
	} else 
    {
        HL_SetPORTBit(port, pin);
	}
}

/**
 * uint8_t HAL_ReadPin(te_DIO_Pins dio)
 * \brief: 
 * 		Reads state from a digital pin		 
 * \param[in]:	dio
 * 		A digital pin to be read 
 * \description: 
 * 		This finction reads and returns digital pin state. Works only if 
 *      digital pin has been configured to CP_IN
 * \return value:
 * 		Will return CP_LOW if digital pin is supplied with low voltage,
 *      or CP_HIGH, if with high voltage 
 */
static uint8_t HAL_ReadPin(te_DIO_Pins dio) 
{
    uint8_t port = 0u;
	uint8_t pin = 0u;

    port = HAL_PortFromConfig(dio);
	pin = HAL_PinFromConfig(dio);
    uint8_t bitValue = HL_ReadPINBit(port, pin);

    return bitValue;
} 

/*todod description*/
static uint8_t HAL_ReadDebouncedPin(te_DIO_Pins dio) 
{
    uint8_t port = 0u;
	uint8_t pin = 0u;

    port = HAL_PortFromConfig(dio);
	pin = HAL_PinFromConfig(dio);
    uint8_t bitValue = DBC_ReadDebouncedPin(port, pin);

    return bitValue;
} 


/**
 * void HAL_WritePinDirection(te_DIO_Pins dio, uint8_t direction)
 * \brief: 
 * 		Configures digital pin's data direction		 
 * \param[in]:	dio
 * 		A digital pin to be configure 
 * \description: 
 * 		This finction configures pin to be output, if direction is
 *      CP_OUT, so you can supply pin. Or to be input, if direction
 *      is CP_IN, so you can read pin state  
 * \return value:
 * 		No return value 
 */
static void HAL_WritePinDirection(te_DIO_Pins dio, uint8_t direction) 
{
    uint8_t port = 0u;
	uint8_t pin = 0u;

    port = HAL_PortFromConfig(dio);
	pin = HAL_PinFromConfig(dio);

    if(direction == CP_OUT) 
    {
		HL_SetDDRBit(port, pin);
	} else 
    {
		HL_ClearDDRBit(port, pin);
	}
}

/**
 * void HAL_TogglePin(te_DIO_Pins dio)
 * \brief: 
 * 		Toggles digital pin	 
 * \param[in]:	dio
 * 		A digital pin to be toggle
 * \description: 
 * 		This function toggles the digital pin from CP_HIGH to
 *      CP_LOW or otherwise. If pin has been configured as CP_OUT,
 *      then toggles supply state. If as CP_IN, then toggles Pull-Up
 *      connection.
 * \return value:
 * 		No return value
 */
static void HAL_TogglePin(te_DIO_Pins dio) 
{
    uint8_t port = 0u;
	uint8_t pin = 0u;

    port = HAL_PortFromConfig(dio);
	pin = HAL_PinFromConfig(dio) ;
    HL_TogglePORTBit(port, pin);
}



/**
 * void HAL_PortToConfig(te_DIO_Pins dio, uint8_t port)
 * \brief: 
 * 		Confogure port of digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * 				port
 * 		An index of port to be writen to config variable. Can be any of CP_[A-D].
 * \description: 
 * 		This function writes index of port to config variable
 * \return value:
 * 		No return value
 */
static void HAL_PortToConfig(te_DIO_Pins dio, uint8_t port) 
{
	U_BitSeqToConfig(&HAL_Config[dio], HAL_CFG_PORT, HAL_CFG_PORT_MASK, port);
}

/**
 * uint8_t HAL_PortFromConfig(te_DIO_Pins dio)
 * \brief: 
 * 		Return index of port of digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * \description: 
 * 		This function returns index of port of digital pin dio.
 * \return value:
 * 		This function will return one of CP_[A-D] or CP_A, if port was not
 * 		configured.
 */
static uint8_t HAL_PortFromConfig(te_DIO_Pins dio) 
{
	uint8_t retVal = 0u;
	retVal = U_BitSeqFromConfig(&HAL_Config[dio], HAL_CFG_PORT, HAL_CFG_PORT_MASK);
	return retVal;
}

/**
 * void HAL_PinToConfig(te_DIO_Pins dio, uint8_t pin)
 * \brief: 
 * 		Confogure port of digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * 				pin
 * 		An index of pin to be writen to config variable. Can be any of CP_[0-7]
 * \description: 
 * 		This function writes index of pin to config variable  of digital pin dio
 * \return value:
 * 		No return value
 */
static void HAL_PinToConfig(te_DIO_Pins dio, uint8_t pin) 
{
	U_BitSeqToConfig(&HAL_Config[dio], HAL_CFG_PIN, HAL_CFG_PIN_MASK, pin);
}

/**
 * uint8_t HAL_PortFromConfig(te_DIO_Pins dio)
 * \brief: 
 * 		Return index of port of digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * \description: 
 * 		This function returns an index of port of digital pin dio
 * \return value:
 * 		This function will return one of CP_[A-D] or CP_A, if port was not
 * 		configured.
 */
static uint8_t HAL_PinFromConfig(te_DIO_Pins dio) 
{
	uint8_t retVal = 0u;
	retVal = U_BitSeqFromConfig(&HAL_Config[dio], HAL_CFG_PIN, HAL_CFG_PIN_MASK);
	return retVal;
}

/**
 * void HAL_TypeToConfig(te_DIO_Pins dio, uint8_t type)
 * \brief: 
 * 		Confogure control type of digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * 				type
 * 		A control type to be writen to config variable. Can be CP_R - regular
 * 		type or CP_I - inverse type
 * \description: 
 * 		This function writes control type to config variable of digital pin dio
 * \return value:
 * 		No return value
 */
static void HAL_TypeToConfig(te_DIO_Pins dio, uint8_t type) 
{
	U_BitToConfig(&HAL_Config[dio], HAL_CFG_TYPE, CP_R, CP_I, type);
}


/**
 * uint8_t HAL_TypeFromConfig(te_DIO_Pins dio)
 * \brief: 
 * 		Return index of port of digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * \description: 
 * 		This function returns a control type of digital pin dio
 * \return value:
 * 		This function will return CP_R, if digital pin has regular
 * 		control type or CP_I - has inverse control type
 */
static uint8_t HAL_TypeFromConfig(te_DIO_Pins dio) 
{
	uint8_t retVal = CP_R;
	retVal = U_BitFromConfig(&HAL_Config[dio], HAL_CFG_TYPE, CP_R, CP_I);
	return retVal;
}

#ifdef DIO_ASCESS_USING
/**
 * void HAL_AccessToConfig(te_DIO_Pins dio, uint8_t access)
 * \brief: 
 * 		Confogure access mode to digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * 				access
 * 		An access mode to be writen to config variable. Can be CP_RD - read
 * 		mode or CP_WR - write mode
 * \description: 
 * 		This function writes index of pin to config variable of digital pin dio
 * \return value:
 * 		No return value
 */
static void HAL_AccessToConfig(te_DIO_Pins dio, uint8_t access) 
{
	U_BitToConfig(&HAL_Config[dio], HAL_CFG_ACCESS, CP_RD, CP_WR, access);
}

/**
 * uint8_t HAL_AccessFromConfig(te_DIO_Pins dio) 
 * \brief: 
 * 		Return index of port of digital pin
 * \param[in]:	dio
 * 		A digital pin to be configured
 * \description: 
 * 		This function returns an access mode of digital pin dio
 * \return value:
 * 		This function will return CP_RD, if digital pin has read
 *      mode access or CP_WR - has write mode type
 */
static uint8_t HAL_AccessFromConfig(te_DIO_Pins dio) 
{
	uint8_t retVal = CP_RD;
	retVal = U_BitFromConfig(&HAL_Config[dio], HAL_CFG_ACCESS, CP_RD, CP_WR);
	return retVal;
}
#endif

/*
 * HAL level
 */ 

/**
 * void HAL_InitWRPin(te_DIO_Pins dio, uint8_t initState)
 * \brief: 
 * 		Initializes digital pin with write access	 
 * \param[in]:	dio
 * 		A digital pin
 *              initState
 *      A initial state to be write
 * \description: 
 * 		This function initializes digital port with initial state:
 * 		CP_OFF - disable, CP_ON - enable, using configured
 * 		control type. So, configure initial supply. Initialize only 
 * 		digital pin with CP_WR access.
 * \return value:
 * 		No return value
 */
static void HAL_InitWRPin(te_DIO_Pins dio, uint8_t initState) 
{
	uint8_t controlType = 0u;

	controlType = HAL_TypeFromConfig(dio);

	if( (controlType == CP_R) && (initState == CP_OFF) ) 
    {
		HAL_WritePin(dio, CP_LOW);
	}
	if( (controlType == CP_R) && (initState == CP_ON) ) 
    {
		HAL_WritePin(dio, CP_HIGH);
	}
	if( (controlType == CP_I) && (initState == CP_OFF) ) 
    {
		HAL_WritePin(dio, CP_HIGH);
	}
	if( (controlType == CP_I) && (initState == CP_ON) ) 
    {
		HAL_WritePin(dio, CP_LOW);
	}
}

/**
 * void HAL_InitRDPin(te_DIO_Pins dio, uint8_t pull)
 * \brief: 
 * 		Initializes digital pin with read access	 
 * \param[in]:	dio
 * 		A digital pin
 *              pull
 *      A initial state to be write
 * \description: 
 * 		This function initializes digital port with initial state:
 * 		CP_OFF - disable, CP_ON - enable. So, initialize Pull-Up
 * 		state. Initialize only digital pin with CP_WR access
 * \return value:
 * 		No return value
 */
static void HAL_InitRDPin(te_DIO_Pins dio, uint8_t pull) 
{
	if( pull == CP_OFF ) 
    {
		HAL_WritePin(dio, CP_LOW);
	}
	if( pull == CP_ON ) 
    {
		HAL_WritePin(dio, CP_HIGH);
	}
}

/**
 * void DIO_ConfigurePin(te_DIO_Pins dio, uint8_t port, uint8_t pin, uint8_t controlType, uint8_t initState, uint8_t access)
 * \brief: 
 * 		Configures digital pin	 
 * \param[in]:	dio
 * 		A digital pin to be configured
 *              port
 *      A port that a digital pin will be configured to. Can be CP_[A-D]
 *              pin
 *      A pin that a digital pin will be configured to. Can be CP_[0-7]
 *              controlType
 *      A control type that a digital pin will be configured to. Can be 
 *      CP_R - regular or CP_I - invrece
 * 				initState
 * 		An initial state type that a digital pin will be configured to. 
 *      Can be CP_OFF - disable or CP_ON - enable
 * 				access
 * 		An access to be write: CP_RD - read, CP_WR - write
 * \description: 
 * 		This function configures initial digital pin state, control type and 
 * 		access mode.
 * \return value:
 * 		No return value
 */
void DIO_ConfigurePin(te_DIO_Pins dio, uint8_t port, uint8_t pin, uint8_t controlType, uint8_t initState, uint8_t access) {

	/* Set pin control type */
 	HAL_PortToConfig(dio, port);
 	HAL_PinToConfig(dio, pin);
 	HAL_TypeToConfig(dio, controlType);

    #ifdef DIO_ASCESS_USING
 	HAL_AccessToConfig(dio, access);
    #endif

	/* If access is write */
	if(access == CP_WR) 
    {
		/* Then configure data direction mode to output */
        HAL_WritePinDirection(dio, CP_OUT);
		/* And initial supply to digital pin */
		HAL_InitWRPin(dio, initState);
	} else /* But if access is read */
    if(access == CP_RD) 
    {
		/* Then configure data direction mode to input */
        HAL_WritePinDirection(dio, CP_IN);
		/* And initial Pull-Up state */
		HAL_InitRDPin(dio, initState);
	} else 
    {
        /* Nothing to do */
	}
}



/**
 * uint8_t DIO_ReadPin(te_DIO_Pins dio)
 * \brief: 
 * 		Reads digital pin state	 
 * \param[in]:	dio
 * 		A digital pin to be read
 * \description: 
 * 		This function reads a state of digital pin, using 
 * 		configured control type. Can be used only on digital pin, 
 * 		configured with CP_RD access mode
 * \return value:
 * 		Will return a state of digital pin: CP_PASS - pin is passive,
 * 		CP_ACT - pin is active
 */
uint8_t DIO_ReadPin(te_DIO_Pins dio) 
{
	uint8_t controlType = CP_R;
	uint8_t pinValue = CP_OFF;
    uint8_t retVal = CP_PASS;
	
	pinValue = HAL_ReadPin(dio);
    controlType = HAL_TypeFromConfig(dio); 

	if( (controlType == CP_R) && (pinValue == CP_LOW) ) 
    {
		retVal = CP_PASS;
	}
	if( (controlType == CP_R) && (pinValue == CP_HIGH) ) 
    {
		retVal = CP_ACT;
	}
	if( (controlType == CP_I) && (pinValue == CP_LOW) ) 
    {
		retVal = CP_ACT;
	}
	if( (controlType == CP_I) && (pinValue == CP_HIGH) ) 
    {
		retVal = CP_PASS;
	}

	return retVal;
}

/*todo description*/
uint8_t DIO_ReadDeboucedPin(te_DIO_Pins dio) 
{
	uint8_t pinValue = CP_OFF;
    uint8_t retVal = CP_PASS;
	
	pinValue = HAL_ReadDebouncedPin(dio);
    /* !!!!! todo control type has bad influence */
    if(pinValue == CP_OFF)
    {
        retVal = CP_PASS;
    } else 
    {
        retVal = CP_ACT;
    }

	return retVal;
}

/**
 * void DIO_PinOn(te_DIO_Pins dio)
 * \brief: 
 * 		Enables digital pin	 
 * \param[in]:	dio
 * 		A digital pin to be enabled
 * \description: 
 * 		This function enables digital pin, using configured
 * 		control type. Can be used only on digital pin,
 * 		configured to CP_WR
 * \return value:
 * 		No return value
 */
void DIO_PinOn(te_DIO_Pins dio) 
{
	uint8_t controlType = 0u;

	controlType = HAL_TypeFromConfig(dio);

	if( (controlType == CP_R) ) 
    {
		HAL_WritePin(dio, CP_HIGH);
	}
	if( (controlType == CP_I) ) 
    {
		HAL_WritePin(dio, CP_LOW);
	}
}

/**
 * void DIO_PinOff(te_DIO_Pins dio)
 * \brief: 
 * 		Disables digital pin	 
 * \param[in]:	dio
 * 		A digital pin to be enabled
 * \description: 
 * 		This function disabled digital pin, using configured
 * 		control type. Can be used only on digital pin,
 * 		configured to CP_WR
 * \return value:
 * 		No return value
 */
void DIO_PinOff(te_DIO_Pins dio) 
{
	uint8_t controlType = 0u;

	controlType = HAL_TypeFromConfig(dio);

	if( (controlType == CP_R) ) 
    {
		HAL_WritePin(dio, CP_LOW);
	}
	if( (controlType == CP_I) ) 
    {
		HAL_WritePin(dio, CP_HIGH);
	}
}

/**
 * void HAL_TogglePin(te_DIO_Pins dio)
 * \brief: 
 * 		Toggles digital pin	 
 * \param[in]:	dio
 * 		A digital pin to be toggle
 * \description: 
 * 		This function toggles the digital pin from CP_HIGH to
 *      CP_LOW or otherwise. If pin has been configured as CP_OUT,
 *      then toggles supply state. If as CP_IN, then toggles Pull-Up
 *      connection.
 * \return value:
 * 		No return value
 */
void DIO_TogglePin(te_DIO_Pins dio) 
{
    HAL_TogglePin(dio);
}

void DIO_WritePin(te_DIO_Pins dio, uint8_t state) 
{
	if(state == CP_OFF ) 
    {
		DIO_PinOn(dio);
	}else
    {
		DIO_PinOff(dio);
	}
}