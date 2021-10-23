#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "./spiPorts.h"

class SPI
{
protected:
    spi_inst_t *spi_Hw_inst;

public:
    SPIPORTS *ports;
    SPI(int frekv, SPIPORTS ports);
    ~SPI();

    //* if true then it sets SPI to write 16 bit
    //* else outputs 8 bit only
    void changeFormat(bool dub);
    void cs_select();
    void cs_deselect();

    void write_data(uint8_t *buffer, int bytes);
    void write_data_continuous(uint8_t *buffer, int bytes);

    void write_data(uint16_t *buffer, int bytes);
    void write_data_continuous(uint16_t *buffer, int bytes);

    //* DEBUG
    void printer();
};
