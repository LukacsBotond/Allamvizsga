#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"
#include "hardware/pll.h"
#include "hardware/vreg.h"
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
#include "display/include/characterDisplay.h"
#include "display/include/graphDisplay.h"

#include "ADC/include/ADC.h"
//#include "ADC/include/ADCCorrecter.h"

//#include "controll/include/BaseSwitch.h"
#include "controll/include/Aswitch.h"
#include "controll/include/ASwithcController.h"
#include "controll/include/DAC.h"

//#include "Calculate/include/BaseCalculate.h"
//#include "Calculate/include/BaseValues.h"
//#include "Calculate/include/BaseCleanInput.h"

#include "common/include/common.h"

//#include "stateMachine/include/Machine.h"
//#include "stateMachine/include/Resistor.h"

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

IADC *adc = new ADC();
COMMON *commonClass = new COMMON();
ICALCULATE *IADCORRECTER::icalculate = nullptr;
//ICALCULATE *STATE::icalculate = nullptr;
IVALUES *ICALCULATE::values;
ICLEANINPUT *ICALCULATE::cleanup;
//ISWITCHCONTROLLER *ICALCULATE::controller;
//IADCORRECTER *ICALCULATE::adccorrecter;
//std::vector<std::string> STATE::usedModes = {};

#endif // TESTS
int main()
{
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    stdio_init_all();

    set_sys_clock_khz(280000, true);

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
    ISWITCHCONTROLLER *controller = new ASWITCHCONTROLLER(aswitch1, aswitch2, aswitch3, dac);
    //
    //IVALUES *val = new BASEVALUES();
    //ICLEANINPUT *cleanup = new BASECLEANINPUT();
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

    GRAPHDISPLAY driver(spidispl, commonClass->swap_bytes(0x0000), commonClass->swap_bytes(0x081F));
    /*
    std::cout << "TEST6" << std::endl;
    driver.fillColor();
    std::cout << "fekete\n";
    driver.fillColor(commonClass->swap_bytes(0x0000));
    std::cout << "feher\n";
    driver.fillColor(commonClass->swap_bytes(0xFFFF));

    std::cout << "piros\n";
    driver.fillColor(commonClass->swap_bytes(0xF800));
    std::cout << "Sarga\n";
    driver.fillColor(commonClass->swap_bytes(0xFE60));
    std::cout << "kek\n";
    driver.fillColor(commonClass->swap_bytes(0x081F));

    driver.fillColor(commonClass->swap_bytes(0x081F));
    driver.printLine("|");
    driver.printLine("!!!!");
    driver.printLine("!!!!");
    driver.printLine("=");
    driver.printLine("almafa");
    */
    std::vector<double> y{0.010101, 0.020202, 0.030303, 0.040404, 0.0505051, 0.0606061, 0.0707071, 0.0808081, 0.0909091, 0.10101, 0.111111, 0.121212, 0.131313, 0.141414, 0.151515, 0.161616, 0.171717, 0.181818, 0.191919, 0.20202, 0.212121, 0.222222, 0.232323, 0.242424, 0.252525, 0.262626, 0.272727, 0.282828, 0.292929, 0.30303, 0.313131, 0.323232, 0.333333, 0.343434, 0.353535, 0.363636, 0.373737, 0.383838, 0.393939, 0.40404, 0.414141, 0.424242, 0.434343, 0.444444, 0.454545, 0.464646, 0.474747, 0.484848, 0.494949, 0.505051, 0.515152, 0.525253, 0.535354, 0.545455, 0.555556, 0.565657, 0.575758, 0.585859, 0.59596, 0.606061, 0.616162, 0.626263, 0.636364, 0.646465, 0.656566, 0.666667, 0.676768, 0.686869, 0.69697, 0.707071, 0.717172, 0.727273, 0.737374, 0.747475, 0.757576, 0.767677, 0.777778, 0.787879, 0.79798, 0.808081, 0.818182, 0.828283, 0.838384, 0.848485, 0.858586, 0.868687, 0.878788, 0.888889, 0.89899, 0.909091, 0.919192, 0.929293, 0.939394, 0.949495, 0.959596, 0.969697, 0.979798, 0.989899, 1, 1.0101, 1.0202, 1.0303, 1.0404, 1.05051, 1.06061, 1.07071, 1.08081, 1.09091, 1.10101, 1.11111, 1.12121, 1.13131, 1.14141, 1.15152, 1.16162, 1.17172, 1.18182, 1.19192, 1.20202, 1.21212, 1.22222, 1.23232, 1.24242, 1.25253, 1.26263, 1.27273, 1.28283, 1.29293, 1.30303, 1.31313, 1.32323, 1.33333, 1.34343, 1.35354, 1.36364, 1.37374, 1.38384, 1.39394, 1.40404, 1.41414, 1.42424, 1.43434, 1.44444, 1.45455, 1.46465, 1.47475, 1.48485, 1.49495, 1.50505, 1.51515, 1.52525, 1.53535, 1.54545, 1.55556, 1.56566, 1.57576, 1.58586, 1.59596, 1.60606, 1.61616, 1.62626, 1.63636, 1.64646, 1.65657, 1.66667, 1.67677, 1.68687, 1.69697, 1.70707, 1.71717, 1.72727, 1.73737, 1.74747, 1.75758, 1.76768, 1.77778, 1.78788, 1.79798, 1.80808, 1.81818, 1.82828, 1.83838, 1.84848, 1.85859, 1.86869, 1.87879, 1.88889, 1.89899, 1.90909, 1.91919, 1.92929, 1.93939, 1.94949, 1.9596, 1.9697, 1.9798, 1.9899, 2, 2.0101, 2.0202, 2.0303, 2.0404, 2.05051, 2.06061, 2.07071, 2.08081, 2.09091, 2.10101, 2.11111, 2.12121, 2.13131, 2.14141, 2.15152, 2.16162, 2.17172, 2.18182, 2.19192, 2.20202, 2.21212, 2.22222, 2.23232, 2.24242, 2.25253, 2.26263, 2.27273, 2.28283, 2.29293, 2.30303, 2.31313, 2.32323, 2.33333, 2.34343, 2.35354, 2.36364, 2.37374, 2.38384, 2.39394, 2.40404, 2.41414, 2.42424, 2.43434, 2.44444, 2.45455, 2.46465, 2.47475, 2.48485, 2.49495, 2.50505, 2.51515, 2.52525, 2.53535, 2.54545, 2.55556, 2.56566, 2.57576, 2.58586, 2.59596, 2.60606, 2.61616, 2.62626, 2.63636, 2.64646, 2.65657, 2.66667, 2.67677, 2.68687, 2.69697, 2.70707, 2.71717, 2.72727, 2.73737, 2.74747, 2.75758, 2.76768, 2.77778, 2.78788, 2.79798, 2.80808, 2.81818, 2.82828, 2.83838, 2.84848, 2.85859, 2.86869, 2.87879, 2.88889, 2.89899, 2.90909, 2.91919, 2.92929, 2.93939, 2.94949, 2.9596, 2.9697, 2.9798, 2.9899, 3, 3.0101, 3.0202, 3.0303, 3.0404, 3.05051, 3.06061, 3.07071, 3.08081, 3.09091, 3.10101, 3.11111, 3.12121, 3.13131, 3.14141, 3.15152, 3.16162, 3.17172, 3.18182, 3.19192, 3.20202, 3.21212, 3.22222, 3.23232, 3.24242, 3.25253, 3.26263, 3.27273, 3.28283, 3.29293, 3.30303, 3.31313, 3.32323, 3.33333, 3.34343, 3.35354, 3.36364, 3.37374, 3.38384, 3.39394, 3.40404, 3.41414, 3.42424, 3.43434, 3.44444, 3.45455, 3.46465, 3.47475, 3.48485, 3.49495, 3.50505, 3.51515, 3.52525, 3.53535, 3.54545, 3.55556, 3.56566, 3.57576, 3.58586, 3.59596, 3.60606, 3.61616, 3.62626, 3.63636, 3.64646, 3.65657, 3.66667, 3.67677, 3.68687, 3.69697, 3.70707, 3.71717, 3.72727, 3.73737, 3.74747, 3.75758, 3.76768, 3.77778, 3.78788, 3.79798, 3.80808, 3.81818, 3.82828, 3.83838, 3.84848, 3.85859, 3.86869, 3.87879, 3.88889, 3.89899, 3.90909, 3.91919, 3.92929, 3.93939, 3.94949, 3.9596, 3.9697, 3.9798, 3.9899, 4, 4.0101, 4.0202, 4.0303, 4.0404, 4.05051, 4.06061, 4.07071, 4.08081, 4.09091, 4.10101, 4.11111, 4.12121, 4.13131, 4.14141, 4.15152, 4.16162, 4.17172, 4.18182, 4.19192, 4.20202, 4.21212, 4.22222, 4.23232, 4.24242, 4.25253, 4.26263, 4.27273, 4.28283, 4.29293, 4.30303, 4.31313, 4.32323, 4.33333, 4.34343, 4.35354, 4.36364, 4.37374, 4.38384, 4.39394, 4.40404, 4.41414, 4.42424, 4.43434, 4.44444, 4.45455, 4.46465, 4.47475, 4.48485, 4.49495, 4.50505, 4.51515, 4.52525, 4.53535, 4.54545, 4.55556, 4.56566, 4.57576, 4.58586, 4.59596, 4.60606, 4.61616, 4.62626, 4.63636, 4.64646, 4.65657, 4.66667, 4.67677, 4.68687, 4.69697, 4.70707, 4.71717, 4.72727, 4.73737, 4.74747, 4.75758, 4.76768, 4.77778, 4.78788, 4.79798, 4.80808, 4.81818, 4.82828, 4.83838, 4.84848, 4.85859, 4.86869, 4.87879, 4.88889, 4.89899, 4.90909, 4.91919, 4.92929, 4.93939, 4.94949, 4.9596, 4.9697, 4.9798, 4.9899, 5, 5.0101, 5.0202, 5.0303, 5.0404};
    // std::vector<double> y{0.010101, 0.020202, 0.030303, 0.040404, 0.0505051, 0.0606061, 0.0707071, 0.0808081, 0.0909091, 0.10101, 0.111111, 0.121212, 0.131313, 0.141414, 0.151515, 0.161616, 0.171717, 0.181818, 0.191919, 0.20202, 0.212121, 0.222222, 0.232323, 0.242424, 0.252525, 0.262626, 0.272727, 0.282828, 0.292929, 0.30303, 0.313131, 0.323232, 0.333333, 0.343434, 0.353535, 0.363636, 0.373737, 0.383838, 0.393939, 0.40404, 0.414141, 0.424242, 0.434343, 0.444444, 0.454545, 0.464646, 0.474747, 0.484848, 0.494949, 0.505051, 0.515152, 0.525253, 0.535354, 0.545455, 0.555556, 0.565657, 0.575758, 0.585859, 0.59596, 0.606061, 0.616162, 0.626263, 0.636364, 0.646465, 0.656566, 0.666667, 0.676768, 0.686869, 0.69697, 0.707071, 0.717172, 0.727273, 0.737374, 0.747475, 0.757576, 0.767677, 0.777778, 0.787879, 0.79798, 0.808081, 0.818182, 0.828283, 0.838384, 0.848485, 0.858586, 0.868687, 0.878788, 0.888889, 0.89899, 0.909091, 0.919192, 0.929293, 0.939394, 0.949495, 0.959596, 0.969697, 0.979798, 0.989899, 1, 1.0101, 1.0202, 1.0303, 1.0404, 1.05051, 1.06061, 1.07071, 1.08081, 1.09091, 1.10101, 1.11111, 1.12121, 1.13131, 1.14141, 1.15152, 1.16162, 1.17172, 1.18182, 1.19192, 1.20202, 1.21212, 1.22222, 1.23232, 1.24242, 1.25253, 1.26263, 1.27273, 1.28283, 1.29293, 1.30303, 1.31313, 1.32323, 1.33333, 1.34343, 1.35354, 1.36364, 1.37374, 1.38384, 1.39394, 1.40404, 1.41414, 1.42424, 1.43434, 1.44444, 1.45455, 1.46465, 1.47475, 1.48485, 1.49495, 1.50505, 1.51515, 1.52525, 1.53535, 1.54545, 1.55556, 1.56566, 1.57576, 1.58586, 1.59596, 1.60606, 1.61616, 1.62626, 1.63636, 1.64646, 1.65657, 1.66667, 1.67677, 1.68687, 1.69697, 1.70707, 1.71717, 1.72727, 1.73737, 1.74747, 1.75758, 1.76768, 1.77778, 1.78788, 1.79798, 1.80808, 1.81818, 1.82828, 1.83838, 1.84848, 1.85859, 1.86869, 1.87879, 1.88889, 1.89899, 1.90909, 1.91919, 1.92929, 1.93939, 1.94949, 1.9596, 1.9697, 1.9798, 1.9899, 2, 2.0101, 2.0202, 2.0303, 2.0404, 2.05051, 2.06061, 2.07071, 2.08081, 2.09091, 2.10101, 2.11111, 2.12121, 2.13131, 2.14141, 2.15152, 2.16162, 2.17172, 2.18182, 2.19192, 2.20202, 2.21212, 2.22222, 2.23232, 2.24242, 2.25253};
    // std::vector<double> y{0.010101, 0.020202, 0.030303, 0.040404};
    driver.plotArray(y, "nA", 2.1);

#endif

    vreg_set_voltage(VREG_VOLTAGE_0_85);
    set_sys_clock_khz(16000, true);
    stdio_init_all();

    std::cout << "Sleeping\n";
    while (1)
    {
        sleep_ms(5000);
    }

    return 0;
}
