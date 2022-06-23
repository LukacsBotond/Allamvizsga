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
    std::vector<double> NPNLoop(ICALCULATE *icalculate);
    std::vector<double> PNPLoop(ICALCULATE *icalculate);

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
