#include "./include/PID.h"

PID::PID(float kp) : kp(kp)
{
}

float PID::pid_process(float error)
{
    return -kp * error;
}
