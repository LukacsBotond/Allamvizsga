// #pragma once
// #include "./IAswitch.h"

// //! when using resistors and not an analog switch
// class BASESWITCH : public IASWITCH
// {
// private:
//     uint outRes1;
//     uint outRes2;
//     uint outRes3;
//     uint gpio1;
//     uint gpio2;
//     uint outPort; //current port mode

// public:
//     BASESWITCH(uint outRes1, uint outRes2, uint outRes3, uint gpio1, uint gpio2);
//     //give values to the output resistors
//     void selectOutput(const uint8_t port) override;
//     //value sent to the analog switch
//     uint8_t getOutput() const  override;
//     uint getResistor(uint8_t nr) const override;
//     double getTotSwitchResistance() const override;
//     double getTotSwitchResistanceFromMode(uint8_t mode) const override;
// };