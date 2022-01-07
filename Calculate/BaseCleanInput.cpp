#include "./include/BaseCleanInput.h"
#include <iostream>
#include <numeric> // std::accumulate

double BASECLEANINPUT::AVGVoltage(uint16_t *samples, uint16_t samplesSize)
{
    double sum = 0;
    sum = std::accumulate(samples, samples + samplesSize, sum);
    return (((double)sum / samplesSize) * 3.3) / 4096.0;
}