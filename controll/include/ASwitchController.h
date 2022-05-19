#pragma once

#include "./ISwitchController.h"
#include "./NothingDAC.h"
#include "../../Calculate/include/ICalculate.h"
#include "../../Exceptions/include/NotSupposedToReachThis.h"
#include "./IDAC.h"

//works for both Analog and resistor swich mode
class ASWITCHCONTROLLER : public ISWITCHCONTROLLER
{
private:
    IASWITCH *aswitch1;
    IASWITCH *aswitch2;
    IASWITCH *aswitch3;
    IDAC* idac;

public:
    ASWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3);
    ASWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3, IDAC *idac);
    ASWITCHCONTROLLER(ASWITCHCONTROLLER &copy);
    ~ASWITCHCONTROLLER();

    void setSwithcSetting(const uint32_t mask, const uint32_t value) override;

    void setSwithcSetting(const uint8_t swNum, const uint8_t mode) override;
    uint8_t getSwithcSetting(const uint8_t swNum) const override;
    
    uint getResistorSetting(const uint8_t swNum,const uint8_t resistorNr) const override;
    double getswTotResistorSetting(uint8_t swNum) const  override;
    double getTotResistorFromMode(uint8_t mode) const override;
};
