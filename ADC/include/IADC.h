#pragma once
#include <iostream>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

class IADC
{
public:
    virtual void setupFIFO() = 0;
    virtual void waitDMAFull() = 0;
    virtual void adcSelect(int chanel) = 0;
    virtual void start_freeRunning() = 0;
    virtual void stop_freeRunning() = 0;

    //! debug
    virtual void printSamples() = 0;
};