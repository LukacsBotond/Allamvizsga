#pragma once

#include "./ISwitchController.h"
#include "../../Calculate/include/ICalculate.h"
#include "../../Exceptions/include/NotSupposedToReachThis.h"
#include "pico/sem.h"
#include "pico/util/queue.h"

class BASESWITCHCONTROLLER : public ISWITCHCONTROLLER
{
private:
    IASWITCH *aswitch1;
    IASWITCH *aswitch2;
    IASWITCH *aswitch3;
public:
    BASESWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3);
    ~BASESWITCHCONTROLLER();

    //! TODO implement this in CALCULATE class
    //void SameOut3ChannelRepeat(uint8_t sw1P, uint8_t sw2P, uint8_t sw3P) override;

    void setSwithcSetting(uint8_t swNum, uint8_t mode) override;
    uint8_t getSwithcSetting(uint8_t swNum) override;
    uint getResistorSetting(uint8_t swNum, uint8_t resistorNr) override;
    double getswTotResistorSetting(uint8_t swNum) override;
};
