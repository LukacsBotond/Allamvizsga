#pragma once
#include "./IAswitch.h"
#include "../../Exceptions/include/NoSuchPort.h"

class ASWITCH : public IASWITCH
{
    uint outRes1;
    uint outRes2;
    uint outRes3;
    uint gpio1;
    uint gpio2;

public:
    //initialize high Z
    ASWITCH(uint outRes1, uint outRes2, uint outRes3, uint gpio1, uint gpio2);

    //select analog swithch
    void selectOutput(uint_8 port) override;
    //value sent to the analog switch
    uint8_t getOutput() override;
    uint getResistor(uint8_t nr) override;
};