#include "./include/spi.h"
#include "../Global.h"
#include <iostream>
using namespace std;

SPI::SPI(int frekv, SPIPORTS *portsIn) : mode(false)
{
    this->ports = portsIn;
    // printer();
    if (this->ports->spi)
    {
        spi_Hw_inst = spi1;
    }
    else
    {
        spi_Hw_inst = spi0;
    }
    spi_init(spi_Hw_inst, frekv);
    int baudrate = spi_set_baudrate(spi_Hw_inst, frekv);
    std::cout << "baudrate: " << baudrate << std::endl;
    gpio_set_function(ports->sck, GPIO_FUNC_SPI);
    gpio_set_function(ports->mosi, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(ports->cs);
    gpio_set_dir(ports->cs, GPIO_OUT);
    gpio_put(ports->cs, LOW);

    printer();
}

SPI::~SPI()
{
    delete ports;
}

void SPI::changeFormat(bool mode)
{
    this->mode = mode;
    if (mode)
        spi_set_format(spi_Hw_inst, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    else
        spi_set_format(spi_Hw_inst, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

void SPI::changeFormat(uint data_bits, spi_cpol_t cpol, spi_cpha_t cpha, spi_order_t order)
{
    spi_set_format(spi_Hw_inst, data_bits, cpol, cpha, order);
}
void SPI::cs_select()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(ports->cs, LOW); // Active low
    asm volatile("nop \n nop \n nop");
}
void SPI::cs_deselect()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(ports->cs, HIGH);
    asm volatile("nop \n nop \n nop");
}

void SPI::write_data(const uint8_t *buffer, int bytes)
{
    cs_select();
    spi_write_blocking(spi_Hw_inst, buffer, bytes);
    cs_deselect();
}
void SPI::write_data_continuous(uint8_t *buffer, int bytes)
{
    spi_write_blocking(spi_Hw_inst, buffer, bytes);
}

void SPI::write_data(const uint16_t *buffer, int bytes)
{
    cs_select();
    spi_write16_blocking(spi_Hw_inst, buffer, bytes);
    cs_deselect();
}

void SPI::write_data_continuous(uint16_t *buffer, int bytes)
{
    spi_write16_blocking(spi_Hw_inst, buffer, bytes);
}

void SPI::write_data(const uint32_t *buffer, int bytes)
{
    for (int i = 0; i < bytes; i++)
    {
        uint8_t command = (uint8_t)(buffer[i] >> 16);
        uint16_t voltage = (uint16_t)buffer[i];
        cout << "SPI 32 bit data: " << hex << int(command) << " // " << hex << int(voltage) << endl;
        cout << dec << (int)0 << std::endl;
        sleep_ms(100);
        command = command >> 1;
        voltage = voltage >> 1;
        cs_select();
        if (mode)
        { // the spi is in 16 bit format
            // cout << "mode0" << std::endl;
            changeFormat(!mode);                          // set it to 8
            spi_write_blocking(spi_Hw_inst, &command, 1); // send command
            changeFormat(!mode);                          // currently 8 set back to 16
            spi_write16_blocking(spi_Hw_inst, &voltage, 1);
        }
        else
        { // the spi is in 8 bit format
            // cout << "mode1" << std::endl;
            spi_write_blocking(spi_Hw_inst, &command, 1);   // send command
            changeFormat(!mode);                            // currently 8 set back to 16
            spi_write16_blocking(spi_Hw_inst, &voltage, 1); // send voltage
            changeFormat(!mode);                            // set it to 8
        }
        cs_deselect();
    }
}

//* DEBUG

void SPI::printer()
{
    cout << "DEBUG" << endl;
    std::cout << "spi: " << ports->spi << std::endl;
    std::cout << "cs: " << ports->cs << std::endl;
    std::cout << "mosi: " << ports->mosi << std::endl;
    std::cout << "sck: " << ports->sck << std::endl;
}
