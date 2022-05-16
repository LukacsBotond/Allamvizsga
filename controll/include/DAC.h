#pragma once

#include "./IDAC.h"
#include "./spi.h"
class DAC : public IDAC
{
private:
    SPI *spi;

public:
    explicit DAC(SPI *spi);
    ~DAC();
    void setVoltageOnChannel(uint16_t voltage, uint8_t command) override;
};

DAC::DAC(SPI *spi) : spi(spi)
{
    gpio_init(DAC_LDAC);
    gpio_init(DAC_RESET);
    gpio_init(DAC_ENABLE);
    gpio_init(DAC_RSTSEL);

    gpio_set_dir(DAC_LDAC,true);
    gpio_set_dir(DAC_RESET,true);
    gpio_set_dir(DAC_ENABLE,true);
    gpio_set_dir(DAC_RSTSEL,true);

    gpio_put(DAC_LDAC,LOW);
    gpio_put(DAC_RESET,HIGH);
    gpio_put(DAC_ENABLE,LOW);
    gpio_put(DAC_RSTSEL,LOW);

    this->spi->changeFormat(8,SPI_CPOL_0,SPI_CPHA_1,SPI_MSB_FIRST);
    // disable internal reference
    uint32_t merged = DAC_INTERNAL_REFERENCE_ALLW_DOWN;
    this->spi->write_data(&merged, 1);
    // inicialize with 0
    // setVoltageOnChannel(0, DAC_COMM_WRITE_LOAD_ALLCHANNEL);
    merged = ((uint32_t)(DAC_COMM_WRITE_LOAD_ALLCHANNEL << 16)) | (uint16_t)0;
    this->spi->write_data(&merged, 1);
}

DAC::~DAC()
{
    delete spi;
}

void DAC::setVoltageOnChannel(uint16_t voltage, uint8_t command)
{
    uint32_t merged = ((uint32_t)(command << 16)) | (uint16_t)voltage;
    this->spi->write_data(&merged, 1);
}
