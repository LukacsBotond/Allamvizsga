#include "./include/BaseCleanInput.h"
#include <iostream>
#include <numeric> // std::accumulate

double BASECLEANINPUT::AVGVoltage(uint16_t *samples, uint16_t samplesSize)
{
    if (samples == nullptr || samplesSize < 1)
    {
        return 0;
    }
    double sum = 0;
    sum = std::accumulate(samples, samples + samplesSize, sum);
    return (((double)sum / samplesSize) * 3.3) / 4096.0;
}

bool BASECLEANINPUT::IsAnythingConnected(double avgVoltage, uint8_t portMode)
{
    //port sends 3.3V on a pin, if it passes through a resistor without volate
    //drop then there is nothing connected or high impedance as trying
    //to send voltage on a diode backwards
    if (portMode == 2 || portMode == 4)
    {
        return (avgVoltage < 3.1);
    }
    //
    return (avgVoltage > 0.02);
}
