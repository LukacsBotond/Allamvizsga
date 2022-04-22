#pragma once
#include "./IValues.h"
#include "../../Exceptions/include/NoSuchMeasurement.h"
#include "../../ADC/include/ADCCorrecter.h"
#include "./ICleanInput.h"
#include "../../ADC/include/IADC.h"
#include "../../controll/include/IAswitch.h"
#include "../../controll/include/BaseSwithcController.h"
#include <vector>

class IADCORRECTER;
class ICALCULATE
{
protected:
public:
    static IVALUES *values;
    static ICLEANINPUT *cleanup;
    static ISWITCHCONTROLLER *controller;
    static IADCORRECTER *adccorrecter;
    virtual ~ICALCULATE() {}

    virtual void SameOut3ChannelRepeat(const uint8_t sw1,const uint8_t sw2,const uint8_t sw3) = 0;
    virtual double calcResistance(std::vector<std::string> &measurements) = 0;
    

    /*
        Get the avarage measurement by the used pin mode
        @param measurement: string, the used pin mode in which the measurement was stored

        @return std::vector<double> the avaraged voltage of that setting if exist THROW NOSUCHMEASUREMENT if there is no such saved measurement
    */
    virtual std::vector<double> getMeasurement(const std::string &measurement) const = 0;
    
    /*
        stores the avg values of a measurement
        @param measurement: string, it names the measurement, by the used port modes
        @param valuesVector: vector<double>, stores the 3 values measured on each pin
        @return bool if it was succesfull
    */    
    virtual bool setMeasurement(const std::string &measurement, std::vector<double> valuesVector) = 0;
    
    /*
        Deletes all stored measurements
    */
    virtual void cleanMesurements() = 0;

    void startSemaphoreRelease();
    void doneSemaphoreAquire();

    // ICLEANINPUT interface
    virtual bool IsAnythingConnected(const double avgVoltage,const uint8_t portMode) = 0;
};
