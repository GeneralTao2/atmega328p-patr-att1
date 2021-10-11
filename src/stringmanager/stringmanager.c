#include "stringmanager.h"

#include "../signalgateway/signalgateway.h"
#include "../defines.h"
#include "../utils/utils.h"

#define STR_NUMBER_STRING_LENGTH 5u
#define STR_16_BIT_DIGIT_QUANTITY 5u

#define STR_BCD_DIGIT_MASK (uint32_t)15u
#define STR_BCD_DIGIT_STEP 4u
#define STR_BITS_IN_DEC 17
#define STR_BITS_IN_BCD 20

#define STR_MAX_NUMBER 99999

#define STR_HALF_BYTE_MASK 15u
#define STR_HALF_BYTE_STEP 4u

/* https://my.eng.utah.edu/~nmcdonal/Tutorials/BCDTutorial/BCDConversion.html */
uint32_t STR_16bitDecToBCD(uint32_t dec) 
{
    /* The maximum value of uint16_t is 65535, so we need up to 5
       digits to represent uint16_t value in BCD */
    /* In BCD each digit takes 4 bits, so to store 20 bit we need uint32_t */
    uint32_t retVal = 0u;

    /* In the retVal variable these digit look like:
        For dec = 1234
                        1    2    3    4    5
        0b000000000000 0001 0010 0011 0100 0101 
    */

    int8_t idx = 0u;
    uint8_t digitIdx = 0u;
    
    for(idx = STR_BITS_IN_DEC - 1; idx >= 0; idx--) 
    {
        /* If some digit is greater then 6, we need to add 3 to digit,
           so we prevent it in advance digit value overflou */
        for(digitIdx = 0u; digitIdx < STR_BITS_IN_BCD - STR_BCD_DIGIT_STEP; digitIdx += STR_BCD_DIGIT_STEP)
        {
            if( (retVal & (STR_BCD_DIGIT_MASK << digitIdx ) ) >= (5 << digitIdx)) 
            {
                retVal += (3 << digitIdx);
            }
        }
        /*
        In the expended form cycle from above looks like:
        if( (retVal & 0b00000000000000000000000000001111) >= 0b00000000000000000000000000000101) {
            retVal += 0b00000000000000000000000000000011;
        }
        if( (retVal & 0b00000000000000000000000011110000) >= 0b00000000000000000000000001010000) {
            retVal += 0b00000000000000000000000000110000;
        }
        ....
        */

        /* Here we just shift retVal and add next dec bit to the end */
        retVal <<= 1;
        if( (dec & ( (uint32_t)1 << idx ) ) != 0)
        {
            retVal++;
        }

    }
    return retVal;
}

void STR_WriteStringToLCD(const uint8_t lineId, const uint8_t position, const uint8_t length, const char *str)
{
    uint8_t *LCD_string = 0;
    uint8_t newPosition = position;
    LCD_string = GW_Get_LCD_String();
    if(lineId == LCD_LINE_2) 
    {
        newPosition += LCD_LINE_LENGTH;
    }
    STR_WriteStringToArray(str, (char*)LCD_string, newPosition, length);
}

void STR_WriteNumberToLCD(const uint8_t lineId, const uint8_t position, const uint8_t length, const uint8_t alignment, const uint8_t filling, const uint32_t number)
{
    uint8_t *LCD_string = 0;
    uint8_t newPosition = position;
    LCD_string = GW_Get_LCD_String();
    if(lineId == LCD_LINE_2) 
    {
        newPosition += LCD_LINE_LENGTH;
    }
    STR_WriteNumberToArray((char*)LCD_string, newPosition, length, alignment, filling, number);
}

void STR_WriteStringToArray(const char src[], char dst[], const uint8_t position, const uint8_t length) 
{
    uint8_t idx = 0u;
    uint8_t absIdx = idx + position;
    for(idx=0; idx < length; idx++)
    {
        absIdx = idx + position;
        dst[absIdx] = src[idx];
    }
}

