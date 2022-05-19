#include "./include/ASwitchController.h"
#include "../Global.h"
#include <iostream>
#include "pico/multicore.h"

ASWITCHCONTROLLER::ASWITCHCONTROLLER(IDAC *idac)
    : idac(idac)
{
}

ASWITCHCONTROLLER::~ASWITCHCONTROLLER()
{
    delete idac;
}

void ASWITCHCONTROLLER::setSwithcSetting(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3)
{
    outPort1 = sw1;
    outPort2 = sw2;
    outPort3 = sw3;
    gpio_put_masked(this->mask, 0 | (sw1 << 16) | (sw2 << 18) | (sw3 << 20));
}

uint8_t ASWITCHCONTROLLER::getSwithcSetting(uint8_t swNum) const
{
    switch (swNum)
    {
    case 1:
        return outPort1;
    case 2:
        return outPort2;
    case 3:
        return outPort3;
    default:
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch getSwitchSetting out of range 0-3 only, got" + swNum);
    }
}

void ASWITCHCONTROLLER::setVoltage(const uint16_t voltage, const uint8_t command)
{
    this->idac->setVoltageOnChannel(voltage, command);
}