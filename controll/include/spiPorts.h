#pragma once

class SPIPORTS
{
public:
    int spi;
    int miso;
    int cs;
    int sck;
    int mosi;
    int reset;
    int dc;
    SPIPORTS(
        int spi,
        int miso,
        int cs,
        int sck,
        int mosi,
        int reset,
        int dc);

    ~SPIPORTS(){}
};
