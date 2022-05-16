#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "hardware/pll.h"
#include <vector>

#include "Global.h"

#ifdef TESTS
//! Test classes

#include "ADC/include/ADC.h"
#include "controll/include/Aswitch.h"
#include "controll/include/BaseSwithcController.h"
#include "Tests/include/TestPrinter.h"
#include "Tests/include/BaseCleanInputTest.h"
#include "Tests/include/ADCTest.h"
#include "Calculate/include/BaseCalculate.h"
#include "Calculate/include/BaseValues.h"
#include "Tests/include/BaseCalculateTest.h"

void ICALCULATE::startSemaphoreRelease()
{
    sem_release(&startSemaphore1);
}

void ICALCULATE::doneSemaphoreAquire()
{
    sem_acquire_blocking(&doneSemaphore1);
}

IADC *adc = new ADC();
COMMON *commonClass = new COMMON();
ICALCULATE *IADCORRECTER::icalculate = nullptr;
// ICALCULATE *STATE::icalculate = nullptr;
IVALUES *ICALCULATE::values;
ICLEANINPUT *ICALCULATE::cleanup;
ISWITCHCONTROLLER *ICALCULATE::controller;
IADCORRECTER *ICALCULATE::adccorrecter;
void testCasesCaller()
{
    IASWITCH *aswitch1 = new ASWITCH(RESISTOR_LOW, RESISTOR_MID, RESISTOR_HIGH, SWITHCH1_1, SWITHCH1_2);
    IASWITCH *aswitch2 = new ASWITCH(RESISTOR_LOW, RESISTOR_MID, RESISTOR_HIGH, SWITHCH2_1, SWITHCH2_2);
    IASWITCH *aswitch3 = new ASWITCH(RESISTOR_LOW, RESISTOR_MID, RESISTOR_HIGH, SWITHCH3_1, SWITHCH3_2);
    std::cout << "Test1\n";
    sleep_ms(100);
    TESTPRINTER *testprinter = new TESTPRINTER();
    IVALUES *val = new BASEVALUES();
    ICLEANINPUT *cleanup = new BASECLEANINPUT();
    IADCORRECTER *adccorrecter = new ADCCORRECTER();
    std::cout << "Test2\n";
    ISWITCHCONTROLLER *controller = new BASESWITCHCONTROLLER(aswitch1, aswitch2, aswitch3);
    ICALCULATE *basecalculate = new BASECALCULATE(val, cleanup, controller, adccorrecter);
    BASECLEANINPUT *cleanuptest = new BASECLEANINPUT();
    std::cout << "Test3\n";
    /*
    std::cout << "BaseCleanInput Test\n";
    sleep_ms(1000);
    BaseCleanInputTest testCleanup(cleanuptest, testprinter);
    std::cout << "ADC Tests\n";
    sleep_ms(1000);
    ADCTest adctest(testprinter);
    std::cout << "BaseCalculate Tests\n";
    sleep_ms(1000);
    */
    BaseCalculateTest calculatetest(basecalculate, testprinter);
}

//! END test cases
#endif // TESTS
#ifndef TESTS
#include "display/include/ili9341.h"
#include "display/include/displayDriver.h"
#include "display/include/characterDisplay.h"
#include "ADC/include/ADC.h"
#include "ADC/include/ADCCorrecter.h"

//#include "controll/include/BaseSwitch.h"
#include "controll/include/Aswitch.h"
#include "controll/include/BaseSwithcController.h"
#include "controll/include/DAC.h"
#include "Calculate/include/BaseCalculate.h"
#include "Calculate/include/BaseValues.h"
#include "Calculate/include/BaseCleanInput.h"

#include "common/include/common.h"

#include "stateMachine/include/Machine.h"
#include "stateMachine/include/Resistor.h"

static struct semaphore startSemaphore1;
static struct semaphore doneSemaphore1;

uint8_t counter = 0;
void gpio_callback(uint gpio, uint32_t events)
{
    std::cout << "mosi: " << gpio_get(DAC_MOSI) << " cs: " << gpio_get(DAC_CS) << " RESET: " << gpio_get(DAC_RESET) << " enable: " << gpio_get(DAC_ENABLE) << " RSTSEL: " << gpio_get(DAC_RSTSEL) << std::endl;
    // std::cout << "mosi: " << gpio_get(DISP_MOSI) << " cs: " << gpio_get(DISP_CS) << std::endl;
    counter++;
    if (counter == 8)
    {
        std::cout << std::endl;
        counter = 0;
    }
}

void ICALCULATE::startSemaphoreRelease()
{
    sem_release(&startSemaphore1);
}

void ICALCULATE::doneSemaphoreAquire()
{
    sem_acquire_blocking(&doneSemaphore1);
}

