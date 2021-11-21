#pragma one
#include "./IAswitch.h"

//! when using resistors and not an analog switch
class BASESWITCH : public IASWITCH
{
private:
    uint outRes1;
    uint outRes2;
    uint outRes3;
    uint gpio1;
    uint gpio2;
public:
    BASESWITCH(uint outRes1, uint outRes2, uint outRes3, uint gpio1, uint gpio2);

    virtual void selectOutput(uint port);
};