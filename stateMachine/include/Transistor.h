#pragma once
#include "State.h"
#include <vector>
#include <iostream>
#include <string>

class TRANSISTOR : public STATE
{
private:
public:
    explicit TRANSISTOR(ICALCULATE *icalculate);
    ~TRANSISTOR() {}

    bool check() override;

    void calculate() override;
};

TRANSISTOR::TRANSISTOR(ICALCULATE *icalculate)
{
    this->icalculate = icalculate;
}

bool TRANSISTOR::check()
{
    std::string modes[] = {"000", "111", "161", "166", "116", "611", "616", "661", "666"};

    for (int i = 0; i < 9; i++)
    {
        icalculate->SameOut3ChannelRepeat(modes[i][0] - '0', modes[i][1] - '0', modes[i][2] - '0', true);
    }

    icalculate->values->printMeasurements();
    return false;
}

void TRANSISTOR::calculate()
{
    check();
}