void STR_NumberToString(char *str, const uint32_t number) 
{
    uint8_t zeroChar = 0u;
    int8_t idx = 0u;
    uint8_t digitIdx = 0u;
    uint32_t BCDvalue = 0u;
    uint32_t newNumber = number;

    zeroChar = (uint8_t)'0';
    if(number > STR_MAX_NUMBER)
    {
        newNumber = newNumber - (STR_MAX_NUMBER + 1) ;
    }
    BCDvalue = STR_16bitDecToBCD(newNumber);

    for(idx = STR_16_BIT_DIGIT_QUANTITY - 1; idx >= 0; idx--)
    {
        str[idx] = ( (uint32_t)( BCDvalue & (STR_BCD_DIGIT_MASK << digitIdx ) ) >> digitIdx ) + zeroChar;
        digitIdx += STR_BCD_DIGIT_STEP;
    }

}


void STR_WriteNumberToArray(char dst[], const uint8_t position, const uint8_t length, const uint8_t alignment, const uint8_t filling, const uint32_t number) 
{
    char tmpNumberString[STR_NUMBER_STRING_LENGTH] = {0};
    uint8_t zerosStrLength = 0u;
    uint8_t idx = 0u;
    uint8_t numberStrLength = 0u;
    uint8_t modifiedPosition = position; 
    uint8_t modifiedLength = length;

    /* Converting number to string, so we get something like 00XXX */
    STR_NumberToString(tmpNumberString, number);

    /* Next we need to costumize our raw string */
    /* To do this we firstly need to calculate lengths of zeros-filled and number-filled 
       parts of string */
    /* Count zero ('0') charcaters in beginning of received string 
       in case if number is equal to 0, we need to let one zero character for number-filled part */
    for(idx = 0u; (tmpNumberString[idx] == ( (uint8_t)'0') ) && (idx < (STR_NUMBER_STRING_LENGTH-1) ); idx++)
    {
        zerosStrLength++;
    }

    /* Knowing length of zero-filled part, we can calculate number-filled part */
    numberStrLength = STR_NUMBER_STRING_LENGTH - zerosStrLength;

    /* Next begin costumizing */
    /*todo*/
    if(alignment == STR_ALIGNMENT_LEFT) 
    {
        /* If we got STR_ALIGNMENT_LEFT and STR_FILLING_SPACES */
        /*todo*/
        if(filling == STR_FILLING_SPACES)
        {
            /* Then we copy number-part to the beginnig of string */
            for(idx = 0u; idx < numberStrLength; idx++) 
            {
                tmpNumberString[idx] = tmpNumberString[idx + zerosStrLength];
            }
            /* And fill unnesecessary spase from the left with chararacters ' ' */
            for(idx = numberStrLength; idx < STR_NUMBER_STRING_LENGTH; idx++) 
            {
                tmpNumberString[idx] = (uint8_t)' ';
            }
        } else 
        /* If we got STR_ALIGNMENT_LEFT and STR_FILLING_ZEROS */
        if(filling == STR_FILLING_ZEROS)
        {
            /* To fill empty spase from the rigth with zeros is not good idea 
               so we don't do anything */
        } else 
        /* If we got STR_ALIGNMENT_LEFT and STR_FILLING_NONE */
        if(filling == STR_FILLING_NONE)
        {
            /* Then we need also copy number-part to the beginnig of string */
            for(idx = 0u; idx < numberStrLength; idx++) 
            {
                tmpNumberString[idx] = tmpNumberString[idx + zerosStrLength];
            }
            /* And just cut off unnesecessary space */
            modifiedLength = numberStrLength;
            
        } else {
            /* Nothing to do */
        }
        
    } else
    if(alignment == STR_ALIGNMENT_RIGHT)
    {
        /* If we got STR_ALIGNMENT_RIGHT and STR_FILLING_SPACES */
        if(filling == STR_FILLING_SPACES)
        {
            /* Then we just need to fill unnesecessary space with ' ' */
            for(idx = 0u; idx < zerosStrLength; idx++) 
            {
                 tmpNumberString[idx] = (uint8_t)' ';
            }
        } else 
        /* If we got STR_ALIGNMENT_RIGHT and STR_FILLING_ZEROS */
        if(filling == STR_FILLING_ZEROS)
        {
            /* String is already styled with required reles
               so we don't do anything */
        } else 
        /* If we got STR_ALIGNMENT_RIGHT and STR_FILLING_NONE */
        if(filling == STR_FILLING_NONE)
        {
            /* Then we need also copy number-part to the beginnig of string */
            for(idx = 0u; idx < numberStrLength; idx++) 
            {
                tmpNumberString[idx] = tmpNumberString[idx + zerosStrLength];
            }
            /* Shift string a bit to the rigth */
            modifiedPosition = position + zerosStrLength;
            /* And cut off unnesecessary space */
            modifiedLength = numberStrLength;
            
        } else 
        {
            /* Nothing to do */
        }
    } else 
    {
        /* Nothing to do */
    }

    STR_WriteStringToArray(tmpNumberString, dst, modifiedPosition, modifiedLength);
}

