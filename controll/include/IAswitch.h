#pragma once
#include <stdio.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

class IASWITCH{
    public:
        virtual void selectOutput(uint port);
};