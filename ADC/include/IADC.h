#pragma once
#include <iostream>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "../../Global.h"

#include "../../Exceptions/include/NoSuchPort.h"
#include "../../Exceptions/include/NullExept.h"

