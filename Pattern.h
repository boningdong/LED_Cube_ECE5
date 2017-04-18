#include <Arduino.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Modules/dispctr.h"

#ifndef CUBE_PARTTERN
    #define CUBE_PARTTERN

extern unsigned char disp_data [8][8];

//DisplayPyramid:
void DisplaySquare(char, char, char);
void DisplayPyramid();
//DisplayRain:
void ShiftLayer(char sft_num);
void DisplayRain();
//MeteorEffects:
void DrawCenterSquare(char x, char y, char z, char dis);
void MeteorEffects();
//Random:
void RandLightUp();
void RandLightDown();
//Trig Function:
void DisplayTrig(int speed);
//Suspend:
void RandSuspend_OffAir(char m, int j, char positions);
void RandSuspend_OnAir(char m, int j, char positions);
void RandSuspend(char m);
void RandSuspendSequence();

//Basic Transform:
void ShiftLayer(char sft_num){
    switch(sft_num){
        case 1:
        for(char layer = 7; layer > 0; layer --)
        {
            for(char row = 0; row < 8; row ++){
                disp_data[layer][row] = disp_data[layer-1][row];
            }
        }
        ResetLayer(0);
        break;
    
        case -1:
        for(char layer = 0; layer < 7; layer ++)
        {
            for(char row = 0; row < 8; row ++){
                disp_data[layer][row] = disp_data[layer+1][row];
            }
        }
        ResetLayer(7);
        break;
    }
}
void ShiftYWall(char sft_num){
    switch(sft_num){
        case 1:
        for(char layer = 0; layer < 8; layer ++)
        {
            for(char row = 7; row > 0; row --){
                disp_data[layer][row] = disp_data[layer][row-1];
            }
        }
        ResetYWall(0);
        break;
    
        case -1:
        for(char layer = 0; layer < 8; layer ++)
        {
            for(char row = 0; row < 7; row ++){
                disp_data[layer][row] = disp_data[layer][row+1];
            }
        }
        ResetYWall(7);
        break;
    }
}
void ShiftXWall(char sft_num){
    unsigned char *p = disp_data[0];
    switch(sft_num){
        case 1:
        for(char i = 0; i < 64; i ++){
            *(p+i) = *(p+i) << 1; 
        }
        ResetXWall(0);
        break;
    
        case -1:
        for(char i = 0; i < 64; i ++){
            *(p+i) = *(p+i) >> 1; 
        }
        ResetXWall(7);
        break;
    }
}

//DisplayPyramid:
void DisplaySquare(char lenth, char layer, char speed){
    for(char y = 7-lenth; y <= lenth; y++){
        TurnOnLED(7-lenth, y, layer);
        delay(speed);
    }
        
    for(char x = 7-lenth; x <= lenth; x++){
        TurnOnLED(x, lenth, layer);
        delay(speed);
    }

    for(char y = lenth; y >= 7-lenth; y--){
        TurnOnLED(lenth, y, layer);
        delay(speed);
    }

    for(char x = lenth; x >= 7-lenth; x--){
        TurnOnLED(x, 7-lenth, layer);
        delay(speed);
    }
}

void DisplayPyramid(int speed){
    ResetDispData();
    for(char i = 0; i < 4; i++)
        DisplaySquare(7-i, i, speed);
    for(char i = 0; i < 4; i++)
        DisplaySquare(4+i, 4+i, speed);
}

//DisplayRain:
void DisplayRain(int speed){
    ResetDispData();
    char led_x, led_y;
    for(int times = 0; times < 200; times ++){
        for(int led_num = 0; led_num < 4; led_num++){
            led_x = rand()%8; led_y = rand()%8;
            TurnOnLED(led_x, led_y, 7); 
        }
        delay(speed);
        ShiftLayer(-1);
    }
}

//DisplaySpread:
void DrawCenterSquare(char x, char y, char z, char dis){
    char x1 = x - dis, x2 = x + dis, y1 = y - dis, y2 = y + dis;
    for (char dy = y1; dy <= y2; dy++){
        if((dy >= 0)&&(dy < 8)){
            if((x1 >= 0)&&(x1 < 8))
                TurnOnLED(x1, dy, z);
            if((x2 >= 0)&&(x2 < 8))
                TurnOnLED(x2, dy, z);
        }
    }
    for (char dx = x1; dx <= x2; dx++){
        if((dx >= 0)&&(dx < 8)){
            if((y1 >= 0)&&(y1 < 8))
                TurnOnLED(dx, y1, z);
            if((y2 >= 0)&&(y2 < 8))
                TurnOnLED(dx, y2, z);
        }
    }
}

