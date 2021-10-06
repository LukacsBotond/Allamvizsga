#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "include/ili9341.h"

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
    .port = spi1,
    .pin_miso = 10,
    .pin_cs = 13,
    .pin_sck = 14,
    .pin_mosi = 15,
    .pin_reset = 12,
    .pin_dc = 11
};
*/

ili9341::ili9341()
{
    //spiPorts* tmp = new spiPorts(0, 10, 13, 14, 15, 12, 11);
    spiPorts *tmpPorts = new spiPorts(0, 10, 13, 14, 15, 12, 11);
    spi_instance = new spi(300, tmpPorts);

    sleep_ms(10);
    gpio_put(spi_instance->ports->reset, 0);
    sleep_ms(10);
    gpio_put(spi_instance->ports->reset, 1);

    set_command(0x01); //soft reset
    sleep_ms(100);

    set_command(ILI9341_GAMMASET);
    command_param(0x01);
    uint8_t tmp[]{0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00};

    // positive gamma correction
    set_command(ILI9341_GMCTRP1);
    spi_instance->write_data(tmp, 15);
    //write_data(tmp, 15);
    uint8_t tmp1[]{0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f};
    // negative gamma correction
    set_command(ILI9341_GMCTRN1);
    spi_instance->write_data(tmp1, 15);

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

    // display on
    set_command(ILI9341_DISPON);

    //

    // column address set
    set_command(ILI9341_CASET);
    command_param(0x00);
    command_param(0x00); // start column
    command_param(0x00);
    command_param(0xef); // end column -> 239

    // page address set
    set_command(ILI9341_PASET);
    command_param(0x00);
    command_param(0x00); // start page
    command_param(0x01);
    command_param(0x3f); // end page -> 319

    set_command(ILI9341_RAMWR);
}

void ili9341::set_command(uint8_t cmd)
{
    spi_instance->cs_select();
    gpio_put(spi_instance->ports->dc, 0);
    spi_instance->write_data(&cmd, 1);
    //spi_write_blocking(ili9341_config.port, &cmd, 1);
    gpio_put(spi_instance->ports->dc, 1);
    spi_instance->cs_deselect();
}

void ili9341::command_param(uint8_t data)
{
    spi_instance->cs_select();
    spi_instance->write_data(&data, 1);
    spi_instance->cs_deselect();
}