#include <Arduino.h>
#ifndef CUBE_DISPLAY
    #define CUBE_DISPLAY

//Initializing Functions:
void DisplayInit();
void LayerDisplayInit();
void LayerChooseInit();

//Control Functions:
void LayerDisplay(unsigned char *);
void LayerChoose(char);

//Status Control/Check Functions
void DisplayOn();
void DisplayOff();
void DisplayIsOn();

//Private Functions:
void ShiftClk();
void LatchClk();
void DelayClk();
#endif

//Name Definitions:
#define SHCP 6
#define STCP 5 
#define SDIN 4
#define RST 7
 /*
 * Initializing Functions:
 * - DisplayInit: Initializing the global switch of LED Cube. Enable the cube to be turned on/off.
 * - LayerDisplayInit: Initializing layer led control function. Enable user to write data layerly.
 * - LayerChooseInit: Initializing layer control function. Choose which layer to be lighted up.
 */
void DisplayInit()
{
    DDRB |= (1 << DDB3);
    PORTB |= (1 << PB3); 
}

void LayerDisplayInit()
{
  DDRD |= (1 << SHCP) | (1 << STCP) | (1 << SDIN) | (1 << RST);
  PORTD |= (1 << RST);
}

void LayerChooseInit()
{
  DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2);  //Set PC0 (A0), PC1 (A1), PC2 (A2) to be output mode
  PORTC = 0x00;
}

/*
* Status Control/Check Functions
* - DisplayOn: Turn on all the lights
* - DisplayIsOff: Turn off all the lights
* - DisplayIsOn: Check if the LED is turned on. if on return 1; else return false
*/
void DisplayOn()
{
    PORTB &= ~(1 << PB3);
}

void DisplayOff()
{
    PORTB |= (1 << PB3);
}

void DisplayIsOn()
{
    if (!(PORTB & (1<<PB3)))
        return 1;
    else 
        return 0;
}

/*
* Control Functions:
* - LayerDisplay: Write data layerly. Control which LED will be lighted up in a single plane.
* - LayerChoose: Choose which layer will be lighted up.
*/

void LayerDisplay(unsigned char layer_data[])
{
  /*
  * Data Type:
  * -layer_data should be an array including 8 char type data.
  */
    char i, y;
    for (i = 0; i <= 7; i++)
    {
      for (y = 0; y <= 7; y++)
    {
            PORTD &= (~(0x01 << 4));   //PORTD Register
            PORTD |= ((((0x01 << y) & layer_data[i]) >> y) << 4);
      ShiftClk();
        }
    LatchClk();
    }
}

void LayerChoose(char layer)
{
  PORTC = layer; //layer should be between 0 - 7
}

/*
* Private Functions:
*   Used by above functions
* - ShiftClk: The clock signal to shift data on shift register.
* - LatchClk: The clock signal to latch data to output on shift register.
* - DelayClk: For keeping the signal high to make sure the clock signal is valid. 
*/
void ShiftClk()
{
    PORTD |= (0x01 << SHCP);
  DelayClk();
    PORTD &= ~(0x01 << SHCP);
}

void LatchClk()
{
  PORTD |= (0x01 << STCP);
    DelayClk();
    PORTD &= ~(0x01 << STCP);
}

void DelayClk()
{
    char i = 0;
    for (; i < 20; i++);
}
