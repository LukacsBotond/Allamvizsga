#include "./include/ICleanInput.h"


double ICLEANINPUT::AVGVoltage(uint16_t *samples, uint16_t samplesSize)
{
    if (samples == nullptr || samplesSize < 1)
    {
        return 0;
    }
    double sum = 0;
    sum = std::accumulate(samples, samples + samplesSize, sum);
    return (((double)sum / samplesSize) * 3.3) / 4096.0;
}
