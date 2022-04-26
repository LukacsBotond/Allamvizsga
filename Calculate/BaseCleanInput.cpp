#include "./include/BaseCleanInput.h"

bool BASECLEANINPUT::IsAnythingConnected(const double avgVoltage, const uint8_t portMode) const
{
    // port sends 3.3V on a pin, if it passes through a resistor without voltage
    // drop then there is nothing connected or high impedance as trying
    // to send voltage on a diode backwards
    if (portMode == 0)
    {
        return (avgVoltage > 3);
    }
    if (portMode == 2 || portMode == 4)
    {
        return (avgVoltage < 3.1);
    }
    //
    return (avgVoltage > 0.1);
}
