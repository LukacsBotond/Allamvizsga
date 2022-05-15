#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "pico/stdlib.h"
#include "include/ili9341.h"
#include "../Global.h"

/*

 (pin 1) VCC        5V/3.3V power input
 (pin 2) GND        Ground
 (pin 3) CS         LCD chip select signal, low level enable
 (pin 4) RESET      LCD reset signal, low level reset
 (pin 5) DC/RS      LCD register / data selection signal; high level: register, low level: data
 (pin 6) SDI(MOSI)  SPI bus write data signal
 (pin 7) SCK        SPI bus clock signal
 (pin 8) LED        Backlight control; if not controlled, connect 3.3V always bright
 (pin 9) SDO(MISO)  SPI bus read data signal; optional

 */
/*
ili9341_config_t ili9341_config = {
    .port = spi0,
    .pin_miso = 4,
    .pin_cs = 5,
    .pin_sck = 6,
    .pin_mosi = 7,
    .pin_reset = 8,
    .pin_dc = 9
};
*/

ILI9341::ILI9341(SPI *spi) : spi(spi)
{
    // spiPorts* tmp = new spiPorts(0, 10, 13, 14, 15, 12, 11);
    // SPIPORTS *tmpPorts = new SPIPORTS(0, 4, 5, 6, 7, 8, 9);
    // spi_instance = new SPI(300, tmpPorts);
    // delete tmpPorts;
    // high = command, low = data

    gpio_init(DISP_DC);
    gpio_set_dir(DISP_DC, GPIO_OUT);
    gpio_put(DISP_DC, LOW);

    // Reset is active-low
    gpio_init(DISP_RESET);
    gpio_set_dir(DISP_RESET, GPIO_OUT);
    gpio_put(DISP_RESET, HIGH);

    sleep_ms(10);
    gpio_put(DISP_RESET, LOW);
    sleep_ms(10);
    gpio_put(DISP_RESET, HIGH);
    spi->changeFormat(false);
    set_command(0x01); // soft reset
    sleep_ms(100);
    set_command(ILI9341_GAMMASET);
    command_param(0x01);
    uint8_t tmp[]{0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00};
    // positive gamma correction
    set_command(ILI9341_GMCTRP1);
    spi->write_data(tmp, 15);
    uint8_t tmp1[]{0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f};
    // negative gamma correction
    set_command(ILI9341_GMCTRN1);
    spi->write_data(tmp1, 15);
    // memory access control
    set_command(ILI9341_MADCTL);
    command_param(0x48);

    // pixel format
    set_command(ILI9341_PIXFMT);
    command_param(0x55); // 16-bit
    // frame rate; default, 70 Hz
    set_command(ILI9341_FRMCTR1);
    command_param(0x00);
    command_param(0x1B);
    // exit sleep
    set_command(ILI9341_SLPOUT);
    set_command(ILI9341_DISPOFF);
    sleep_ms(100);

    // display on
    set_command(ILI9341_DISPON);
    //

    // landscape mode
    set_command(ILI9341_MADCTL);
    command_param(0b11110100);

    // column address set
    set_command(ILI9341_CASET);
    command_param(0x00);
    command_param(0x00); // start page
    command_param(0x01);
    command_param(0x3f); // end page -> 319

    // page address set
    set_command(ILI9341_PASET);
    command_param(0x00);
    command_param(0x00); // start column
    command_param(0x00);
    command_param(0xef); // end column -> 239

    set_command(ILI9341_RAMWR);

    this->currentLine = 0;
    this->rowSize = lineHeight * ILI9341_TFTWIDTH;
    this->row = new uint16_t[rowSize];
}

ILI9341::~ILI9341()
{
    delete row;
}

void ILI9341::set_command(uint8_t cmd)
{
    gpio_put(DISP_DC, LOW);
    spi->write_data(&cmd, 1);
    gpio_put(DISP_DC, HIGH);
}

void ILI9341::command_param(uint8_t data)
{
    spi->write_data(&data, 1);
}

void ILI9341::writeLine()
{
    spi->changeFormat(true);
    spi->write_data(row, rowSize);
    spi->changeFormat(false);
}

// TODO whatewer this is NOT doing
void ILI9341::fillRestScreen(uint16_t color)
{
    /*
    display->changeFormat(true);
    if (currentLine >= ILI9341::height / lineHeight)
    {
        return;
    }
    display->changeFormat(false);
    */
}

void ILI9341::fillColor(uint16_t color)
{
    set_command(ILI9341_RAMWR);
    spi->changeFormat(true);
    int pixels = ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT;
    for (int i = 0; i < pixels; i++)
    {
        spi->write_data(&color, 1);
    }
    spi->changeFormat(false);
}

//* DEBUG
void ILI9341::dumpRow()
{
    std::cout << "character write dump row\n";
    for (int i = 0; i < rowSize; i++)
    {
        std::cout << row[i] << " ";
        sleep_ms(1);
    }
}