#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>

class CAPACITOR : public STATE
{
private:
    /* data */
public:
    explicit CAPACITOR(ICALCULATE *icalculate);
    ~CAPACITOR() {}

    bool check() override;

    std::map<std::string, double> calculate() override;
};

CAPACITOR::CAPACITOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool CAPACITOR::check()
{
    std::string modes[] = {"650", "610", "210"};
    int div[] = {96, 96 * 10, 96 * 100, 96 * 1000, 96 * 10000, 96 * 100000};
    double get;
    for (int8_t i = 0; i < 3; i++)
    {
        for (int8_t j = 0; j < 6; j++)
        {
            std::cout << "Current mode: " << modes[i] << " " << (int)div[j] << std::endl;
            adc->set_clkDiv(div[j]);
            this->icalculate->ChannelMeasure(modes[i][0] - '0', modes[i][1] - '0', modes[i][2] - '0', 2, false);
            sleep_ms(500);

            //*check if it is drained
            while (1)
            {
                std::cout << "Waiting to drain.....\n";
                adc->set_clkDiv(96000);
                get = this->icalculate->ChannelMeasure(1, 1, 1, 2, false);
                if (get < 0.1)
                {
                    break;
                }
            }
        }
    }
    return true;
}

std::map<std::string, double> CAPACITOR::calculate()
{
    std::map<std::string, double> ret;
    return ret;
}