void MeteorEffects(int speed){
    ResetDispData();
    char led_x = rand()%8; char led_y = rand()%8;
    TurnOnLED(led_x, led_y, 7);
    for(char i = 0; i < 7; i++){
        delay(speed);
        ShiftLayer(-1);
    }
    for(char dis = 0; dis <= max(8-led_x, 8-led_y); dis++){
        ResetLayer(0);
        DrawCenterSquare(led_x, led_y, 0, dis);
        delay(speed);
    }
}

//Rand Light:
void RandLightUp(){
    ResetDispData();
    for (int i = 0; i < 512; i++){
        char x = rand()%8, y = rand()%8, z = rand()%8;
        while (IsLED_On(x, y, z)){
            x = rand()%8;
            y = rand()%8;
            z = rand()%8;
        }
        TurnOnLED(x, y, z);
        delay(int(20 - 0.03*i));
    }
}

void RandLightDown(){
    for (int i = 0; i < 512; i++){
        char x = rand()%8, y = rand()%8, z = rand()%8;
        while (!IsLED_On(x, y, z)){
            x = rand()%8;
            y = rand()%8;
            z = rand()%8;
        }
        TurnOffLED(x, y, z);
        delay(int(20 - 0.03*i));
    }
}

//Cos Function:
void DisplayTrig(int speed){
    ResetDispData();
    float z;
    float X;
    for(int t = 0; t < 100; t ++)
    {
        ResetDispData();
        for(char x = 0; x < 8; x++){
            for(char y = 0; y < 8; y ++)
            {
                X = sqrt( pow((float(x)-3.5),2) + pow((float(y)-3.5),2) );    
                z = 3.5*cos(0.5*(X - 3.5 + t)) + 4.0;
                TurnOnLED(x, y, int(z));
                
            }
        }
        delay(speed);
    } 
}

//Random Suspend
void RandSuspend_OffAir(char m, int j, char positions){
	if (m == 0)
		TurnOffLED((j%8), (j/8), positions);
	if (m == 1)
		TurnOffLED((j%8), (j/8), (7-positions));
	if (m == 2)
		TurnOffLED((j%8), positions, (j/8));
	if (m == 3)
		TurnOffLED((j%8), (7-positions), (j/8));
	if (m == 4)
		TurnOffLED(positions,(j%8),  (j/8));
	if (m == 5)
		TurnOffLED((7-positions),(j%8),  (j/8));
}

void RandSuspend_OnAir(char m, int j, char positions){
	if (m == 0)
		TurnOnLED((j%8), (j/8), positions);
	if (m == 1)
		TurnOnLED((j%8), (j/8), (7-positions));
	if (m == 2)
		TurnOnLED((j%8), positions, (j/8));
	if (m == 3)
		TurnOnLED((j%8), (7-positions), (j/8));
	if (m == 4)
		TurnOnLED(positions,(j%8),  (j/8));
	if (m == 5)
		TurnOnLED((7-positions),(j%8),  (j/8));
}

void RandSuspend(char m){
  char positions[64];
  char destinations[64];
  for (int i=0;i<64;i++){
    positions[i] = 0;
    destinations[i] = rand()%8;
  }
  switch (m){
	case 0:
		TurnOnLayer(0);
		break;
	case 1:
		TurnOnLayer(7);
		break;
	case 2:
		TurnOnYWall(0);
		break;
	case 3:
		TurnOnYWall(7);
		break;
	case 4:
		TurnOnXWall(0);
		break;
	case 5:
		TurnOnXWall(7);
		break;
  }
  delay(1000);
  for (int i=0;i<8;i++){
    for (int j=0;j<64;j++){
      if (positions[j] < destinations[j]){
        RandSuspend_OffAir(m, j, positions[j]);
		//TurnOffLED((j%8), (j/8), positions[j]);
        positions[j]++;
        RandSuspend_OnAir(m, j, positions[j]);
		//TurnOnLED((j%8), (j/8), positions[j]);
      }
    }
	delay(40);
  }
  for (int i=0;i<64;i++){
	  destinations[i] =7;
  }
  delay(700);
  for (int i=0;i<8;i++){
    for (int j=0;j<64;j++){
      if (positions[j] < destinations[j]){
        RandSuspend_OffAir(m, j, positions[j]);
		//TurnOffLED((j%8), (j/8), positions[j]);
        positions[j]++;
        RandSuspend_OnAir(m, j, positions[j]);
		//TurnOnLED((j%8), (j/8), positions[j]);
      }
    }
	delay(40);
  }
  delay(1000);
}

void RandSuspendSequence(){
    for(int i = 0; i < 6; i++)
    {
        if(i%2 == 0)
            ResetDispData();
        RandSuspend(i);
    }
}

//Random Track:
void TurnOnNearLEDs(float x, float y, float z){
    for(int x_num = int(x); x_num < int(x) + 2; x_num++)
        for(int y_num = int(x); y_num < int(x) + 2; y_num++)
            for(int z_num = int(x); z_num < int(x) + 2; z_num++)
                TurnOnLED(x_num, y_num, z_num); 
}




#endif