#include "./include/BaseSwitch.h"
#include "../Exceptions/include/NoSuchPort.h"
#include "../Exceptions/include/NotSupposedToReachThis.h"
/*
    selection table
    0 -> ZZ
    1 -> Z0
    2 -> Z1
    3 -> 0Z
    4 -> 1Z
    5 -> 00
    6 -> 01
    7 -> 10
    8 -> 11
*/
BASESWITCH::BASESWITCH(uint outRes1, uint outRes2, uint outRes3, uint gpio1, uint gpio2)
    : outRes1(outRes1), outRes2(outRes2), outRes3(outRes3), gpio1(gpio1), gpio2(gpio2)
{
    outPort = 0;
    gpio_init(gpio1);
    gpio_init(gpio2);
    gpio_set_dir(gpio1, GPIO_IN);
    gpio_set_dir(gpio2, GPIO_IN);
    gpio_disable_pulls(gpio1);
    gpio_disable_pulls(gpio2);
}

void BASESWITCH::selectOutput(uint8_t port)
{
    this->outPort = port;
    if (port > 5)
        throw NOSUCHPORT("port must between 0-5");

    switch (port)
    {
    case 0:
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_IN);
        gpio_pull_down(gpio1);
        gpio_pull_down(gpio2);
        break;
    case 1:
        gpio_disable_pulls(gpio1);
        gpio_disable_pulls(gpio2);
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio2, 0);
        break;

    case 2:
        gpio_disable_pulls(gpio1);
        gpio_disable_pulls(gpio2);
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio2, 1);
        break;
    case 3:
        gpio_disable_pulls(gpio1);
        gpio_disable_pulls(gpio2);
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_IN);
        gpio_put(gpio1, 0);
        break;
    case 4:
        gpio_disable_pulls(gpio1);
        gpio_disable_pulls(gpio2);
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_IN);
        gpio_put(gpio1, 1);
        break;
    case 5:
        gpio_disable_pulls(gpio1);
        gpio_disable_pulls(gpio2);
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio1, 0);
        gpio_put(gpio2, 0);
        break;
    /*
    case 6:
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio1, 0);
        gpio_put(gpio2, 1);
        break;
    case 7:
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio1, 1);
        gpio_put(gpio2, 0);
        break;
    case 8:
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio1, 1);
        gpio_put(gpio2, 1);
        break;
    */
    default:
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_IN);
    }
}

uint8_t BASESWITCH::getOutput()
{
    return outPort;
}

uint BASESWITCH::getResistor(uint8_t nr)
{
    if (nr > 1)
        throw NOSUCHPORT("only 2 resistor 0/1 is used");
    switch (nr)
    {
    case 0:
        return outRes2;
    case 1:
        return outRes1;
    default:
        throw NOTSUPPOSEDTOREACHTHIS("getResistor, not supposed to reach this");
    }
}
