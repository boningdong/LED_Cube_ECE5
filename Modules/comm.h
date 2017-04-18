#include <Arduino.h>
#ifndef CUBE_COMM
    #define CUBE_COMM
#define SDA PC4
#define SCL PC5
#define OWN_ADDR 0x06

void CommInit();

void CommInit(){
	DDRC &= ~((1<<SCL)|(1<<SDA));
	PORTC |= (1<<SCL)|(1<<SDA);
    TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE); //Acknoledge Enable, I2C Enable, Interrupt Enable
    TWAR = OWN_ADDR << 1; 
}

#endif
