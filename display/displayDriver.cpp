#include "./include/displayDriver.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

DISPLAYDRIVER::DISPLAYDRIVER(/* args */)
{
    display = new ILI9341();
    // setup to draw the whole screen

    // column address set
    display->set_command(ILI9341_CASET);
    display->command_param(0x00);
    display->command_param(0x00); // start column
    display->command_param(0x00);
    display->command_param(0xef); // end column -> 239

    // page address set
    display->set_command(ILI9341_PASET);
    display->command_param(0x00);
    display->command_param(0x00); // start page
    display->command_param(0x01);
    display->command_param(0x3f); // end page -> 319

    // start writing
    display->set_command(ILI9341_RAMWR);

    


}

DISPLAYDRIVER::~DISPLAYDRIVER()
{
    delete display;
}

void DISPLAYDRIVER::fillColor(uint16_t color)
{
    display->changeFormat(true);
    int pixels = 240*320;
    for (int i = 0; i < pixels; i++)
    {
        display->write_data(&color, 1);
    }
    display->changeFormat(false);
}