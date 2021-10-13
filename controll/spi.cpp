#include "./include/spi.h"
#include <iostream>
using namespace std;

SPI::SPI(int frekv, SPIPORTS portsIn)
{
    this->ports = new SPIPORTS(
        portsIn.spi, portsIn.miso, portsIn.cs, portsIn.sck,
        portsIn.mosi, portsIn.reset, portsIn.dc);
    printer();
    if (portsIn.spi == 1)
    {
        spi_Hw_inst = spi1;
    }
    else
    {
        spi_Hw_inst = spi0;
    }
    spi_init(spi_Hw_inst, 1000000000);
    int baudrate = spi_set_baudrate(spi_Hw_inst, 1000000000);
    std::cout << "baudrate: " << baudrate << std::endl;
    gpio_set_function(ports->miso, GPIO_FUNC_SPI);
    gpio_set_function(ports->sck, GPIO_FUNC_SPI);
    gpio_set_function(ports->mosi, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(ports->cs);
    gpio_set_dir(ports->cs, GPIO_OUT);
    gpio_put(ports->cs, 0);

    // high = command, low = data
    gpio_init(ports->dc);
    gpio_set_dir(ports->dc, GPIO_OUT);
    gpio_put(ports->dc, 0);

    // Reset is active-low
    gpio_init(ports->reset);
    gpio_set_dir(ports->reset, GPIO_OUT);
    gpio_put(ports->cs, 1);

    printer();
}

SPI::~SPI()
{
    delete ports;
}

void SPI::cs_select()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(ports->cs, 0); // Active low
    asm volatile("nop \n nop \n nop");
}
void SPI::cs_deselect()
{
    asm volatile("nop \n nop \n nop");
    gpio_put(ports->cs, 1);
    asm volatile("nop \n nop \n nop");
}

void SPI::write_data(uint8_t* buffer, int bytes)
{
    cs_select();
    spi_write_blocking(spi_Hw_inst, buffer, bytes);
    cs_deselect();
}
void SPI::write_data_continuous(uint8_t* buffer, int bytes)
{
    spi_write_blocking(spi_Hw_inst, buffer, bytes);
}

void SPI::write_data(uint16_t *buffer, int bytes)
{
    cs_select();
    spi_write16_blocking(spi_Hw_inst, buffer, bytes);
    cs_deselect();
}

void SPI::write_data_continuous(uint16_t *buffer, int bytes)
{
    spi_write16_blocking(spi_Hw_inst,buffer, bytes);
}

void SPI::read_data(uint8_t send, uint8_t *buf, int bytes)
{
    sleep_ms(10);
    gpio_put(ports->dc, 0);
    sleep_ms(10);
    spi_write_blocking(spi_Hw_inst, &send, 1);
    gpio_put(ports->dc, 1);
    sleep_ms(10);
    cout << "read" << spi_read_blocking(spi_Hw_inst, 0, buf, bytes);
    sleep_ms(10);
    cout << "\nAdat: ";
    for (uint8_t i = 0; i < bytes; i++)
    {
        cout << (int)i << " " << (int)buf[i] << "\n";
        sleep_ms(200);
    }
    cout << endl;
}

//* DEBUG

void SPI::printer()
{
    cout << "DEBUG" << endl;
    std::cout << "spi: " << ports->spi<< std::endl;
    std::cout << "cs: " << ports->cs << std::endl;
    std::cout << "dc: " << ports->dc << std::endl;
    std::cout << "miso: " << ports->miso << std::endl;
    std::cout << "mosi: " << ports->mosi << std::endl;
    std::cout << "reset: " << ports->reset << std::endl;
    std::cout << "sck: " << ports->sck << std::endl;
}
