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
    int CollectorPin = 0;
    int BasePin = 0;
    int EmitterPin = 0;
    uint16_t baseVoltage = UINT16_MAX;

    uint8_t channelSearch(int port);
    double convertToVolt(uint16_t baseVoltage);
    std::string measureMode = "000";
    void PIDCorrection(ICALCULATE *icalculate, int basePin, float ConstCurrentVal);
    double getGatecurrent(ICALCULATE *icalculate, int basePin);
    double getShuntcurrent(ICALCULATE *icalculate, double supplyVoltage, int Pin);
    std::vector<double> NPNLoop(ICALCULATE *icalculate, double IbConst);
    std::vector<double> PNPLoop(ICALCULATE *icalculate, double IbConst);

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

    CharDiagr characteristicDiagramm(ICALCULATE *icalculate) override;
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
    this->spi->write_data(&merged, 1);
}

void DAC::reset(bool resetLvl)
{
    gpio_put(DAC_RESET, resetLvl);
}

CharDiagr DAC::characteristicDiagramm(ICALCULATE *icalculate)
{
    if (STATE::mainResult != "npn transistor" && STATE::mainResult != "pnp transistor")
    {
        std::cout << "DAC characteristicDiagramm is only for transistors\n";
        throw NOTSUPPOSEDTOREACHTHIS("DAC characteristicDiagramm is only for transistors");
    }
    CharDiagr ret;
    std::vector<double> tmp;
    adc->set_clkDiv(0);
    TRANSISTOR transistor(icalculate);
    std::vector<int> res = transistor.usedPinsFindByValue("B");
    BasePin = res.at(0);
    res = transistor.usedPinsFindByValue("C");
    CollectorPin = res.at(0);
    res = transistor.usedPinsFindByValue("E");
    EmitterPin = res.at(0);

    double IbConst = getGatecurrent(icalculate, BasePin);
    double maxIcmA = -1;

    std::cout << "Start B: " << (int)BasePin << " C: " << (int)CollectorPin << " E: " << (int)EmitterPin << " mode: " << measureMode << std::endl;
    //   the switches will be opened by default so we need to reset them
    icalculate->controller->prepareSwitchSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->setSwithcSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');

    /*
        uint8_t IcChannel = channelSearch(CollectorPin);
        for (uint32_t IcVolt = 0; IcVolt < UINT16_MAX; IcVolt += UINT16_MAX / 200)
        {
            setVoltageOnChannel((uint16_t)IcVolt, IcChannel);
            PIDCorrection(icalculate, BasePin, IbConst);
            double IcmA = getShuntcurrent(icalculate, convertToVolt(IcVolt), CollectorPin);
            tmp.push_back(IcmA);
            if (IcmA > maxIcmA)
                maxIcmA = IcmA;
        }
        */
    if (STATE::mainResult == "npn transistor")
        ret.data = NPNLoop(icalculate, IbConst);
    else
        ret.data = PNPLoop(icalculate, IbConst);
    ret.yScale = maxIcmA;
    return ret;
}

//*Private
std::vector<double> DAC::NPNLoop(ICALCULATE *icalculate, double IbConst)
{
    std::vector<double> tmp;
    baseVoltage = UINT16_MAX;
    uint8_t IcChannel = channelSearch(CollectorPin);
    for (uint32_t IcVolt = 0; IcVolt < UINT16_MAX; IcVolt += UINT16_MAX / 200)
    {
        setVoltageOnChannel((uint16_t)IcVolt, IcChannel);
        PIDCorrection(icalculate, BasePin, IbConst);
        double IcmA = getShuntcurrent(icalculate, convertToVolt(IcVolt), CollectorPin);
        tmp.push_back(IcmA);
    }
    return tmp;
}
std::vector<double> DAC::PNPLoop(ICALCULATE *icalculate, double IbConst)
{
    std::vector<double> tmp;
    baseVoltage = 0;
    uint8_t IcChannel = channelSearch(CollectorPin);
    for (uint32_t IcVolt = UINT16_MAX; IcVolt >= 300; IcVolt -= UINT16_MAX / 200)
    {
        setVoltageOnChannel((uint16_t)IcVolt, IcChannel);
        PIDCorrection(icalculate, BasePin, IbConst);
        double IcmA = getShuntcurrent(icalculate, convertToVolt(UINT16_MAX - IcVolt), CollectorPin);
        tmp.push_back(IcmA);
    }
    return tmp;
}

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
        measureMode[basePin] = '6';
    else
        measureMode[basePin] = '5';
    measureMode[resCollector.at(0)] = '2';
    measureMode[resEmmiter.at(0)] = '1';
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    return double((3.3 - measurement.at(basePin)) / RESISTOR_LOW) * 1000.0;
}

double DAC::getShuntcurrent(ICALCULATE *icalculate, double supplyVoltage, int Pin)
{
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat();

    if (Pin == BasePin)
    {
        return double((double)(supplyVoltage - measurement.at(Pin)) / RESISTOR_HIGH) * 1000.0;
    }
    return double((double)(supplyVoltage - measurement.at(Pin)) / RESISTOR_LOW) * 1000.0;
}

void DAC::PIDCorrection(ICALCULATE *icalculate, int basePin, float ConstCurrentVal)
{
    pid = new PID(0.01);
    double currentBasemA;
    double error;
    double pidCorr;
    double reqVolt;
    double maxPID = pid->pid_process(30);
    uint8_t baseCommand = channelSearch(basePin);
    for (int i = 0; i < 10; i++)
    {
        currentBasemA = getShuntcurrent(icalculate, convertToVolt(baseVoltage), basePin);
        error = ConstCurrentVal - currentBasemA;
        pidCorr = pid->pid_process(error);
        pidCorr = (pidCorr / maxPID);
        reqVolt = convertToVolt(baseVoltage) + 3.3 * pidCorr;
        if (std::abs(error) < 0.01)
        {
            return;
        }
        if (reqVolt > 3.3)
            reqVolt = 3.3;
        if (reqVolt < 0)
            reqVolt = 0;
        baseVoltage = (int)((reqVolt / 3.3) * UINT16_MAX);
        setVoltageOnChannel(baseVoltage, baseCommand);
    }
}

double DAC::convertToVolt(uint16_t baseVoltage)
{
    return (double)((double)3.3 * (double)((double)baseVoltage / (double)UINT16_MAX));
}