// when main core starts the semaphore it prints
void core1_entry()
{
    gpio_set_irq_enabled_with_callback(DAC_SCK, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    adc->setupFIFO();
    std::cout << "ADC start! \n";
    adc->set_clkDiv(0);
    while (1)
    {
        adc->setupFIFO();
        sem_acquire_blocking(&startSemaphore1);
        adc->start_freeRunning();
        // adc->printSamples();
        sem_release(&doneSemaphore1);
    }

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

IADC *adc = new ADC();
COMMON *commonClass = new COMMON();
ICALCULATE *IADCORRECTER::icalculate = nullptr;
ICALCULATE *STATE::icalculate = nullptr;
IVALUES *ICALCULATE::values;
ICLEANINPUT *ICALCULATE::cleanup;
ISWITCHCONTROLLER *ICALCULATE::controller;
IADCORRECTER *ICALCULATE::adccorrecter;
std::vector<std::string> STATE::usedModes = {};

#endif // TESTS
int main()
{
    stdio_init_all();
    std::cout << "Test\n";
    sleep_ms(3000);
    std::cout << "Test\n";
#ifdef TESTS

    testCasesCaller();

#endif // DEBUG
#ifndef TESTS

    sem_init(&startSemaphore1, 0, 1);
    sem_init(&doneSemaphore1, 0, 1);
    gpio_init(RED_LED_PIN);
    gpio_init(GREEN_LED_PIN);

    gpio_set_dir(RED_LED_PIN, true);
    gpio_set_dir(GREEN_LED_PIN, true);

    gpio_put(RED_LED_PIN, LOW);
    gpio_put(GREEN_LED_PIN, HIGH);
    multicore_launch_core1(core1_entry);
    // DAC
    SPIPORTS *dac_spi_ports = new SPIPORTS(DAC_SPI_CHANNEL, DAC_CS, DAC_SCK, DAC_MOSI);
    SPI *spidac = new SPI(DAC_FREQ, dac_spi_ports);
    IDAC *dac = new DAC(spidac);
    dac->reset(HIGH);
    gpio_put(GREEN_LED_PIN, LOW);
    //! TESTING delete later
    // std::cout << "0x0000\n";
    // dac->setVoltageOnChannel(0x0000, DAC_COMM_WRITE_LOAD_ALLCHANNEL);
    //! end delete

    // Switch controller
    IASWITCH *aswitch1 = new ASWITCH(RESISTOR_LOW, RESISTOR_MID, RESISTOR_HIGH, SWITHCH1_1, SWITHCH1_2);
    IASWITCH *aswitch2 = new ASWITCH(RESISTOR_LOW, RESISTOR_MID, RESISTOR_HIGH, SWITHCH2_1, SWITHCH2_2);
    IASWITCH *aswitch3 = new ASWITCH(RESISTOR_LOW, RESISTOR_MID, RESISTOR_HIGH, SWITHCH3_1, SWITHCH3_2);
    ISWITCHCONTROLLER *controller = new BASESWITCHCONTROLLER(aswitch1, aswitch2, aswitch3, dac);
    //
    // IVALUES *val = new BASEVALUES();
    // ICLEANINPUT *cleanup = new BASECLEANINPUT();
    // IADCORRECTER *adccorrecter = new ADCCORRECTER();

    // ICALCULATE *calc = new BASECALCULATE(val, cleanup, controller, adccorrecter);
    /*
    MACHINE *machine = new MACHINE();
    machine->setState(new RESISTOR(calc));
    try
    {
        machine->calculate();
    }
    catch (POSSIBLYDIODE &e)
    {
        std::cout << e.what() << std::endl;
    }
    */

    // display
    SPIPORTS *displ_spi_ports = new SPIPORTS(DISP_SPI_CHANNEL, DISP_CS, DISP_SCK, DISP_MOSI);
    SPI *spidispl = new SPI(DISP_FREQ, displ_spi_ports);
    ILI9341 *driver = new CHARACTERDISPLAY(spidispl, commonClass->swap_bytes(0x0000), commonClass->swap_bytes(0x081F));
    std::cout << "TEST6" << std::endl;

    /*
        driver->fillColor();
        std::cout << "fekete\n";
        driver->fillColor(commonClass->swap_bytes(0x0000));
        std::cout << "feher\n";
        driver->fillColor(commonClass->swap_bytes(0xFFFF));

        std::cout << "piros\n";
        driver->fillColor(commonClass->swap_bytes(0xF800));
        std::cout << "Sarga\n";
        driver->fillColor(commonClass->swap_bytes(0xFE60));
        std::cout << "kek\n";
        driver->fillColor(commonClass->swap_bytes(0x081F));
    */
    driver->fillColor(commonClass->swap_bytes(0x081F));
    driver->printLine("|");
    driver->printLine("!!!!");
    driver->printLine("!!!!");
    driver->printLine("=");
    driver->printLine("almafa");

#endif

    std::cout << "Sleeping\n";
    while (1)
    {
        sleep_ms(5000);
    }

    return 0;
}
