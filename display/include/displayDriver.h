#pragma once

#include "./ili9341.h"

class DISPLAYDRIVER
{
private:
    ILI9341 *display;
public:
    DISPLAYDRIVER(/* args */);
    ~DISPLAYDRIVER();

    void readDispInformation();

    void demo();
};

