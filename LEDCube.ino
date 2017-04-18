#include <Arduino.h>
#include <time.h>
#include <stdlib.h>
#include "Modules/disp.h"
#include "Modules/timer.h"
#include "Modules/comm.h"
#include "Modules/dispctr.h"
#include "Pattern.h"
#include "InterruptConfig.h"

//Display Data:
unsigned char disp_data [8][8] = {
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} };
unsigned char *dataPointer = disp_data[0];
unsigned char dispIndex = 0;

char current_layer = 0;
char init_flag = 1;

void setup() 
{
    srand(time(NULL));
    delay(500);
}

void loop()
{
    if (init_flag == 1)
    {
        //Interrupt Init 
        Global_IntInit();
        Timer1_IntOn();

        //Display Part
        //LED Part:
        DisplayInit();
        LayerDisplayInit();
        LayerChooseInit();

        //Timer Part:
        TimerInit();
        DisplayOn();

        //Communication Part
        CommInit();

        //Debug Part:
        Serial.begin(115200);
        init_flag = 0;
    }
    
    RandLightUp();
    RandLightDown();
    DisplayRain(55);
    DisplayPyramid(30);
    for(int i = 0; i < 10; i++)
        MeteorEffects(60);
    DisplayTrig(70);
    RandSuspendSequence();
    
    
}

//Display Per-Layer Interrupt
//For refreshing display
ISR(TIMER1_COMPA_vect)
{
    DisplayOff();
    current_layer ++;
    if (current_layer == 8)
        current_layer = 0;
    LayerChoose(current_layer);
    LayerDisplay(disp_data[current_layer]);
    DisplayOn();
}

//The Interrupt for I2C Communication
//For Receiving the display data.
ISR(TWI_vect){
	switch(TWSR){
		case 0x60: //Own Address Received and ACK sent
            Timer1_IntOff();
			dispIndex = 0;
			break;

		case 0x80: //Own Address Received, Data Received and ACK sent
			*(dataPointer + dispIndex) = TWDR;
			dispIndex ++;
            if(dispIndex == 64){
                dispIndex = 0;
            }
			break;

		case 0xA0: //Still Addressed as Slave, STOP or RESTART received.
            Timer1_IntOn();
			break;
		
		default:
			break;
	}
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1<<TWIE); //Clear TWire Interrupt, then release the CLK line
}
