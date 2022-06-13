#pragma once

#include <iostream>
#include <math.h>

class PID
{
private:
    double kp;
    double ki;
    double kd;
    double integrator;
    double previous_error;
    double integrator_limit;
    double frequency;

public:
    /** Initializes a PID controller. */
    PID();

    PID(float kp, float ki, float kd, float frequency);
    ~PID() {}

    void pid_set_gains(float kp, float ki, float kd);

    /** Process one step if the PID algorithm. */
    float pid_process(float error);

    /** Resets the PID integrator to zero. */
    void pid_reset_integral();
};

PID::PID()
{
    kp = 1;
    ki = 0;
    kd = 0;
    integrator = 0;
    previous_error = 0;
    integrator_limit = INFINITY;
    frequency = 1;
}

PID::PID(float kp, float ki, float kd, float frequency) : kp(kp), ki(ki), kd(kd), frequency(frequency)
{
    integrator = 0;
    previous_error = 0;
    integrator_limit = INFINITY;
}

void PID::pid_set_gains(float kp, float ki, float kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

float PID::pid_process(float error)
{
    float output;
    integrator += error;

    if (integrator > integrator_limit)
    {
        integrator = integrator_limit;
    }
    else if (integrator < -integrator_limit)
    {
        integrator = -integrator_limit;
    }

    output = -kp * error;
    output += -ki * integrator / frequency;
    output += -kd * (error - previous_error) * frequency;

    previous_error = error;
    return output;
}

void PID::pid_reset_integral()
{
    integrator = 0;
}