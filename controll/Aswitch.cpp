#include "./include/Aswitch.h"
#include "../Exceptions/include/NoSuchPort.h"
#include "../Exceptions/include/NotSupposedToReachThis.h"

ASWITCH::ASWITCH(uint outRes1, uint outRes2, uint outRes3, uint8_t gpio1, uint8_t gpio2)
    : outRes1(outRes1), outRes2(outRes2), outRes3(outRes3), gpio1(gpio1), gpio2(gpio2), outPort(0)
{
    gpio_init(gpio1);
    gpio_init(gpio2);
    gpio_set_dir(gpio1, GPIO_OUT);
    gpio_set_dir(gpio2, GPIO_OUT);
    gpio_put(gpio1, 0);
    gpio_put(gpio2, 0);
}

void ASWITCH::selectOutput(uint8_t port)
{
    this->outPort = port;
    if (port > 3)
        throw NOSUCHPORT("port must be 0/1");

    switch (port)
    {
    case 0:
        gpio_put(gpio1, 0);
        gpio_put(gpio2, 0);
        break;
    case 1:
        gpio_put(gpio1, 1);
        gpio_put(gpio2, 0);
        break;
    case 2:
        gpio_put(gpio1, 0);
        gpio_put(gpio2, 1);
        break;
    case 3:
        gpio_put(gpio1, 1);
        gpio_put(gpio2, 1);
    default:
        gpio_put(gpio2, 0);
        gpio_put(gpio1, 0);
    }
}

uint8_t ASWITCH::getOutput() const
{
    return outPort;
}

uint ASWITCH::getResistor(uint8_t nr) const
{
    if (nr > 2)
        throw NOSUCHPORT("only 3 resistor 0/1/2 is used");
    switch (nr)
    {
    case 0:
        return outRes1;
    case 1:
        return outRes2;
    case 2:
        return outRes3;
    default:
        throw NOTSUPPOSEDTOREACHTHIS("getResistor, not supposed to reach this");
    }
}

double ASWITCH::getTotSwitchResistance() const
{
    return getResistor(this->outPort);
}

double ASWITCH::getTotSwitchResistanceFromMode(uint8_t mode) const
{
    return getResistor(mode);
}