#include "./include/displayDriver.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

DISPLAYDRIVER::DISPLAYDRIVER(/* args */)
{
    display = new ILI9341();
    std::cout << "got ili9341 instance" << std::endl;
}

DISPLAYDRIVER::~DISPLAYDRIVER()
{
    delete display;
}

void DISPLAYDRIVER::readDispInformation()
{
    std::cout << "display adatai" << std::endl;
    display->readData(0x04);
    //display->set_command(0x28);
    //sleep_ms(1000);
    //display->set_command(0x29);
    //sleep_ms(1000);

}

void DISPLAYDRIVER::demo()
{
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
    int screen_idx = 0;
    while(1)
    {
        uint8_t buff[1];
        buff[0] = (uint8_t)0x00;
        display->write_data(buff, 1);
    }
}