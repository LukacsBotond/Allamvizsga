#pragma once

#include "./ISwitchController.h"
#include "./NothingDAC.h"
#include "../../Calculate/include/ICalculate.h"
#include "../../Exceptions/include/NotSupposedToReachThis.h"
#include "./IDAC.h"

/*
    Measurement mode table
    mode   swSetting   voltages
    0 ->    Z          0V
    1 ->    1          0V
    2 ->    1          3.3V
    3 ->    2          0V
    4 ->    2          3.3V
    5 ->    3          0V
    6 ->    3          3.3V
*/

class ASWITCHCONTROLLER : public ISWITCHCONTROLLER
{
private:
    IDAC *idac;
    uint8_t outPort1 = 0;
    uint8_t outPort2 = 0;
    uint8_t outPort3 = 0;
    const uint32_t mask = 0 | (3 << SWITHCH1_1) | (3 << SWITHCH2_1) | (3 << SWITHCH3_1);

public:
    explicit ASWITCHCONTROLLER(IDAC *idac);
    ~ASWITCHCONTROLLER();
    /*
    uses bit masking to set the swithc pins, value is set by
    0 | (sw1 << 16) | (sw2 << 18) | (sw3 << 20) where the sws can be between 0 and 3
    @param value: const uint32_t the calculated value with the above function
    */
    void setSwithcSetting(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3) override;
    /*@param swNum which switch we are intrested in
    if incorrect value was given it van generate a NOTSUPPOSEDTOREACHTHIS exception
    @return that switch operating mode
    */
    uint8_t getSwithcSetting(const uint8_t swNum) const override;

    /*
        set voltage of the DAC, 
        @param voltage: const uint16_t sets the voltage, 0xffff 3.3V, 0 0V
        @param command: can be found in #defines in IDAC.h
    */
    void setVoltage(const uint16_t voltage, const uint8_t command) override;
};
