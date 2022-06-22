#include "./include/ACleanInput.h"

bool ACLEANINPUT::IsAnythingConnected(const double avgVoltage, const uint8_t portMode) const
{
    // port sends 3.3V on a pin, if it passes through a resistor without voltage
    // drop then there is nothing connected or high impedance
    if (portMode == 0)
    {
        return (avgVoltage > 0.5 && avgVoltage < 0.9);
    }
    if (portMode % 2 == 1) // pin is on ground
    {
        return (avgVoltage > 0.1);
    }
    //
    return (avgVoltage < 3.15);
}
