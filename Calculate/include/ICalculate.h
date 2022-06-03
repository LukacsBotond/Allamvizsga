#pragma once
#include "./IValues.h"
#include "../../Exceptions/include/NoSuchMeasurement.h"
#include "../../ADC/include/ADCCorrecter.h"
#include "./ICleanInput.h"
#include "../../ADC/include/IADC.h"
#include "../../controll/include/ISwitchController.h"
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


    virtual double ChannelMeasure(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3,uint32_t channelId,  bool saveMeasurement = true) = 0;

    virtual std::vector<double> SameOut3ChannelRepeat(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3, bool saveMeasurement = true) = 0;

    /*
        Give a set of measurement modes where the component is a resistor and calculate
        the resistance from the voltage drops
        @param measurements: std::vector<std::string>& vector of the used port modes
        @return resistance: double
    */
    virtual double calcResistance(std::vector<std::string> &measurements) = 0;

    /*
        Give a measurement mode where the component is a diode to calculate the
        threshold voltage
        @param measurements: std::string udes measurement mode
        @return threshold voltage: double
        Can throw NOSUCHMEASUREMENT if there was no such measurement made before
    */
    virtual double diodeThreshold(std::string &measurement) = 0;

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
    virtual bool setMeasurement(const std::string &measurement, const std::vector<double> &valuesVector) = 0;

    /*
        Deletes all stored measurements
    */
    virtual void cleanMesurements() = 0;

    void startSemaphoreRelease(bool prep = false);
    void doneSemaphoreAquire(bool prep = false);

    // ICLEANINPUT interface
    virtual bool IsAnythingConnected(const double avgVoltage, const uint8_t portMode) = 0;
};
