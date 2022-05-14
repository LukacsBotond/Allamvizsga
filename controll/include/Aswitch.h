#pragma once
#include "./IAswitch.h"
#include "../../Exceptions/include/NoSuchPort.h"


/*
0,0->OFF
0,1->NO0 on
1,0->NO1 on
1,1->NO2 on
*/


//!Analog IC swich mode
class ASWITCH : public IASWITCH
{
    uint outRes1;
    uint outRes2;
    uint outRes3;
    uint8_t gpio1;
    uint8_t gpio2;
    uint8_t outPort; //current port mode

public:
    /*Inicialize to NC- inputs are GND
    @param outRes1: uint Lowest resistor
        outRes2: uint Mid resistor
        outRes3: uint High resistor
        gpio1: IN1 GPIO pin
        gpio2: IN2 GPIO pin
    */ 
    ASWITCH(uint outRes1, uint outRes2, uint outRes3, uint8_t gpio1, uint8_t gpio2);
    //give values to the output resistors
    void selectOutput(const uint8_t port) override;
    //value sent to the analog switch
    uint8_t getOutput() const  override;
    uint getResistor(uint8_t nr) const override;
    double getTotSwitchResistance() const override;
    double getTotSwitchResistanceFromMode(uint8_t mode) const override;
};