#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>

class CAPACITOR : public STATE
{
private:
    void drainCapacitor();

public:
    explicit CAPACITOR(ICALCULATE *icalculate);
    ~CAPACITOR() {}

    bool check() override;

    void calculate() override;
};

CAPACITOR::CAPACITOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool CAPACITOR::check()
{
    try
    {
        this->icalculate->CalcCapacitance_nF(this->icalculate->ChannelMeasure(6, 1, 0, 2, false), adc->getCaptureDepth(), adc->get_clkHz(), 6);
        this->icalculate->CalcCapacitance_nF(this->icalculate->ChannelMeasure(1, 6, 0, 1, false), adc->getCaptureDepth(), adc->get_clkHz(), 6);
        this->icalculate->CalcCapacitance_nF(this->icalculate->ChannelMeasure(0, 1, 6, 0, false), adc->getCaptureDepth(), adc->get_clkHz(), 6);
    }
    catch (CAPACITORSLOWCHARGE &e)
    {
        return true;
    }
    return false;
}

void CAPACITOR::calculate()
{
    std::string modes[] = {"610", "210", "160", "120", "016", "012"};
    int div[] = {96, 96 * 10, 96 * 100, 96 * 1000, 96 * 10000, 96 * 100000};

    for (int8_t i = 0; i < 6; i++)
    {
        for (int8_t j = 0; j < 6; j++)
        {
            // std::cout << "Current mode: " << modes[i] << " " << (int)div[j] << std::endl;
            adc->set_clkDiv(div[j]);
            try
            {
                double result = this->icalculate->CalcCapacitance_nF(this->icalculate->ChannelMeasure(modes[i][0] - '0', modes[i][1] - '0', modes[i][2] - '0', i / 2, false), adc->getCaptureDepth(), adc->get_clkHz(), modes[i][2 - i / 2] - '0');
                if (result > 10)
                { // less than 10nF is just too small
                    this->results.clear();
                    this->results["Capacitor"] = 0;
                    results["Capacitance" + std::to_string(i / 2) + " nF"] = result;
                    setUsedPins(modes[i],'C');
                }
                return;
            }
            catch (CAPACITORSLOWCHARGE &e)
            {
            }
            // sleep_ms(500);
            drainCapacitor();
        }
    }
}

//* private

void CAPACITOR::drainCapacitor()
{

    //*check if it is drained
    while (1)
    {
        double get;
        // std::cout << "Waiting to drain.....\n";
        adc->set_clkDiv(96000);
        get = cleanInput->AVGVoltage(this->icalculate->ChannelMeasure(1, 1, 1, 2, false), adc->getCaptureDepth());
        if (get < 0.1)
        {
            return;
        }
    }
}
