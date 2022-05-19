#include "./include/Aswitch.h"
#include "../Exceptions/include/NoSuchPort.h"
#include "../Exceptions/include/NotSupposedToReachThis.h"

/*
0 -> Z
1 -> L
2 -> M
3 -> H
3+ -> Z

*/

ASWITCH::ASWITCH(uint outResLow, uint outResMid, uint outResHigh, uint8_t gpioLs, uint8_t gpioMs)
    : outResLow(outResLow), outResMid(outResMid), outResHigh(outResHigh), gpioLs(gpioLs), gpioMs(gpioMs), outPort(0)
{
    gpio_init(gpioLs);
    gpio_init(gpioMs);
    gpio_set_dir(gpioLs, GPIO_OUT);
    gpio_set_dir(gpioMs, GPIO_OUT);
    gpio_put(gpioLs, 0);
    gpio_put(gpioMs, 0);
}

void ASWITCH::selectOutput(uint8_t port)
{
    std::cout << "USE gpio_put_masked() instead\n";
    this->outPort = port;
    if (port > 3)
        throw NOSUCHPORT("port must be 0/1");

    switch (port)
    {
    case 0: // ZZ
        gpio_put(gpioLs, 0);
        gpio_put(gpioMs, 0);
        break;
    case 1:
        gpio_put(gpioLs, 1);
        gpio_put(gpioMs, 0);
        break;
    case 2:
        gpio_put(gpioLs, 0);
        gpio_put(gpioMs, 1);
        break;
    case 3:
        gpio_put(gpioLs, 1);
        gpio_put(gpioMs, 1);
    default:
        gpio_put(gpioLs, 0);
        gpio_put(gpioMs, 0);
    }
}

uint8_t ASWITCH::getOutput() const
{
    return outPort;
}

uint ASWITCH::getResistor(uint8_t nr) const
{
    //! DEFINE TAGS ALREADY GIVE THIS INFORMATION SO IT IS USELESS
    std::cout << "DEFINE TAGS ALREADY GIVE THIS INFORMATION SO IT IS USELESS\n";
    throw NOTSUPPOSEDTOREACHTHIS("DEFINE TAGS ALREADY GIVE THIS INFORMATION SO IT IS USELESS\n");
    /*
    if (nr > 2)
        throw NOSUCHPORT("only 3 resistor 0/1/2 is used");
    switch (nr)
    {
    case 0:
        return outResLow;
    case 1:
        return outResMid;
    case 2:
        return outResHigh;
    default:
        throw NOTSUPPOSEDTOREACHTHIS("getResistor, not supposed to reach this");
    }
    */
}

double ASWITCH::getTotSwitchResistance() const
{
    return getResistor(this->outPort);
}

double ASWITCH::getTotSwitchResistance(uint8_t mode) const
{
    return getResistor(mode);
}