uint8_t STR_CharToHexDigit(const int8_t charact, uint8_t *error)
{
    uint8_t retVal = 0u;

    if( (charact >= '0') && (charact <= '9') )
    {
        retVal = charact - '0';
    } else
    if( (charact >= 'A') && (charact <= 'F') )
    {
        retVal = (charact - 'A') + 10;
    } else
    {
        (*error) = ERR_STR_WRONG_CHARACTER;
    }

    return retVal;
}

int8_t STR_HexDigitToChar(const uint8_t hex, uint8_t *error)
{
    int8_t retVal = 0u;

    if( (hex >= 0x00) && (hex <= 0x09) )
    {
        retVal = hex + '0';
    } else
    if( (hex >= 0x0A) && (hex <= 0x0F) )
    {
        retVal = (hex - 0x0A) + 'A';
    } else
    {
        retVal = 0u;
        (*error) = ERR_STR_WRONG_HEX_DIGIT;
    }

    return retVal;
}

uint8_t STR_StringTo8BitHex(const uint8_t src[], uint8_t *error)
{
    uint8_t idx = 0u;
    uint8_t retVal = 0u;

    for(idx = 0; idx < STR_8BIT_STRING_LENGTH; idx++)
    {
        retVal |= STR_CharToHexDigit( (uint8_t)src[idx], error);

        if(idx < (STR_8BIT_STRING_LENGTH - 1) )
        {
            retVal <<= 4u; 
        }
    }
    if((*error) != ERR_NO_ERROR)
    {
        retVal = 0u;
    }

    return retVal;
}

uint16_t STR_StringTo16BitHex(const uint8_t src[], uint8_t *error)
{
    uint8_t idx = 0u;
    uint16_t retVal = 0u;

    for(idx = 0; idx < STR_8BIT_STRING_LENGTH*2; idx++)
    {
        retVal |= STR_CharToHexDigit( (uint8_t)src[idx], error);

        if(idx < ( (STR_8BIT_STRING_LENGTH*2) - 1) )
        {
            retVal <<= 4u; 
        }
    }
    if((*error) != ERR_NO_ERROR)
    {
        retVal = 0u;
    }

    return retVal;
}

void STR_8BitHexToString(uint8_t dst[], const uint8_t hex)
{
    int8_t tmpArrIdx = 0u;
    uint8_t byteStepIdx = 0u;
    uint8_t digit = 0u;
    uint8_t tmpArr[STR_8BIT_STRING_LENGTH] = {0};
    /* Here this error variable plays role of placeholder,
       we are shure that hex is valid, so no need to 
       handle errors */
    uint8_t error = ERR_NO_ERROR;

    for(tmpArrIdx = (int8_t)STR_8BIT_STRING_LENGTH - 1; tmpArrIdx >= 0; tmpArrIdx--)
    {
        digit = (hex >> byteStepIdx) & STR_HALF_BYTE_MASK;

        tmpArr[tmpArrIdx] = STR_HexDigitToChar(digit, &error);
        byteStepIdx += STR_HALF_BYTE_STEP;
    }

    if(error == ERR_NO_ERROR)
    {
        U_ArrCpy(dst, tmpArr, STR_8BIT_STRING_LENGTH);   
    }
}