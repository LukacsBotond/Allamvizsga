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
    uint outPort;
public:
    BASESWITCH(uint outRes1, uint outRes2, uint outRes3, uint gpio1, uint gpio2);

    //give values to the output resistors
    virtual void selectOutput(uint8_t port);

    uint8_t getOutput() override;
    uint getResistor(uint8_t nr) override;
};