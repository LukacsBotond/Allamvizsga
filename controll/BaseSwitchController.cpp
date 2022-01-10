#include "./include/BaseSwithcController.h"
#include "../Global.h"
#include <iostream>
#include "pico/multicore.h"

BASESWITCHCONTROLLER::BASESWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3)
    : aswitch1(sw1), aswitch2(sw2), aswitch3(sw3)
{
}

BASESWITCHCONTROLLER::~BASESWITCHCONTROLLER()
{
    delete aswitch1;
    delete aswitch2;
    delete aswitch3;
}
void BASESWITCHCONTROLLER::setSwithcSetting(uint8_t swNum, uint8_t mode)
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
        std::string swNumstr = std::to_string(swNum);
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch setSwitchSetting out of range 0-3 only, got" + swNum);
    }
}

uint8_t BASESWITCHCONTROLLER::getSwithcSetting(uint8_t swNum)
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
        std::string swNumstr = std::to_string(swNum);
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch getSwitchSetting out of range 0-3 only, got" + swNum);
    }
}

uint BASESWITCHCONTROLLER::getResistorSetting(uint8_t swNum, uint8_t resistorNr)
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
        std::string swNumstr = std::to_string(swNum);
        throw NOTSUPPOSEDTOREACHTHIS("BaseSwitch getSwitchSetting out of range 0-3 only, got" + swNum);
    }
}
