#include "./include/displayDriver.h"

#include <iostream>

using namespace std;

DISPLAYDRIVER::DISPLAYDRIVER(/* args */)
{
    display = ILI9341::getInstance();
    std::cout << "got ili9341 instance" << std::endl;
}

DISPLAYDRIVER::~DISPLAYDRIVER()
{
    delete display;
}

void DISPLAYDRIVER::readDispInformation(){
    std::cout << "display adatai" << std::endl;
    display->readData(0x04);
    //display->set_command(0x28);
    //sleep_ms(1000);
    //display->set_command(0x29);
    //sleep_ms(1000);
    


    /*
    uint8_t readReg[4];
    display->spi_instance->cs_select();
    display->set_command(0x04);
    display->spi_instance->read_data(0x04,readReg,4);
    display->spi_instance->cs_deselect();
    */
}

