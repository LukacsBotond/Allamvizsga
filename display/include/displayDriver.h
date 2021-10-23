#pragma once

#include "./ili9341.h"

class DISPLAYDRIVER
{
protected:
    int rowHeight;
    ILI9341 *display;
    uint16_t* row;
public:
    DISPLAYDRIVER();
    ~DISPLAYDRIVER();

    void writeLine();
    //* crears the screen with a set color
    void fillColor(uint16_t color = 0x0000);
};

