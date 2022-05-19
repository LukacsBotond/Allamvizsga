// #pragma once
// #include "./IAswitch.h"
// #include "../../Exceptions/include/NoSuchPort.h"

// /*
// 0,0->OFF
// 0,1->NO0 on
// 1,0->NO1 on
// 1,1->NO2 on
// */

// //! Analog IC swich mode
// class ASWITCH : public IASWITCH
// {
//     uint outResLow;
//     uint outResMid;
//     uint outResHigh;
//     uint8_t gpioLs;
//     uint8_t gpioMs;
//     uint8_t outPort; // current port mode

// public:
//     /*Inicialize to NC- inputs are GND
//     @param outResLow: uint Lowest resistor
//         outResMid: uint Mid resistor
//         outResHigh: uint High resistor
//         gpioLs: IN1 GPIO pin
//         gpioMs: IN2 GPIO pin
//     */
//     ASWITCH(uint outResLow, uint outResMid, uint outResHigh, uint8_t gpioLs, uint8_t gpioMs);
//     // give values to the output resistors
//     void selectOutput(const uint8_t port) override;
//     // value sent to the analog switch
//     uint8_t getOutput() const override;
//     uint getResistor(uint8_t nr) const override;
//     double getTotSwitchResistance() const override;
//     double getTotSwitchResistance(uint8_t mode) const override;
// };