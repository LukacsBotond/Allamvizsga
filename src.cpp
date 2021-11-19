#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "pico/sem.h"

#include "display/include/ili9341.h"
#include "display/include/displayDriver.h"
#include "display/include/characterDisplay.h"
#include "ADC/include/ADC.h"
#include "controll/include/Aswitch.h"

#include "common/include/common.h"

static struct semaphore startSemaphore;
static struct semaphore doneSemaphore;

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

int channel = 0;

//when main core starts the semaphore it prints
void core1_entry()
{
    IADC *adc = new ADC();
    
    while (1)
    {
        adc->setupFIFO();
        sem_acquire_blocking(&startSemaphore);
        adc->adcSelect(channel);
        adc->start_freeRunning();
        adc->waitDMAFull();
        adc->stop_freeRunning();
        adc->printSamples();
        sem_release(&doneSemaphore);
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
    //COMMON *common = new COMMON();
    IASWITCH *aswitch1 = new ASWITHCH(330, 4600, 0, 16, 17);
    IASWITCH *aswitch2 = new ASWITHCH(330, 4600, 0, 18, 19);
    IASWITCH *aswitch3 = new ASWITHCH(330, 4600, 0, 20, 21);
    sem_init(&startSemaphore, 0, 1);
    sem_init(&doneSemaphore, 0, 1);

    multicore_launch_core1(core1_entry);
    channel = 0;
    aswitch1->selectOutput(0);
    aswitch2->selectOutput(0);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    aswitch1->selectOutput(1);
    aswitch2->selectOutput(0);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    aswitch1->selectOutput(0);
    aswitch2->selectOutput(1);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    channel = 1;
    aswitch1->selectOutput(0);
    aswitch2->selectOutput(0);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    aswitch1->selectOutput(1);
    aswitch2->selectOutput(0);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    aswitch1->selectOutput(0);
    aswitch2->selectOutput(1);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    channel = 2;
    aswitch1->selectOutput(0);
    aswitch2->selectOutput(0);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    aswitch1->selectOutput(1);
    aswitch2->selectOutput(0);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);
    aswitch1->selectOutput(0);
    aswitch2->selectOutput(1);
    aswitch3->selectOutput(0);
    sem_release(&startSemaphore);
    sem_acquire_blocking(&doneSemaphore);
    sleep_ms(5000);

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
        sleep_ms(5000);
        std::cout << "Sleeping\n";
    }

    return 0;
}
