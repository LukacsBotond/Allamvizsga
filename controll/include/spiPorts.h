#pragma once

class spiPorts
{
public:
    int spi;
    int miso;
    int cs;
    int sck;
    int mosi;
    int reset;
    int dc;
    spiPorts(
        int spi,
        int miso,
        int cs,
        int sck,
        int mosi,
        int reset,
        int dc);

    ~spiPorts();
};
