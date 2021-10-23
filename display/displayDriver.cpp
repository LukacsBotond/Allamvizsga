#include "./include/displayDriver.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

DISPLAYDRIVER::DISPLAYDRIVER(/* args */)
{
     DISPLAYDRIVER::display = new ILI9341();
}

DISPLAYDRIVER::~DISPLAYDRIVER()
{
    delete display;
}

void DISPLAYDRIVER::fillColor(uint16_t color)
{
    display->set_command(ILI9341_RAMWR);
    int pixels = 240*320;
    for (int i = 0; i < pixels; i++)
    {
        display->write_data(&color, 1);
    }
}