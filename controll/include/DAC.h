#pragma once

#include "./IDAC.h"
#include "./spi.h"
#include "./PID.h"
#include "../../stateMachine/include/Transistor.h"

class DAC : public IDAC
{
private:
    SPI *spi;
    PID *pid;

    uint8_t channelSearch(int port);
    std::string measureMode = "000";
    void PIDCorrection(int gatePin, float ConstCurrentVal, int resistor);
    double getGatecurrent(ICALCULATE *icalculate, int basePin);
    double getGatecurrent(ICALCULATE *icalculate, double supplyVoltage, int basePin);

public:
    explicit DAC(SPI *spi);
    ~DAC();
    /*
    Set voltage level on the DAC, with the above defines the command can be set and
    the voltage is a 16 bit value, 16 bit is 3.3V
    @param voltage: uint16_t set voltage level
    @param command: uint_8 command for the data, for this usage of the defines is recommended
    */
    void setVoltageOnChannel(uint16_t voltage, uint8_t command) override;
    /*
    Set the DAC reset level
    @param resetLvl: bool true=reset High, false= reset Low
    */
    void reset(bool resetLvl) override;

    void characteristicDiagramm(ICALCULATE *icalculate) override;
};

DAC::DAC(SPI *spi) : spi(spi)
{
    // inicialize GPIO pins
    gpio_init(DAC_LDAC);
    gpio_init(DAC_RESET);
    gpio_init(DAC_ENABLE);
    gpio_init(DAC_RSTSEL);

    gpio_set_dir(DAC_LDAC, true);
    gpio_set_dir(DAC_RESET, true);
    gpio_set_dir(DAC_ENABLE, true);
    gpio_set_dir(DAC_RSTSEL, true);

    gpio_put(DAC_LDAC, LOW);
    gpio_put(DAC_RESET, HIGH);
    gpio_put(DAC_ENABLE, LOW);
    gpio_put(DAC_RSTSEL, LOW);

    this->spi->changeFormat(8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    // disable internal reference
    uint32_t merged = DAC_INTERNAL_REFERENCE_ALLW_DOWN;
    this->spi->write_data(&merged, 1);
    // inicialize with 0
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
    std::cout << "voltage: " << (int)voltage << " command: " << (int)command << " merged: " << merged << std::endl;
    this->spi->write_data(&merged, 1);
}

void DAC::reset(bool resetLvl)
{
    gpio_put(DAC_RESET, resetLvl);
}

void DAC::characteristicDiagramm(ICALCULATE *icalculate)
{
    adc->set_clkDiv(0);
    pid = new PID(5, 1, 1, 500000);
    TRANSISTOR transistor(icalculate);
    std::vector<int> resBase = transistor.usedPinsFindByValue("B");
    std::vector<int> resCollector = transistor.usedPinsFindByValue("C");
    std::cout << "max baseCurrent" << getGatecurrent(icalculate, resBase.at(0)) << std::endl;
    std::cout << "measureMode: " << measureMode << std::endl;
    // the switches will be opened by default so we need to reset them
    icalculate->controller->prepareSwitchSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->setSwithcSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    uint8_t IcChannel = channelSearch(resCollector.at(0));
    // uint8_t IbChannel = channelSearch(resBase.at(0));
    std::cout << "IcChannel: " << (int)IcChannel << std::endl;
    for (uint16_t IcVolt = 0; IcVolt < INT16_MAX; IcVolt += INT16_MAX / 200)
    {
        setVoltageOnChannel(IcVolt, IcChannel);
        std::cout << "icVolt: " << IcVolt << " max baseCurrent: " << getGatecurrent(icalculate, 3.3, resBase.at(0)) << std::endl;
        sleep_ms(250);
    }
}

//*Private
uint8_t DAC::channelSearch(int port)
{
    switch (port)
    {
    case 0:
        return DAC_COMM_WRITE_BUFF_LOAD_ALL_B;

    case 1:
        return DAC_COMM_WRITE_BUFF_LOAD_ALL_C;
        break;
    case 2:
        return DAC_COMM_WRITE_BUFF_LOAD_ALL_D;

        break;
    default:
        break;
    }
    throw NOTSUPPOSEDTOREACHTHIS("DAC channelSearch out of range");
}

double DAC::getGatecurrent(ICALCULATE *icalculate, int basePin)
{
    TRANSISTOR transistor(icalculate);
    std::vector<int> resCollector = transistor.usedPinsFindByValue("C");
    std::vector<int> resEmmiter = transistor.usedPinsFindByValue("E");

    if (STATE::mainResult == "npn transistor")
    {
        measureMode[basePin] = '2';
    }
    else
    {
        measureMode[basePin] = '1';
    }
    measureMode[resCollector.at(0)] = '2';
    measureMode[resEmmiter.at(0)] = '1';
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    std::cout << measurement.at(0) << " " << measurement.at(1) << " " << measurement.at(2) << std::endl;
    return double((3.3 - measurement.at(basePin)) / RESISTOR_LOW) * 1000.0;
}

double DAC::getGatecurrent(ICALCULATE *icalculate, double supplyVoltage, int basePin)
{
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat();
    std::cout << measurement.at(0) << " " << measurement.at(1) << " " << measurement.at(2) << std::endl;
    return double((supplyVoltage - measurement.at(basePin)) / RESISTOR_LOW) * 1000.0;
}

void DAC::PIDCorrection(int gatePin, float ConstCurrentVal, int resistor)
{
}