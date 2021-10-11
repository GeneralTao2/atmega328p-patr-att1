#include "lcddisplay.h"

#include <util/delay.h>

//#define LCD_8_BIT_MODE
#define LCD_4_BIT_MODE

#define LCD_SET_ENABLE_SIGNAL DIO_PinOn(LCD_E)
#define LCD_CLEAR_ENABLE_SIGNAL DIO_PinOff(LCD_E)

#define LCD_SET_REGISTER_SELECT_SIGNAL DIO_PinOn(LCD_RS)
#define LCD_CLEAR_REGISTER_SELECT_SIGNAL DIO_PinOff(LCD_RS)

#define LCD_WRITE_MODE DIO_PinOn(LCD_RW)
#define LCD_READ_MODE DIO_PinOff(LCD_RW)

#define LCD_SET_BACKLIGHT DIO_PinOn(LCD_BL)
#define LCD_CLEAR_BACKLIGHT DIO_PinOff(LCD_BL)

/**
 * \def: LCD_CLEAR_DISPAY_CMD
 * \brief: 
 */
#define LCD_CLEAR_DISPAY_CMD 0x01 

#ifdef LCD_4_BIT_MODE
/**
 * \def: LCD_FUNCTION_SET_CMD
 * \brief: DL - 0, N - 1, F - 0
 */
#define LCD_FUNCTION_SET_CMD 0x28  
#define LCD_RESET_FUNCTION_SET_STEP_1_CMD 0x03 
#define LCD_RESET_FUNCTION_SET_STEP_2_CMD 0x02 
#else
/**
 * \def: LCD_FUNCTION_SET_CMD
 * \brief: DL - 1, N - 1, F - 0
 */
#define LCD_FUNCTION_SET_CMD 0x38  
#endif

/**
 * \def: LCD_DISPAY_CONTROL_CMD
 * \brief: D - 1, C - 1, B - 0
 */
#define LCD_DISPAY_CONTROL_CMD 0x0C 

#define LCD_RETURN_HOME_CMD 0x02 

#ifdef LCD_4_BIT_MODE
#define LCD_INTERFACE_DATA_LENTH 4
#else
#define LCD_INTERFACE_DATA_LENTH 8
#endif

#define LCD_BEGINNING_OF_LINE_1_ADRESS 0x80
#define LCD_BEGINNING_OF_LINE_2_ADRESS 0xC0

#define LCD_STOP_POINT 0u
#define LCD_START_POINT 1u
#define LCD_INIT_POINT 2u

#ifdef LCD_4_BIT_MODE
#define LCD_FUNCTION_SET_CONFIG_HIGH_POINT 3u
#define LCD_FUNCTION_SET_CONFIG_LOW_POINT 4u
#define LCD_DISPLAY_CONTOL_CONFIG_HIGH_POINT 5u
#define LCD_DISPLAY_CONTOL_CONFIG_LOW_POINT 6u
#define LCD_CHARACTER_WRITING_HIGH_POINT 7u
#define LCD_CHARACTER_WRITING_LOW_POINT 8u
#define LCD_SET_CURSOR_POSITION_HIGH_POINT 9u
#define LCD_SET_CURSOR_POSITION_LOW_POINT 10u
#define LCD_CLEAR_DISPAY_HIGH_POINT 11u
#define LCD_CLEAR_DISPAY_LOW_POINT 12u
#define LCD_RETURN_HOME_HIGH_POINT 13u
#define LCD_RETURN_HOME_LOW_POINT 14u
#define LCD_RESET_FUCTION_SET_STEP_1_POINT 15u
#define LCD_RESET_FUCTION_SET_STEP_2_POINT 16u
#else
#define LCD_FUNCTION_SET_CONFIG_POINT 3u
#define LCD_DISPLAY_CONTOL_CONFIG_POINT 4u
#define LCD_CHARACTER_WRITING_POINT 5u
#define LCD_SET_CURSOR_POSITION_POINT 6u
#define LCD_CLEAR_DISPAY_POINT 7u
#endif


