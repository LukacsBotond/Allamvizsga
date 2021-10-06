#pragma once

class spiPorts
{
public:
    int sp1;
    int miso;
    int cs;
    int sck;
    int mosi;
    int reset;
    int dc;
    spiPorts(
        int sp1,
        int miso,
        int cs,
        int sck,
        int mosi,
        int reset,
        int dc);

    ~spiPorts();
};
