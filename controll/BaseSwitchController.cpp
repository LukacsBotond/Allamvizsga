#include "./include/BaseSwithcController.h"
#include "../Global.h"
#include <iostream>
#include "pico/multicore.h"

BASESWITCHCONTROLLER::BASESWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3,ICALCULATE* calc)
    : aswitch1(sw1), aswitch2(sw2), aswitch3(sw3), calc(calc)
{
}

BASESWITCHCONTROLLER::~BASESWITCHCONTROLLER()
{
    delete aswitch1;
    delete aswitch2;
    delete aswitch3;
}
