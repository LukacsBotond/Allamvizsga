#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

class ADC{
    public:

        ADC();
        ~ADC();

        void adcSelect(int chanel);
};