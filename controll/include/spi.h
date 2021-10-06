#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "spiPorts.h"

class spi
{
private:
    spi_inst_t *spi_Hw_inst;
public:
    spiPorts *ports;
    spi(int frekv, spiPorts *ports);
    ~spi();

    void cs_select();
    void cs_deselect();

    void write_data(uint8_t *buffer, int bytes);
    void write_data_continuous(uint8_t *buffer, int bytes);
};