#define LCD_CLEAR_E_SIGNAL_POINT 200u
#define LCD_DELAY_POINT 201u


#ifdef LCD_4_BIT_MODE
#define LCD_LOW_HALF_WRITED 0
#define LCD_HIGH_HALF_WRITED 1
#endif

uint8_t LCD_DataDIOs[LCD_INTERFACE_DATA_LENTH] = {
#ifdef LCD_8_BIT_MODE
    LCD_D0,
    LCD_D1,
    LCD_D2,
    LCD_D3,
#endif  
    LCD_D4,
    LCD_D5,
    LCD_D6,
    LCD_D7
};

//uint8_t LCD_currentCharacters[LCD_CURRENT_CHARACTERS_QUANTITY] = "Hello, World!";

uint16_t LCD_currentPoint = LCD_START_POINT;

uint8_t LCD_nextPoint = LCD_STOP_POINT;

int16_t LCD_delayCounter = 0u;

uint8_t LCD_currentCharIndex = 0u;

uint8_t LCD_cursorPosition = 0u;

uint8_t LCD_currentLine = LCD_LINE_1;

uint8_t LCD_tmpDataBuffer = 0u;

#ifdef LCD_4_BIT_MODE
uint8_t LCD_tmpNextPoint = LCD_STOP_POINT;
int8_t LCD_resetFunctionSetPart1Counter = 3u;
#endif



void LCD_Init(void) 
{
#ifdef LCD_8_BIT_MODE
    DIO_ConfigurePin(LCD_D0,    CP_B, CP_0, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_D1,    CP_B, CP_1, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_D2,    CP_B, CP_2, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_D3,    CP_B, CP_3, CP_R, CP_OFF, CP_WR);
#endif
    DIO_ConfigurePin(LCD_D4,    CP_B, CP_2, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_D5,    CP_B, CP_3, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_D6,    CP_B, CP_4, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_D7,    CP_B, CP_5, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_RS,    CP_C, CP_3, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_E,     CP_C, CP_4, CP_R, CP_OFF, CP_WR);
    DIO_ConfigurePin(LCD_BL,    CP_C, CP_5, CP_R, CP_OFF, CP_WR);

    LCD_FillCurrentCharacters();

}

void LCD_FillCurrentCharacters(void) 
{
    uint8_t idx = 0u;
    uint8_t *tmpString = 0u;
    tmpString = GW_Get_LCD_String();
    for(idx=0; idx<LCD_CURRENT_CHARACTERS_QUANTITY; idx++) {
        tmpString[idx] = (uint8_t)' ';
    }
}

void LCD_WriteData(uint8_t data) 
{
    uint8_t idx = 0u;
    for(idx=0; idx<LCD_INTERFACE_DATA_LENTH; idx++)
    {
        if( (data & U_bitMasks[idx] ) == 0 ) 
        {
            DIO_PinOff( LCD_DataDIOs[idx] );
        } else 
        {
            DIO_PinOn( LCD_DataDIOs[idx] );
        }
    }
}


void LCD_SendCommand(uint8_t command)
{
    LCD_CLEAR_REGISTER_SELECT_SIGNAL;
    LCD_WriteData(command >> 4);
    LCD_SET_ENABLE_SIGNAL;
    _delay_ms(2);
    LCD_CLEAR_ENABLE_SIGNAL;
    _delay_ms(2);
    LCD_WriteData(command);
    LCD_SET_ENABLE_SIGNAL;
    _delay_ms(2);
    LCD_CLEAR_ENABLE_SIGNAL;
    _delay_ms(5);
    LCD_WriteData(0u);
}

void LCD_SendChar(uint8_t charact)
{
    LCD_SET_REGISTER_SELECT_SIGNAL;
    LCD_WriteData(charact >> 4);
    LCD_SET_ENABLE_SIGNAL;
    _delay_ms(2);
    LCD_CLEAR_ENABLE_SIGNAL;
    _delay_ms(2);
    LCD_WriteData(charact);
    LCD_SET_ENABLE_SIGNAL;
    _delay_ms(2);
    LCD_CLEAR_ENABLE_SIGNAL;
    _delay_ms(5);
    LCD_WriteData(0u);
}

