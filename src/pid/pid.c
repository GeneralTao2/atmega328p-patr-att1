#include "pid.h"

#include "../pwm/pwm.h"
#include "../motorencoder/motorencoder.h"
#include "../adc/adc.h"
#include "../signalgateway/signalgateway.h"
#include "../stringmanager/stringmanager.h"

#define KP 1
#define KD 1
#define KI 1


uint8_t PID_Calculate(int32_t setPintRPM, int32_t actualRPM)
{
    int32_t err=0;
    int32_t priveousErr=0;
    int32_t pid_i=0;
    int32_t pid_p=0;
    int32_t pid_d=0;
    int32_t retVal=0;

    
    err = setPintRPM - actualRPM;
    pid_p = KP * err;
    pid_d = KD * (err - priveousErr)/3;
    pid_i = KI * (pid_i + err)/2;
    priveousErr = err;
    retVal = pid_p + pid_d + pid_i;

    if(retVal < 0) 
    {
        retVal = 0;
    } else 
    if(retVal >= 255) {
        retVal = 255;
    }
    
    return retVal;
}
