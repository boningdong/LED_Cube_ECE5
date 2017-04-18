#include <Arduino.h>
#ifndef CUBE_INT_CONFIG
    #define CUBE_INT_CONFIG

void Global_IntInit();
void Timer1_IntOn();
void Timer1_IntOff();

#endif
 
void Global_IntInit()
{
    sei(); 
}

void Timer1_IntOn()
{
    TIMSK1 |= (1 << OCIE1A);
}

void Timer1_IntOff()
{
    TIMSK1 &= ~(1 << OCIE1A);
}