void LCD_Run(void) 
{
    uint8_t *tmpString = 0;

    tmpString = GW_Get_LCD_String();

    #ifdef LCD_4_BIT_MODE

    switch (LCD_currentPoint)
    {
    case LCD_START_POINT:
        LCD_nextPoint = LCD_INIT_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 35u;
        break;
    case LCD_INIT_POINT:
        /* Clearing data and signal pins */
        LCD_WriteData(0u);  
        LCD_CLEAR_ENABLE_SIGNAL;
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        /* Set backlight */
        LCD_SET_BACKLIGHT;
        /* All our actions need write mode */
        LCD_WRITE_MODE;

        LCD_nextPoint = LCD_RESET_FUCTION_SET_STEP_1_POINT;
        LCD_currentPoint = LCD_CLEAR_DISPAY_HIGH_POINT;
        break;
    case LCD_RESET_FUCTION_SET_STEP_1_POINT:
        /* Here begin to send RESET sequence */
        /* We need to do reset becouse of probabilty of losing 
           E signal sincronization after AVR hardware reset */
        /* All we need to is send 0x03 0x03 0x03 0x03 0x02 sequence,
           according to datasheet (page 47)
           http://www.datasheetcatalog.com/datasheets_pdf/H/D/4/4/HD44780U.shtml */
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_RESET_FUNCTION_SET_STEP_1_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;

        if(LCD_resetFunctionSetPart1Counter <= 0) 
        {
            LCD_nextPoint = LCD_RESET_FUCTION_SET_STEP_2_POINT;
        } else
        {
            LCD_nextPoint = LCD_RESET_FUCTION_SET_STEP_1_POINT;
        }
        LCD_resetFunctionSetPart1Counter--;

        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 5u;
        break;
    case LCD_RESET_FUCTION_SET_STEP_2_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_RESET_FUNCTION_SET_STEP_2_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        /* Here RESET sequence ends and we move to regular initialization */
        LCD_nextPoint = LCD_RETURN_HOME_HIGH_POINT;
        LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        break;
    case LCD_RETURN_HOME_HIGH_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_RETURN_HOME_CMD >> HALF_OF_BYTE_LENTH); // <----
        LCD_SET_ENABLE_SIGNAL;

        LCD_nextPoint = LCD_RETURN_HOME_LOW_POINT;
        LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        break;
    case LCD_RETURN_HOME_LOW_POINT:
        LCD_WriteData(LCD_RETURN_HOME_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_FUNCTION_SET_CONFIG_HIGH_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 2u;
        break;
    case LCD_FUNCTION_SET_CONFIG_HIGH_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_FUNCTION_SET_CMD >> HALF_OF_BYTE_LENTH); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_FUNCTION_SET_CONFIG_LOW_POINT;
        LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        break;
    case LCD_FUNCTION_SET_CONFIG_LOW_POINT:
        LCD_WriteData(LCD_FUNCTION_SET_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_DISPLAY_CONTOL_CONFIG_HIGH_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 1u;
        break;
    case LCD_DISPLAY_CONTOL_CONFIG_HIGH_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_DISPAY_CONTROL_CMD >> HALF_OF_BYTE_LENTH); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_DISPLAY_CONTOL_CONFIG_LOW_POINT;
        LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        break;
    case LCD_DISPLAY_CONTOL_CONFIG_LOW_POINT:
        LCD_WriteData(LCD_DISPAY_CONTROL_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_CHARACTER_WRITING_HIGH_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 1u;
        break;
    case LCD_CHARACTER_WRITING_HIGH_POINT:
        LCD_SET_REGISTER_SELECT_SIGNAL;
        
        /* If writing to line 1 is compleate */
        if( (LCD_currentCharIndex >= LCD_LINE_LENGTH) && (LCD_currentLine == LCD_LINE_1) ) 
        {
            
            /* Then we move corsor to position LCD_LINE_LENGTH (16, first position of line 2) */
            LCD_cursorPosition = LCD_LINE_LENGTH;
            LCD_currentPoint = LCD_SET_CURSOR_POSITION_HIGH_POINT;
        } else 
        /* Or, if writing to line 1 and 2 is compleate */
        if(LCD_currentCharIndex >= LCD_CURRENT_CHARACTERS_QUANTITY) 
        {
            /* Then we move cursor to position 0 (first position of line 1) */
            LCD_nextPoint = LCD_CHARACTER_WRITING_HIGH_POINT;
            LCD_currentPoint = LCD_SET_CURSOR_POSITION_HIGH_POINT;
            LCD_currentCharIndex = 0;
            LCD_cursorPosition = 0;
        } else 
        /* Any other case means, that we still fill lines */
        {
            LCD_tmpDataBuffer = tmpString[LCD_currentCharIndex];
            /*todo*/
            LCD_WriteData(LCD_tmpDataBuffer >> HALF_OF_BYTE_LENTH);
            LCD_SET_REGISTER_SELECT_SIGNAL;
            LCD_SET_ENABLE_SIGNAL;
           
            LCD_currentCharIndex++;
            LCD_nextPoint = LCD_CHARACTER_WRITING_LOW_POINT;
            LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        }
        break;
    case LCD_CHARACTER_WRITING_LOW_POINT:
        LCD_WriteData(LCD_tmpDataBuffer); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_CHARACTER_WRITING_HIGH_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 1u;
        break;
    /* Service cases */
    case LCD_DELAY_POINT:
        LCD_delayCounter--;
        if(LCD_delayCounter <= 0) 
        {
            LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        }
        break;  
    case LCD_CLEAR_DISPAY_HIGH_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_CLEAR_DISPAY_CMD >> HALF_OF_BYTE_LENTH); // <----
        LCD_SET_ENABLE_SIGNAL;
        LCD_currentCharIndex = 0;
        
        LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        LCD_tmpNextPoint = LCD_nextPoint;
        LCD_nextPoint = LCD_CLEAR_DISPAY_LOW_POINT;
        break;
    case LCD_CLEAR_DISPAY_LOW_POINT:
        LCD_WriteData(LCD_CLEAR_DISPAY_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_nextPoint = LCD_tmpNextPoint;
        LCD_delayCounter = 2u;
        break;
    case LCD_CLEAR_E_SIGNAL_POINT:
        LCD_CLEAR_ENABLE_SIGNAL;
        
        LCD_currentPoint = LCD_nextPoint;

        break;
    case LCD_SET_CURSOR_POSITION_HIGH_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        /* If cursor position locates on line 1 */
        if( LCD_cursorPosition < LCD_LINE_LENGTH ) 
        {
            LCD_tmpDataBuffer = LCD_BEGINNING_OF_LINE_1_ADRESS + LCD_cursorPosition;
            LCD_currentLine = LCD_LINE_1;
        } else 
        /* If cursor position locates on line 2 */
        {
            
            LCD_tmpDataBuffer = LCD_BEGINNING_OF_LINE_2_ADRESS + (LCD_cursorPosition - LCD_LINE_LENGTH);
            LCD_currentLine = LCD_LINE_2;
        }
        LCD_WriteData(LCD_tmpDataBuffer >> HALF_OF_BYTE_LENTH);
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_SET_CURSOR_POSITION_LOW_POINT;
        LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        break;
    case LCD_SET_CURSOR_POSITION_LOW_POINT:
        LCD_WriteData(LCD_tmpDataBuffer); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_nextPoint = LCD_CHARACTER_WRITING_HIGH_POINT;
        LCD_delayCounter = 2u;
        break;
    default:

        break;
    }

    #else

    switch (LCD_currentPoint)
    {
    case LCD_START_POINT:
        LCD_nextPoint = LCD_INIT_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 35u;
        break;
    case LCD_INIT_POINT:
        /* Clearing data and signal pins */
        LCD_WriteData(0u);  
        LCD_CLEAR_ENABLE_SIGNAL;
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        /* Set backlight */
        LCD_SET_BACKLIGHT;
        /* All our actions need write mode */
        LCD_WRITE_MODE;

        LCD_nextPoint = LCD_FUNCTION_SET_CONFIG_POINT;
        LCD_currentPoint = LCD_CLEAR_DISPAY_POINT;
        break;
    case LCD_FUNCTION_SET_CONFIG_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_FUNCTION_SET_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_DISPLAY_CONTOL_CONFIG_POINT;
        LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        LCD_delayCounter = 1u;
        break;
    case LCD_DISPLAY_CONTOL_CONFIG_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_DISPAY_CONTROL_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        
        LCD_nextPoint = LCD_CHARACTER_WRITING_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 1u;
        break;
    case LCD_CHARACTER_WRITING_POINT:
        LCD_SET_REGISTER_SELECT_SIGNAL;
        /* If writing to line 1 is compleate */
        if( (LCD_currentCharIndex >= LCD_LINE_LENGTH) && (LCD_currentLine == LCD_LINE_1) ) 
        {
            /* Then we move corsor to position LCD_LINE_LENGTH (16, first position of line 2) */
            LCD_cursorPosition = LCD_LINE_LENGTH;
            LCD_currentPoint = LCD_SET_CURSOR_POSITION_POINT;
        } else 
        /* Or, if writing to line 1 and 2 is compleate */
        if(LCD_currentCharIndex >= LCD_CURRENT_CHARACTERS_QUANTITY) 
        {
            /* Then we move cursor to position 0 (first position of line 1) */
            LCD_nextPoint = LCD_CHARACTER_WRITING_POINT;
            LCD_currentPoint = LCD_SET_CURSOR_POSITION_POINT;
            LCD_currentCharIndex = 0;
            LCD_cursorPosition = 0;
        } else 
        /* Any other case means, that we still fill lines */
        {
            LCD_tmpDataBuffer = tmpString[LCD_currentCharIndex];
            /*todo*/
            LCD_WriteData(LCD_tmpDataBuffer);
            LCD_SET_REGISTER_SELECT_SIGNAL;
            LCD_SET_ENABLE_SIGNAL;
            LCD_currentCharIndex++;
        
            LCD_nextPoint = LCD_CHARACTER_WRITING_POINT;
            LCD_currentPoint = LCD_DELAY_POINT;
            LCD_delayCounter = 1u;
        }
        break;
    /* Service cases */
    case LCD_DELAY_POINT:
        LCD_delayCounter--;
        if(LCD_delayCounter <= 0) 
        {
            LCD_currentPoint = LCD_CLEAR_E_SIGNAL_POINT;
        }
        break;  
    case LCD_CLEAR_DISPAY_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        LCD_WriteData(LCD_CLEAR_DISPAY_CMD); // <----
        LCD_SET_ENABLE_SIGNAL;
        LCD_currentCharIndex = 0;

        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 2u;
        break;
    case LCD_CLEAR_E_SIGNAL_POINT:
        LCD_CLEAR_ENABLE_SIGNAL;

        LCD_currentPoint = LCD_nextPoint;
        
        break;
    case LCD_SET_CURSOR_POSITION_POINT:
        LCD_CLEAR_REGISTER_SELECT_SIGNAL;
        /* If cursor position locates on line 1 */
        if( LCD_cursorPosition < LCD_LINE_LENGTH ) 
        {
            LCD_WriteData(LCD_BEGINNING_OF_LINE_1_ADRESS + LCD_cursorPosition);
            LCD_currentLine = LCD_LINE_1;
        } else 
        /* If cursor position locates on line 2 */
        {
            LCD_WriteData(LCD_BEGINNING_OF_LINE_2_ADRESS + (LCD_cursorPosition - LCD_LINE_LENGTH));
            LCD_currentLine = LCD_LINE_2;
        }
        LCD_SET_ENABLE_SIGNAL;
        LCD_nextPoint = LCD_CHARACTER_WRITING_POINT;
        LCD_currentPoint = LCD_DELAY_POINT;
        LCD_delayCounter = 1u;
        break;
    default:

        break;
    }
    #endif
}
