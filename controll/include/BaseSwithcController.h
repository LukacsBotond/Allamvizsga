#pragma once

#include "./ISwitchController.h"
#include "../../Calculate/include/ICalculate.h"
#include "pico/sem.h"
#include "pico/util/queue.h"

class BASESWITCHCONTROLLER : public ISWITCHCONTROLLER
{
private:
    IASWITCH *aswitch1;
    IASWITCH *aswitch2;
    IASWITCH *aswitch3;
    ICALCULATE *calc;
    //struct semaphore *startSemaphore;
    //struct semaphore *doneSemaphore;

public:
    BASESWITCHCONTROLLER(IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3, ICALCULATE* calc);
    ~BASESWITCHCONTROLLER();
    void SameOut3ChannelRepeat(uint8_t sw1P, uint8_t sw2P, uint8_t sw3P) override;
};