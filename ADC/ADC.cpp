#include "./include/ADC.h"

ADC::ADC(){

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);

    
}