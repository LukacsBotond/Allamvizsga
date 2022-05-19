#include "./include/ASwithcController.h"
#include "../Global.h"
#include <iostream>
#include "pico/multicore.h"

ASWITCHCONTROLLER::ASWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3)
    : aswitch1(sw1), aswitch2(sw2), aswitch3(sw3)
{
    this->idac = new NothingDAC();
}

ASWITCHCONTROLLER::ASWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3, IDAC *idac)
    : aswitch1(sw1), aswitch2(sw2), aswitch3(sw3), idac(idac)
{
}

ASWITCHCONTROLLER::~ASWITCHCONTROLLER()
{
    delete aswitch1;
    delete aswitch2;
    delete aswitch3;
    delete idac;
}

void ASWITCHCONTROLLER::setSwithcSetting(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3)
{
    gpio_put_masked(this->mask, 0 | (sw1 << 16) | (sw2 << 18) | (sw3 << 20));
}

void ASWITCHCONTROLLER::setSwithcSetting(uint8_t swNum, uint8_t mode)
{
    switch (swNum)
    {
    case 1:
        return this->aswitch1->selectOutput(mode);
    case 2:
        return this->aswitch2->selectOutput(mode);
    case 3:
        return this->aswitch3->selectOutput(mode);
    default:
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch setSwitchSetting out of range 0-3 only, got" + swNum);
    }
}

uint8_t ASWITCHCONTROLLER::getSwithcSetting(uint8_t swNum) const
{
    switch (swNum)
    {
    case 1:
        return this->aswitch1->getOutput();
    case 2:
        return this->aswitch2->getOutput();
    case 3:
        return this->aswitch3->getOutput();
    default:
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch getSwitchSetting out of range 0-3 only, got" + swNum);
    }
}

uint ASWITCHCONTROLLER::getResistorSetting(uint8_t swNum, uint8_t resistorNr) const
{
    switch (swNum)
    {
    case 1:
        return this->aswitch1->getResistor(resistorNr);
    case 2:
        return this->aswitch2->getResistor(resistorNr);
    case 3:
        return this->aswitch3->getResistor(resistorNr);
    default:
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch getSwitchSetting out of range 0-3 only, got" + swNum);
    }
}

double ASWITCHCONTROLLER::getswTotResistorSetting(uint8_t swNum) const
{
    switch (swNum)
    {
    case 1:
        return this->aswitch1->getTotSwitchResistance();
    case 2:
        return this->aswitch2->getTotSwitchResistance();
    case 3:
        return this->aswitch3->getTotSwitchResistance();
    default:
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch getSwitchSetting out of range 0-3 only, got" + swNum);
    }
}

double ASWITCHCONTROLLER::getTotResistorFromMode(uint8_t mode) const
{
    return this->aswitch1->getTotSwitchResistance(mode);
}
