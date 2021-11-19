#pragma once
#include "./IAswitch.h"

class ASWITHCH : public IASWITCH
{
    uint outRes1;
    uint outRes2;
    uint outRes3;
    uint gpio1;
    uint gpio2;
public:
    ASWITHCH(uint outRes1,uint outRes2,uint outRes3, uint gpio1, uint gpio2);

    void selectOutput(uint port) override;

};