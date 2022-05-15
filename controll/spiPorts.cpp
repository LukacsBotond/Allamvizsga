#include "include/spiPorts.h"

SPIPORTS::SPIPORTS(
    int spi,
    int cs,
    int sck,
    int mosi) : spi(spi), cs(cs), sck(sck), mosi(mosi){}

