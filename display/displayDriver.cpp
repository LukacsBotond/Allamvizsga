#include "./include/displayDriver.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

uint8_t DISPLAYDRIVER::currentLine = 0;
ILI9341 *DISPLAYDRIVER::display = new ILI9341();
uint16_t DISPLAYDRIVER::rowSize = DISPLAYDRIVER::lineHeight * ILI9341::width;
uint16_t *DISPLAYDRIVER::row = new uint16_t[DISPLAYDRIVER::rowSize];

DISPLAYDRIVER::DISPLAYDRIVER(/* args */)
{
    //DISPLAYDRIVER::display = new ILI9341();
}

DISPLAYDRIVER::~DISPLAYDRIVER()
{
    //delete display;
}

void DISPLAYDRIVER::writeLine()
{
    display->changeFormat(true);
    display->write_data(row, rowSize);
    display->changeFormat(false);
}

void DISPLAYDRIVER::fillRestScreen(uint16_t color)
{
    display->changeFormat(true);
    if (currentLine >= ILI9341::height / lineHeight)
    {
        return;
    }
    display->changeFormat(false);
}

void DISPLAYDRIVER::fillColor(uint16_t color)
{
    display->set_command(ILI9341_RAMWR);
    display->changeFormat(true);
    int pixels = 240 * 320;
    for (int i = 0; i < pixels; i++)
    {
        display->write_data(&color, 1);
    }
    display->changeFormat(false);
}