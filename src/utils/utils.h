#ifndef utils_h
#define utils_h

#include <avr/io.h>

/**
 * SHFT_BIT(bit)
 * \brief: 
 * 		Makes a bit mask 		 
 * \param[in]:	bit
 * 		Position of bit 
 * \description: 
 * 		This macro function makes a bit mask, consisting from logic zeros
 *      and logic in position bit
 * \return value:
 * 		Returns bit mask
 */
#define SHFT_BIT(bit) (1<<(bit))

/**
 * SET_BIT(port, bit)
 * \brief: 
 * 		Sets a bit in a variable		 
 * \param[in]:  port
 *      Variable, in which bit to be set	
 *              bit
 * 		Position of a bit to be set 
 * \description: 
 * 		This macro function sets a bit of a variable to logic one
 * \return value:
 * 		No return value
 */
#define SET_BIT(port, bit) ( (port) |= SHFT_BIT(bit) )

/**
 * CLR_BIT(port, bit)
 * \brief: 
 * 		Clears a bit in a variable		 
 * \param[in]:  port
 *      Variable, in which bit to be cleared	
 *              bit
 * 		Position of a bit to be cleared 
 * \description: 
 * 		This macro function resets a bit of a variable to logic zero
 * \return value:
 * 		No return value
 */
#define CLR_BIT(port, bit) ( (port) &= ~SHFT_BIT(bit) )

/**
 * READ_BIT(port, bit)
 * \brief: 
 * 		Reads a bit in a variable		 
 * \param[in]:  port
 *      Variable, in which bit to be read	
 *              bit
 * 		Position of a bit to be read 
 * \description: 
 * 		This macro reads a bit in a variable 
 * \return value:
 * 		WIll return 0, if bit is in logic zero or 1 - if in logic one
 */
#define READ_BIT(port, bit) ( (port) & SHFT_BIT(bit) )

/**
 * CLR_BIT(port, bit)
 * \brief: 
 * 		Toggles bit in a variable		 
 * \param[in]:  port
 *      Variable, in which bit to be toggled	
 *              bit
 * 		Position of a bit to be toggled 
 * \description: 
 * 		This macro function sets a logic zero to logic one or
 *      otherwise
 * \return value:
 * 		No return value
 */
#define TOGG_BIT(port, bit) ( (port) ^= SHFT_BIT(bit) )

/**
 * \def: BITS_IN_ONE_BYTE
 * \brief: The quantity of bits in one byte
 */
#define BITS_IN_ONE_BYTE 8

extern const uint8_t U_bitMasks[BITS_IN_ONE_BYTE];

extern void U_BitToConfig(uint8_t *configsCell, uint8_t position, uint8_t state1, uint8_t state2, uint8_t state);
extern uint8_t U_BitFromConfig(uint8_t *configsCell, uint8_t position, uint8_t state1, uint8_t state2);

extern void U_BitSeqToConfig(uint8_t *configsCell, uint8_t bitSeqPosition, uint8_t bitSeqMask, uint8_t bitSeq);
extern uint8_t U_BitSeqFromConfig(uint8_t *configsCell, uint8_t bitSeqPosition, uint8_t bitSeqMask);

extern int8_t U_ArrCmp(const uint8_t arrA[], const uint8_t arrB[], const uint8_t length);
extern void U_ArrCpy(uint8_t dst[], const uint8_t src[], const uint8_t length);

extern int32_t U_Map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

#endif