#include "./include/ASwitchController.h"
#include "../Global.h"
#include <iostream>
#include "pico/multicore.h"

ASWITCHCONTROLLER::ASWITCHCONTROLLER(IDAC *idac)
    : idac(idac)
{
    SwSettingVoltagePair tmp;
    tmp.setting = 0;
    tmp.voltage = 0;
    Sw_translation_Map[0] = tmp;
    tmp.setting = 1;
    Sw_translation_Map[1] = tmp;
    tmp.setting = 2;
    Sw_translation_Map[3] = tmp;
    tmp.setting = 3;
    Sw_translation_Map[5] = tmp;
    tmp.setting = 1;
    tmp.voltage = 0xffff;
    Sw_translation_Map[2] = tmp;
    tmp.setting = 2;
    Sw_translation_Map[4] = tmp;
    tmp.setting = 3;
    Sw_translation_Map[6] = tmp;
    gpio_init_mask(this->mask);
    gpio_set_dir_out_masked(this->mask);
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
    // set supply voltage
    setVoltage(Sw_translation_Map[sw1].voltage, DAC_COMM_WRITE_BUFF_B);
    setVoltage(Sw_translation_Map[sw2].voltage, DAC_COMM_WRITE_BUFF_C);
    setVoltage(Sw_translation_Map[sw3].voltage, DAC_COMM_WRITE_BUFF_LOAD_ALL_D);
    // set switch
    gpio_put_masked(this->mask, 0);
    gpio_put_masked(this->mask, 0 | (Sw_translation_Map[sw1].setting << 16) | (Sw_translation_Map[sw2].setting << 18) | (Sw_translation_Map[sw3].setting << 20));
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