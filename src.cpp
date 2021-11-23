#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "hardware/pll.h"

#include "Global.h"
#include "display/include/ili9341.h"
#include "display/include/displayDriver.h"
#include "display/include/characterDisplay.h"
#include "ADC/include/ADC.h"
#include "controll/include/BaseSwitch.h"
#include "controll/include/BaseSwithcController.h"
#include "Calculate/include/BaseCalculate.h"
#include "Calculate/include/BaseValues.h"
#include "Calculate/include/BaseCleanInput.h"

#include "common/include/common.h"

struct semaphore *startSemaphore1;
struct semaphore *doneSemaphore1;

/*
uint8_t counter = 0;
void gpio_callback(uint gpio, uint32_t events)
{
    std::cout << "miso: " << gpio_get(4) << " cs: " << gpio_get(5) << " mosi: " << gpio_get(7) << " dc " << gpio_get(9) << std::endl;
    counter++;
    if (counter == 8)
    {
        std::cout << std::endl;
        counter = 0;
    }
}
*/

IADC *adc = new ADC();
//when main core starts the semaphore it prints
void core1_entry()
{
    adc->setupFIFO();
    std::cout << "ADC1\n";
    while (1)
    {
        std::cout << &startSemaphore1 << std::endl;
        std::cout << &doneSemaphore1 << std::endl;
        std::cout << sem_available(startSemaphore1);
        std::cout << sem_available(doneSemaphore1);
        std::cout << "ADC2\n";
        sem_acquire_blocking(startSemaphore1);
        std::cout << "ADC3\n";
        adc->start_freeRunning();
        std::cout << "ADC4\n";
        adc->waitDMAFull();
        std::cout << "ADC5\n";
        adc->stop_freeRunning();
        std::cout << "ADC6\n";
        //adc->printSamples();
        sem_release(doneSemaphore1);
        std::cout << "ADC7\n";
    }
    //gpio_set_irq_enabled_with_callback(6, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (1)
    {
        sleep_ms(1000);
    }
}
/*
void resus_callback(void) {
    // Reconfigure PLL sys back to the default state of 1500 / 6 / 2 = 125MHz
    pll_init(pll_sys, 1, 400 * MHZ, 2, 1);

    // CLK SYS = PLL SYS (125MHz) / 1 = 125MHz
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                    200 * MHZ,
                    200 * MHZ);

    // Reconfigure uart as clocks have changed
    stdio_init_all();
}
*/

int main()
{

    stdio_init_all();
    std::cout << "Test\n";
    sleep_ms(3000);
    std::cout << "Test\n";
    sem_init(startSemaphore1, 0, 1);
    sem_init(doneSemaphore1, 0, 1);
    std::cout << "real\n";
    std::cout << &startSemaphore1 << std::endl;
    std::cout << &doneSemaphore1 << std::endl;
    //COMMON *common = new COMMON();
    IVALUES *val = new BASEVALUES();
    ICLEANINPUT *cleanup = new BASECLEANINPUT();
    IASWITCH *aswitch1 = new BASESWITCH(330, 4600, 0, 16, 17);
    IASWITCH *aswitch2 = new BASESWITCH(330, 4600, 0, 18, 19);
    IASWITCH *aswitch3 = new BASESWITCH(330, 4600, 0, 20, 21);

    ICALCULATE *calc = new BASECALCULATE(val, adc, cleanup, aswitch1, aswitch2, aswitch3);
    //int spinL = spin_lock_claim_unused(true);
    //queue_init_with_spinlock(&ADCSelect_queue, sizeof(int), 2, spinL);
    //std::cout << "quee" << queue_get_level(&ADCSelect_queue);
    ISWITCHCONTROLLER *controller = new BASESWITCHCONTROLLER(aswitch1, aswitch2, aswitch3);
    controller->SameOut3ChannelRepeat(4, 0, 5);
    sleep_ms(3000);
    multicore_launch_core1(core1_entry);

    /*
    sleep_ms(3000);
    controller->SameOut3ChannelRepeat(2, 0, 5);

    sleep_ms(3000);
    controller->SameOut3ChannelRepeat(5, 0, 4);

    sleep_ms(3000);
    controller->SameOut3ChannelRepeat(5, 0, 2);

    sleep_ms(3000);
    controller->SameOut3ChannelRepeat(5, 0, 5);
*/
    /*
    DISPLAYDRIVER *driver = new DISPLAYDRIVER();
    
    driver->fillColor();
    sleep_ms(1000);
    std::cout<<"fekete\n";
    driver->fillColor(common->swap_bytes(0x0000));
    sleep_ms(1000);
    std::cout<<"feher\n";
    driver->fillColor(common->swap_bytes(0xFFFF));
    sleep_ms(1000);
    std::cout<<"piros\n";
    driver->fillColor(common->swap_bytes(0xF800));
    sleep_ms(1000);
    std::cout<<"Sarga\n";
    driver->fillColor(common->swap_bytes(0xFE60));
    sleep_ms(1000);
    std::cout<<"kek\n";
    driver->fillColor(common->swap_bytes(0x081F));
    */
    /*
    sleep_ms(1000);
    CHARACTERDISPLAY *charDriver = new CHARACTERDISPLAY( 0x0000 , 0xFFFF );
    charDriver->fillColor(common->swap_bytes(0x081F));
    charDriver->printLine("!!!!");
    charDriver->printLine("!!!!");
    charDriver->printLine("!!!!");
    charDriver->printLine("=");
    charDriver->printLine("almafa");
*/
    while (1)
    {
        std::cout << "Sleeping\n";
        sleep_ms(5000);
    }

    return 0;
}
