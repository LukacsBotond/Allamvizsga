#include "./include/BaseSwitch.h"
#include "../Exceptions/include/NoSuchPort.h"
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
    gpio_init(gpio1);
    gpio_init(gpio2);
    gpio_set_dir(gpio1, GPIO_IN);
    gpio_set_dir(gpio2, GPIO_IN);
    gpio_disable_pulls(gpio1);
    gpio_disable_pulls(gpio2);
}

void BASESWITCH::selectOutput(uint port)
{
    if (port > 8)
        throw NOSUCHPORT("port must between 0-8");

    switch (port)
    {
    case 0:
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_IN);
        break;
    case 1:
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio2, 0);
        break;

    case 2:
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio2, 1);
        break;
    case 3:
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_IN);
        gpio_put(gpio1, 0);
        break;
    case 4:
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_IN);
        gpio_put(gpio1, 1);
        break;
    case 5:
        gpio_set_dir(gpio1, GPIO_OUT);
        gpio_set_dir(gpio2, GPIO_OUT);
        gpio_put(gpio1, 0);
        gpio_put(gpio2, 0);
        break;
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
    default:
        gpio_set_dir(gpio1, GPIO_IN);
        gpio_set_dir(gpio2, GPIO_IN);
    }
}