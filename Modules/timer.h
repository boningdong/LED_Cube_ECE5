#include <Arduino.h>
#ifdef CUBE_TIMER
    #define CUBE_TIMER

void TimerInit();

#endif
/*
* Timer0 initializing
*/
void TimerInit()
{
    /*
    * Initiate the timer for a fix interval
    */
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCCR1B |= (1 << WGM12) | (CS11 << 1);
    OCR1A = 3500;
}
