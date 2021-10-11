#include "utils.h"
#include "../defines.h"
/**
 * \def: const uint8_t U_bitMasks[BITS_IN_ONE_BYTE]
 * \brief: The array of bit masks with logic one bits in position
 *      from 1 up to 8
 */
const uint8_t U_bitMasks[BITS_IN_ONE_BYTE] = {
    SHFT_BIT(0),
    SHFT_BIT(1),
    SHFT_BIT(2),
    SHFT_BIT(3),
    SHFT_BIT(4),
    SHFT_BIT(5),
    SHFT_BIT(6),
    SHFT_BIT(7),
};

/**
 * void U_BitToConfig(uint8_t *configsCell, uint8_t position, uint8_t state1, uint8_t state2, uint8_t state)
 * \brief: 
 * 		Writes a logic value to bit of variable	 
 * \param[in]:	*configsCell
 * 		The pointer to a variable, bit of that to be written 
 *              position
 * 		Position of bit to be written
 *              state1
 * 		State to be encripted to logic zero
 *              state2
 * 		State to be encripted to logic one
 *              state
 * 		State to be encrypted and writed
 * \description: 
 * 		This function encrypts binary states to logic zero and one and writes them to variable bit
 * \return value:
 * 		No return value
 */
void U_BitToConfig(uint8_t *configsCell, uint8_t position, uint8_t state1, uint8_t state2, uint8_t state) {
    /* 0 = state1, 1 = state2 */
	if(state == state1) 
    {
	 	*configsCell &= ~U_bitMasks[position];
	} else 
    if(state == state2) 
    {
	 	*configsCell |= U_bitMasks[position];
	} else 
    {
        /* Nothing to do */
    }
}

/**
 * uint8_t U_BitFromConfig(uint8_t *configsCell, uint8_t position, uint8_t state1, uint8_t state2)
 * \brief: 
 * 		Read a logic value from bit of variable	 
 * \param[in]:	*configsCell
 * 		The pointer to a variable, bit of that to be read 
 *              position
 * 		Position of bit to be read
 *              state1
 * 		State to be encripted to logic zero
 *              state2
* 		State to be encripted to logic one
 * \description: 
 * 		This function decrypts binary states from logic zero and one and reads them from 
 *      the variable bit
 * \return value:
 * 		Will return state1 or state2
 */
uint8_t U_BitFromConfig(uint8_t *configsCell, uint8_t position, uint8_t state1, uint8_t state2) {
	uint8_t retVal = 0u;
	/* 0 = state1, 1 = state2 */
	if( ( ( *configsCell ) & U_bitMasks[position] ) == 0 ) 
    {
		retVal = state1;
	} else 
    {
		retVal = state2;
	}
	return retVal;
}

/**
 * void U_BitSeqToConfig(uint8_t *configsCell, uint8_t bitSeqPosition, uint8_t bitSeqMask, uint8_t bitSeq)
 * \brief: 
 * 		Writes a value to a bit sequence of variable 
 * \param[in]:	*configsCell
 * 		The pointer to a variable, bit of that to be written 
 *              bitSeqPosition
 * 		Position of bit sequence beginning to be written
 *              bitSeqMask
 * 		A bit mask of bit sequence place, where bitSeq will be written
 *              bitSeq
 * 		A value to be written to bit sequence of variable
 * \description: 
 * 		This function writes a bit sequence bitSeq to plase, masked by bitSeqMask in a variable *configsCell
 * \return value:
 * 		No return value
 */
void U_BitSeqToConfig(uint8_t *configsCell, uint8_t bitSeqPosition, uint8_t bitSeqMask, uint8_t bitSeq) {
    *configsCell &= ~bitSeqMask;
 	*configsCell |= ( (bitSeq << bitSeqPosition) & bitSeqMask );
}

/**
 * uint8_t U_BitSeqFromConfig(uint8_t *configsCell, uint8_t bitSeqPosition, uint8_t bitSeqMask)
 * \brief: 
 * 		Writes a value to a bit sequence of variable 
 * \param[in]:	*configsCell
 * 		The pointer to a variable, bit of that to be written 
 *              bitSeqPosition
 * 		Position of bit sequence beginning to be written
 *              bitSeqMask
 * 		A bit mask of bit sequence place, where bitSeq will be written
 *              bitSeq
 * 		A value to be written to bit sequence of variable
 * \description: 
 * 		This function writes a bit sequence bitSeq to plase, masked by bitSeqMask in a variable *configsCell
 * \return value:
 * 		Will return piece of variable *configsCell, matched by mask bitSeqMask
 */
uint8_t U_BitSeqFromConfig(uint8_t *configsCell, uint8_t bitSeqPosition, uint8_t bitSeqMask) {
	uint8_t retVal = 0u;
	retVal = ( *configsCell & bitSeqMask ) >>  bitSeqPosition;
	return retVal;
}

int8_t U_ArrCmp(const uint8_t arrA[], const uint8_t arrB[], const uint8_t length)
{
    uint8_t idx = 0u;
    int8_t difference = 0u;

    for(idx = 0; idx < length; idx++)
    {
        if(arrA[idx] != arrB[idx])
        {
            difference = arrA[idx] - arrB[idx];
        }    
    }
    return difference;
}

void U_ArrCpy(uint8_t dst[], const uint8_t src[], const uint8_t length)
{
    uint8_t idx = 0u;

    for(idx = 0; idx < length; idx++)
    {
        dst[idx] = src[idx];
    }
}

int32_t U_Map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) 
{
    int32_t retVal = 0u;
    retVal = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return retVal;
}