#include "include/spiPorts.h"

SPIPORTS::SPIPORTS(
    int sp1,
    int miso,
    int cs,
    int sck,
    int mosi,
    int reset,
    int dc) : spi(spi), miso(miso), cs(cs), sck(sck), mosi(mosi), reset(reset), dc(dc) {}

