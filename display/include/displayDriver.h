#pragma once

#include "./ili9341.h"

class DISPLAYDRIVER
{
private:
    ILI9341 *display;
public:
    DISPLAYDRIVER(/* args */);
    ~DISPLAYDRIVER();
    void fillColor(uint16_t color = 0x0000);
};

