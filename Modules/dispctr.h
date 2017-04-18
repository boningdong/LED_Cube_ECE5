#ifndef DISP_CTRL
    #define DISP_CTRL

extern unsigned char disp_data [8][8];

void ResetDispData();
void ResetLayer(char layer);
void TurnOnLayer(char layer);
void ResetYWall(char y);
void ResetXWall(char x);
void TurnOnYWall(char y);
void TurnOnXWall(char x);
void TurnOnLED(char x, char y, char z);
void TurnOffLED(char x, char y, char z);
boolean IsLED_On(char x, char y, char z);

void ResetDispData()
{
    for(int layer = 0; layer < 8; layer++)
    {
        for(int row = 0; row < 8; row++)
        {
            disp_data[layer][row] |= 0xff;
        }
    }
}

void ResetLayer(char layer)
{
    for(char row = 0; row < 8; row ++)
        disp_data[layer][row] |= 0xff; 
}

void TurnOnLayer(char layer)
{
    for(char row = 0; row < 8; row ++)
        disp_data[layer][row] &= 0x00;
}

void ResetYWall(char y)
{
    for(char layer = 0; layer < 8; layer ++){
        disp_data[layer][y] |= 0xff;
    }
}

void TurnOnYWall(char y)
{
    for(char layer = 0; layer < 8; layer ++){
        disp_data[layer][y] &= 0x00;
    }
}

void ResetXWall(char x)
{
    unsigned char *p = disp_data[0];
    for(char i = 0; i < 64; i++){      
        *(p+i) |= (1 << x);
    }
}

void TurnOnXWall(char x)
{
    unsigned char *p = disp_data[0];
    for(char i = 0; i < 64; i++){      
        *(p+i) &= ~(1 << x);
    }
}

void TurnOnLED(char x, char y, char z)
{
    //Row = y   Layer = z
    disp_data[z][y] &= ~(1 << x);
}

void TurnOffLED(char x, char y, char z)
{
    disp_data[z][y] |= (1 << x);
}

boolean IsLED_On(char x, char y, char z)
{
    if (disp_data[z][y] & (1 << x))
        return false;
    else
        return true;
}
#endif