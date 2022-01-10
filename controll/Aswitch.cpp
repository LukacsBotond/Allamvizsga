#include "./include/Aswitch.h"

ASWITCH::ASWITHCH(uint outRes1, uint outRes2, uint outRes3, uint gpio1, uint gpio2)
    : outRes1(outRes1), outRes2(outRes2), outRes3(outRes3), gpio1(gpio1), gpio2(gpio2)
{
    gpio_set_dir(gpio1, GPIO_IN);
    gpio_set_dir(gpio2, GPIO_IN);
    gpio_init(gpio1);
    gpio_init(gpio2);
}
//! change later
void ASWITCH::selectOutput(uint port)
{
    //TODO implement this
    throw NOSUCHPORT("this thing not exist");
    
    if (port > 3)
        throw NOSUCHPORT("port must be 0/1");

    switch (port)
    {
    case 0:
        gpio_set_function(gpio1, GPIO_FUNC_SIO);
        gpio_set_function(gpio2, GPIO_FUNC_SIO);
        gpio_put(gpio1, 0);
        gpio_put(gpio2, 0);
        break;
    case 1:
        gpio_set_function(gpio2, GPIO_FUNC_NULL);
        gpio_set_function(gpio1, GPIO_FUNC_SIO);
        gpio_put(gpio1, 1);
        break;

    case 2:
        gpio_set_function(gpio1, GPIO_FUNC_NULL);
        gpio_set_function(gpio2, GPIO_FUNC_SIO);
        gpio_put(gpio2, 1);
        break;
    default:
        gpio_put(gpio2, 0);
        gpio_put(gpio1, 0);
    }
}