#include "./include/DAC.h"

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
        //std::cout << "DAC characteristicDiagramm is only for transistors\n";
        throw NOTSUPPOSEDTOREACHTHIS("DAC characteristicDiagramm is only for transistors");
    }
    CharDiagr ret;
    adc->set_clkDiv(0);
    TRANSISTOR transistor(icalculate);
    std::vector<int> res = transistor.usedPinsFindByValue("B");
    BasePin = res.at(0);
    res = transistor.usedPinsFindByValue("C");
    CollectorPin = res.at(0);
    res = transistor.usedPinsFindByValue("E");
    EmitterPin = res.at(0);
    if (STATE::mainResult == "npn transistor")
        ret.data = NPNLoop(icalculate);
    else
        ret.data = PNPLoop(icalculate);
    ret.yScale = -1;
    return ret;
}

CharDiagr DAC::InputcharacteristicDiagramm(ICALCULATE *icalculate)
{
    CharDiagr ret;
    std::vector<double> tmp;
    measureMode[CollectorPin] = '1';
    measureMode[EmitterPin] = '1';
    if (STATE::mainResult == "npn transistor")
    {
        measureMode[BasePin] = '6';
    }
    else
    {
        measureMode[BasePin] = '5';
    }
    icalculate->controller->prepareSwitchSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->setSwithcSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    uint8_t IbChannel = channelSearch(BasePin);
    for (uint32_t IbVolt = 0; IbVolt < UINT16_MAX / 3; IbVolt += (UINT16_MAX / 3) / 200)
    {
        setVoltageOnChannel((uint16_t)IbVolt, IbChannel);
        // PIDCorrection(icalculate, BasePin, IbConst);
        double IbmA = getShuntcurrent(icalculate, convertToVolt((uint16_t)IbVolt), BasePin);
        tmp.push_back(IbmA);
    }
    ret.data = tmp;
    ret.yScale = -1;
    return ret;
}

CharDiagr DAC::Curent_Transf_Characteristic_Diagramm(ICALCULATE *icalculate)
{
    CharDiagr ret;
    std::vector<double> tmp;
    if (STATE::mainResult == "npn transistor")
    {
        measureMode[BasePin] = '6';
        measureMode[CollectorPin] = '2';
        measureMode[EmitterPin] = '1';
    }
    else
    {
        measureMode[BasePin] = '5';
        measureMode[CollectorPin] = '1';
        measureMode[EmitterPin] = '2';
    }

    icalculate->controller->prepareSwitchSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->setSwithcSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    uint8_t IbChannel = channelSearch(BasePin);
    if (STATE::mainResult == "npn transistor")
    {
        baseVoltage = ((double)UINT16_MAX / 3.3) * 2.4;
    }
    else
    {
        baseVoltage = ((double)UINT16_MAX / 3.3) * 0.7;
    }
    setVoltageOnChannel(baseVoltage, IbChannel);
    double IbCurrMax = getShuntcurrent(icalculate, convertToVolt(baseVoltage), BasePin);
    for (double IbCurr = 0; IbCurr < IbCurrMax; IbCurr += (IbCurrMax) / 200)
    {
        // setVoltageOnChannel((uint16_t)IbVolt, IbChannel);
        PIDCorrection(icalculate, BasePin, IbCurr);
        double IcmA = getShuntcurrent(icalculate, 3.3, CollectorPin);
        tmp.push_back(IcmA);
    }

    ret.data = tmp;
    ret.yScale = -1;
    return ret;
}

//*Private
std::vector<double> DAC::NPNLoop(ICALCULATE *icalculate)
{
    std::vector<double> tmp;
    baseVoltage = UINT16_MAX;
    double IbConst = getGatecurrent(icalculate, BasePin);
    icalculate->controller->prepareSwitchSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->setSwithcSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
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
std::vector<double> DAC::PNPLoop(ICALCULATE *icalculate)
{
    std::vector<double> tmp;
    baseVoltage = 0;
    double IbConst = getGatecurrent(icalculate, BasePin);
    // std::cout << "gateCurrent: " << IbConst << std::endl;
    icalculate->controller->prepareSwitchSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->setSwithcSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    uint8_t IcChannel = channelSearch(CollectorPin);
    for (uint32_t IcVolt = UINT16_MAX; IcVolt >= 300; IcVolt -= UINT16_MAX / 200)
    {
        setVoltageOnChannel((uint16_t)IcVolt, IcChannel);
        PIDCorrection(icalculate, BasePin, IbConst);
        double IemA = getShuntcurrent(icalculate, convertToVolt(UINT16_MAX), EmitterPin);
        tmp.push_back(IemA);
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

    if (STATE::mainResult == "npn transistor")
    {
        measureMode[BasePin] = '6';
        measureMode[CollectorPin] = '2';
        measureMode[EmitterPin] = '1';
    }
    else
    {
        measureMode[BasePin] = '5';
        measureMode[CollectorPin] = '1';
        measureMode[EmitterPin] = '2';
    }
    // double refCurrent;
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->prepareSwitchSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    icalculate->controller->setSwithcSetting(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    uint8_t baseCommand = channelSearch(basePin);
    while (std::abs(measurement[CollectorPin] - measurement[EmitterPin]) < 1.5)
    {
        // std::cout << "voltDif: " << std::abs(measurement[CollectorPin] - measurement[EmitterPin]) << " baseVolt:" << baseVoltage << std::endl;
        if (STATE::mainResult == "npn transistor")
            baseVoltage -= 200;
        else
            baseVoltage += 200;

        setVoltageOnChannel(baseVoltage, baseCommand);
        measurement = icalculate->SameOut3ChannelRepeat();
    }

    // std::vector<double> measurement = icalculate->SameOut3ChannelRepeat(measureMode[0] - '0', measureMode[1] - '0', measureMode[2] - '0');
    return double((double)(convertToVolt(baseVoltage) - measurement.at(BasePin)) / (double)RESISTOR_HIGH) * 1000.0;
}

double DAC::getShuntcurrent(ICALCULATE *icalculate, double supplyVoltage, int Pin)
{
    std::vector<double> measurement = icalculate->SameOut3ChannelRepeat();
    if (Pin == BasePin)
    {
        return double((double)(supplyVoltage - measurement.at(Pin)) / (double)RESISTOR_HIGH) * 1000.0;
    }
    return double((double)(supplyVoltage - measurement.at(Pin)) / (double)RESISTOR_LOW) * 1000.0;
}

void DAC::PIDCorrection(ICALCULATE *icalculate, int basePin, double ConstCurrentVal)
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
        //std::cout << "error: " << error << " baseVoltage " << baseVoltage << " currentBasemA " << currentBasemA << " ref:" << ConstCurrentVal << std::endl;
    }
}

double DAC::convertToVolt(uint16_t baseVoltage)
{
    return (double)((double)3.3 * (double)((double)baseVoltage / (double)UINT16_MAX));
}