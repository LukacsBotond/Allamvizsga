#include "include/spi.hpp"

using namespace std;


spi::spi(/* args */)
{
    spi_init(ili9341_config.port, 500 * 1000);
    int baudrate = spi_set_baudrate(ili9341_config.port, 75000 * 1000);
}

spi::~spi()
{
}
