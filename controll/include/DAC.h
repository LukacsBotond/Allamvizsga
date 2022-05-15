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
    // disable internal reference
    this->spi->write_data(DAC_INTERNAL_REFERENCE_ALLW_UP, 1);
    // inicialize with 0
    setVoltageOnChannel(0, 0, DAC_COMM_WRITE_LOAD_ALLCHANNEL);
}

DAC::~DAC()
{
}

void DAC::setVoltageOnChannel(uint16_t voltage, uint8_t command)
{
    uint32_t merged = ((uint32_t)(command << 16)) | voltage;
    this->spi->write_data(&merged,1);
}
