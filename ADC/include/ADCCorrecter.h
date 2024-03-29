#pragma once
#include "./IADCCorrecter.h"

class ADCCORRECTER : public IADCORRECTER
{
private:
    double gndOffset = 0;
    double VCCOffset = 1;
    // if all 3 pins are on the ground then the voltage should be 0 on the ADC pins
    // possible exception is when a charged capacitor is connected, it tries to discharge it
    // but if it fails then gives a CELL exception
    double getBaseValue(const int8_t swMode,const double reqVoltage,const bool bigger);

public:
    ADCCORRECTER(){}
    ~ADCCORRECTER(){}
    void init() override;
    uint16_t *offsetCorrection(uint16_t *samples,const uint16_t samplesSize) override;
};