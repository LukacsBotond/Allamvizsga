#include "./include/spi.h"

using namespace std;

spi::spi(int frekv, spiPorts* ports)
{
    ports = ports;
    if (ports->spi == 1)
    {
        spi_Hw_inst = spi1;
    }
    else
    {
        spi_Hw_inst = spi0;
    }
    spi_init(spi_Hw_inst, 1000);
    int baudrate = spi_set_baudrate(spi_Hw_inst, frekv * 1000);

    gpio_set_function(ports->miso, GPIO_FUNC_SPI);
    gpio_set_function(ports->sck, GPIO_FUNC_SPI);
    gpio_set_function(ports->mosi, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(ports->cs);
    gpio_set_dir(ports->cs, GPIO_OUT);
    gpio_put(ports->cs, 0);

    // Reset is active-low
    gpio_init(ports->reset);
    gpio_set_dir(ports->reset, GPIO_OUT);
    gpio_put(ports->reset, 1);
}

spi::~spi()
{
}

void spi::cs_select(){
    asm volatile("nop \n nop \n nop");
    gpio_put(ports->cs, 0); // Active low
    asm volatile("nop \n nop \n nop");
}
void spi::cs_deselect(){
    asm volatile("nop \n nop \n nop");
    gpio_put(ports->cs, 1);
    asm volatile("nop \n nop \n nop");
}

void spi::write_data(uint8_t *buffer, int bytes){
    cs_select();
    spi_write_blocking(spi_Hw_inst, buffer, bytes);
    cs_deselect();
}
void spi::write_data_continuous(uint8_t *buffer, int bytes){
     spi_write_blocking(spi_Hw_inst, buffer, bytes);
}
