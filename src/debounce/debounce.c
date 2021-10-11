#include "debounce.h"
#include "utils/utils.h"
#include "dio/dio.h"


#define DBC_PORT_OFF 0
#define DBC_PORT_ON 1

volatile ts_DBC_PortConfigs DBC_Ports[PORT_QUANTITY] = {
    {DBC_PORT_OFF, 0x00, {0xFF, 0xFF}, 0x00},
    {DBC_PORT_OFF, 0x00, {0xFF, 0xFF}, 0x00},
    {DBC_PORT_OFF, 0x00, {0xFF, 0xFF}, 0x00},
    {DBC_PORT_OFF, 0x00, {0xFF, 0xFF}, 0x00}
};

/* Return non-zero if a button matching mask is pressed. */
uint8_t DBC_ReadDebouncedPin(uint8_t port, uint8_t pin)
{
    uint8_t buttonMask = 0x00;
    uint8_t retVal = CP_OFF;
    buttonMask = U_bitMasks[pin];
#ifdef DEBOUCE_IN_ISR
    /* ATOMIC_BLOCK is needed if debounce() is called from within an ISR */
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        /* And with debounced state for a one if they match */
        buttonMask &= DBC_ButtonsDown;
        /* Clear if there was a match */
        DBC_ButtonsDown ^= buttonMask;
    }
#else
    /* And with debounced state for a one if they match */
    buttonMask &= DBC_Ports[port].buttonsDown;
    /* Clear if there was a match */
    DBC_Ports[port].buttonsDown ^= buttonMask;
#endif
    if(buttonMask) {
        retVal = CP_ON;
    } else 
    {
        retVal = CP_OFF;
    }
    return retVal;
}

void DBC_InitPortDebouncing(uint8_t port) 
{
    DBC_Ports[port].enablingState = DBC_PORT_ON;
}

/* Check button state and set bits in the DBC_ReadDebouncedPin variable if a
   debounced button down press is detected. */
void DBC_DebouncePort(uint8_t portIdx) 
{
    uint8_t PIN_Reg = 0x00;

    PIN_Reg = HL_ReadPIN(portIdx);

    /* Read buttons (active low so invert with ~). Xor with
       button_state to see which ones are about to change state */
    uint8_t state_changed = ~PIN_Reg ^ DBC_Ports[portIdx].button_state;
    /* Decrease counters where state_changed = 1, set the others to 0b11. */
    VC_DEC_OR_SET(DBC_Ports[portIdx].vcounter, state_changed);
    /* Update state_changed to have a 1 only if the counter overflowed */
    state_changed &= DBC_Ports[portIdx].vcounter.highBit & DBC_Ports[portIdx].vcounter.lowBit;
    /* Change button_state for the buttons who’s counters rolled over */
    DBC_Ports[portIdx].button_state ^= state_changed;
    /* Update DBC_ReadDebouncedPin with buttons who’s counters rolled over */
    /* and who’s state is 1 (pressed) */
    DBC_Ports[portIdx].buttonsDown |= DBC_Ports[portIdx].button_state & state_changed;
}

void DBC_Debounce(void) 
{
    uint8_t idx = CP_START_PORT;
    for(idx = CP_START_PORT; idx<PORT_QUANTITY; idx++)
    {
        if(DBC_Ports[idx].enablingState == DBC_PORT_ON) 
        {
            DBC_DebouncePort(idx);
        }
    }
}