#pragma once

#include "./ISwitchController.h"
#include "./NothingDAC.h"
#include "../../Calculate/include/ICalculate.h"
#include "../../Exceptions/include/NotSupposedToReachThis.h"
#include "./IDAC.h"

/*
    Measurement mode table
    mode   swSetting   voltages
    0 ->    0          0V
    1 ->    1          0V
    2 ->    1          3.3V
    3 ->    2          0V
    4 ->    2          3.3V
    5 ->    3          0V
    6 ->    3          3.3V
*/

struct SwSettingVoltagePair
{
public:
    uint8_t setting;
    uint16_t voltage;
};

class ASWITCHCONTROLLER : public ISWITCHCONTROLLER
{
private:
    std::map<uint8_t, SwSettingVoltagePair> Sw_translation_Map;
    IDAC *idac;
    uint8_t outPort1 = 0;
    uint8_t outPort2 = 0;
    uint8_t outPort3 = 0;
    const uint32_t mask = 0x003f0000;

public:
    explicit ASWITCHCONTROLLER(IDAC *idac);
    ~ASWITCHCONTROLLER();
    /*
    With the switch mode as the parameter use the translation map to get the switch setting and
    the supply voltage for that channel
    @param sw1:const uint8_t swith mode for switch 1
    @param sw2:const uint8_t swith mode for switch 2
    @param sw3:const uint8_t swith mode for switch 3
    */
    void setSwithcSetting(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3) override;
    /*@param swNum which switch we are intrested in
    if incorrect value was given it van generate a NOTSUPPOSEDTOREACHTHIS exception
    @return that switch operating mode
    */
    uint8_t getSwithcSetting(const uint8_t swNum) const override;

    uint getTotResistor(uint8_t usedMode) override;
    /*
        set voltage of the DAC,
        @param voltage: const uint16_t sets the voltage, 0xffff 3.3V, 0 0V
        @param command: can be found in #defines in IDAC.h
    */
    void setVoltage(const uint16_t voltage, const uint8_t command) override;
};
