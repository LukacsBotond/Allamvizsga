#include "./include/ADCCorrecter.h"

void ADCCORRECTER::init()
{
    try
    {
        // get "GND" measurement
        this->gndOffset = (getBaseValue(5, 0.1, 1) / 3.3) * 4096;
        // get "VCC" measurement
        this->VCCOffset = 3.3 / getBaseValue(4, 3.1, 0);
    }
    catch (CELL &e)
    {
        std::cout << "Cell found \n";
    }
    std::cout << "gnd offset: " << gndOffset << "VCCoffset: " << VCCOffset << std::endl;
}

uint16_t *ADCCORRECTER::offsetCorrection(uint16_t *samples,const uint16_t samplesSize)
{
    if (samplesSize == 0 || samples == nullptr)
    {
        throw NOTSUPPOSEDTOREACHTHIS("ADC correcter got samplesize 0 or null array");
    }
    for (int i = 0; i < samplesSize; i++)
    {
        samples[i] = (samples[i] * VCCOffset) - gndOffset;
    }
    return samples;
}

//* --------------------- Private functions -------------
double ADCCORRECTER::getBaseValue(const int8_t swMode,const double reqVoltage,const bool bigger)
{
    // get "GND" measurement
    icalculate->SameOut3ChannelRepeat(swMode, swMode, swMode);
    std::cout << "start calibration\n";
    std::vector<double> measurement = icalculate->getMeasurement(std::to_string(swMode) + std::to_string(swMode) + std::to_string(swMode));
    for (int i = 0; i < 3; i++)
    {
        if ((measurement.at(0) < reqVoltage || measurement.at(1) < reqVoltage || measurement.at(2) < reqVoltage) ^ bigger)
        {//wait till a potential capacitor discharges
            sleep_ms(250);
        }
        else// voltage reached the desierable level so start measurement
        {
            return (measurement.at(0) + measurement.at(1) + measurement.at(2)) / 3.0;
        }
    }
    throw CELL("Discharge capacitors before connecting it to the tester");
    //return -1;
}