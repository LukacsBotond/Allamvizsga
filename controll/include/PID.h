#pragma once

#include <iostream>
#include <math.h>

class PID
{
private:
    double kp;
public:
    /** Initializes a PID controller. */
    explicit PID(float kp);
    ~PID() {}

    /** Process one step if the PID algorithm. */
    float pid_process(float error);
};
