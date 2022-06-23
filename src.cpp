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
#include "test.h"

#ifndef TESTS

#include "display/include/characterDisplay.h"
#include "display/include/graphDisplay.h"

#include "ADC/include/ADC.h"
#include "ADC/include/ADCCorrecter.h"

#include "controll/include/ASwitchController.h"
#include "controll/include/DAC.h"

#include "Calculate/include/ACalculate.h"
#include "Calculate/include/BaseValues.h"
#include "Calculate/include/ACleanInput.h"

#include "common/include/common.h"

#include "stateMachine/include/Machine.h"
#include "stateMachine/include/Resistor.h"
#include "stateMachine/include/Diode.h"
#include "stateMachine/include/Capacitor.h"
#include "stateMachine/include/Transistor.h"

#endif

static struct semaphore startSemaphore1;
static struct semaphore doneSemaphore1;

static struct semaphore prepstartSemaphore1;
static struct semaphore prepdoneSemaphore1;

void ICALCULATE::startSemaphoreRelease(bool prep)
{
    if (prep)
        sem_release(&prepstartSemaphore1);
    else
        sem_release(&startSemaphore1);
}

void ICALCULATE::doneSemaphoreAquire(bool prep)
{
    if (prep)
        sem_acquire_blocking(&prepdoneSemaphore1);
    else
        sem_acquire_blocking(&doneSemaphore1);
}

void core1_entry()
{
    adc->set_clkDiv(0);
    while (1)
    {
        sem_acquire_blocking(&prepstartSemaphore1);
        adc->setupFIFO();
        sem_release(&prepdoneSemaphore1);
        sem_acquire_blocking(&startSemaphore1);
        adc->start_freeRunning();
        sem_release(&doneSemaphore1);
    }
}

void printResult(const std::map<std::string, double> &ret, const std::string &mainResult, GRAPHDISPLAY *driver)
{
    std::cout << "Print Results\n";

    std::cout << mainResult << std::endl;
    driver->printLine(mainResult);

    for (auto it : ret)
    {
        std::cout << it.first << " " << it.second << std::endl;
        driver->printLine(it.first + " " + std::to_string(it.second));
    }

    for (auto it : STATE::usedPins)
    {
        std::cout << it.first << " " << it.second << std::endl;
        driver->printLine(std::to_string(it.first) + " " + it.second);
    }
    driver->fillRestScreen(0x0000);
}

void testing(GRAPHDISPLAY *driver, ICALCULATE *calc, MACHINE *machine)
{
    try
    {
        machine->setState(new RESISTOR(calc));
        machine->calculate();

        machine->setState(new CAPACITOR(calc));
        machine->calculate();
        if (STATE::mainResult == "Capacitor")
        {
            return;
        }
    }
    catch (POSSIBLYDIODE &e) // diode path
    {
        std::cout << e.what() << std::endl;
        machine->setState(new DIODE(calc));
        machine->calculate();
        // check if 2 inverse diode
        if (STATE::usedPins.at(0).size() > 1 || STATE::usedPins.at(1).size() > 1)
        {
            return;
        }
        machine->setState(new TRANSISTOR(calc));
        machine->calculate();
    }
    catch (NOTARESISTOR &e) // nothing found
    {
        calc->values->printMeasurements();
        std::cout << e.what() << std::endl;
    }
    catch (NOTHINGCONNECTED &e)
    {
        calc->values->printMeasurements();
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

#ifndef TESTS
IADC *adc = new ADC();
COMMON *commonClass = new COMMON();
ICLEANINPUT *cleanInput = new ACLEANINPUT();
ICALCULATE *IADCORRECTER::icalculate = nullptr;

IVALUES *ICALCULATE::values;
// ICLEANINPUT *ICALCULATE::cleanup;
ISWITCHCONTROLLER *ICALCULATE::controller;
IADCORRECTER *ICALCULATE::adccorrecter;
std::vector<std::string> STATE::usedModes = {};
std::map<std::string, double> STATE::results = {};
std::map<int, std::string> STATE::usedPins = {};
std::string STATE::mainResult = "Unknown part";
ICALCULATE *STATE::icalculate = nullptr;

#endif

int main()
{
    // vreg_set_voltage(VREG_VOLTAGE_1_30);
    stdio_init_all();
    sleep_ms(3000);
    // set_sys_clock_khz(280000, true);

    std::cout << "Start\n";
    sem_init(&startSemaphore1, 0, 1);
    sem_init(&doneSemaphore1, 0, 1);
    sem_init(&prepstartSemaphore1, 0, 1);
    sem_init(&prepdoneSemaphore1, 0, 1);
    multicore_launch_core1(core1_entry);

    gpio_init(RED_LED_PIN);
    gpio_init(GREEN_LED_PIN);

    gpio_set_dir(RED_LED_PIN, true);
    gpio_set_dir(GREEN_LED_PIN, true);

    gpio_put(RED_LED_PIN, LOW);
    gpio_put(GREEN_LED_PIN, HIGH);
#ifdef TESTS

    testCasesCaller();

#endif // DEBUG
#ifndef TESTS
    // DAC
    SPIPORTS *dac_spi_ports = new SPIPORTS(DAC_SPI_CHANNEL, DAC_CS, DAC_SCK, DAC_MOSI);
    SPI *spidac = new SPI(DAC_FREQ, dac_spi_ports);
    IDAC *dac = new DAC(spidac);
    // dac->reset(HIGH);

    // Switch controller
    ISWITCHCONTROLLER *controller = new ASWITCHCONTROLLER(dac);
    IVALUES *val = new BASEVALUES();
    IADCORRECTER *adccorrecter = new ADCCORRECTER();

    ICALCULATE *calc = new ACALCULATE(val, controller, adccorrecter);
    MACHINE *machine = new MACHINE();

    SPIPORTS *displ_spi_ports = new SPIPORTS(DISP_SPI_CHANNEL, DISP_CS, DISP_SCK, DISP_MOSI);
    SPI *spidispl = new SPI(DISP_FREQ, displ_spi_ports);
    GRAPHDISPLAY *driver = new GRAPHDISPLAY(spidispl, 0x0000, commonClass->swap_bytes(0x081F));

    STATE::usedPins[0] = "";
    STATE::usedPins[1] = "";
    STATE::usedPins[2] = "";

    testing(driver, calc, machine);

    printResult(machine->getResult(), machine->getMainResult(), driver);
    try
    {
        CharDiagr ret = dac->characteristicDiagramm(calc);
        // sleep_ms(1000);
        driver->plotArray(ret.data, "mA");
    }
    catch (NOTSUPPOSEDTOREACHTHIS)
    {
    }

    gpio_put(GREEN_LED_PIN, LOW);

#endif

    std::cout << "Sleeping\n";
    vreg_set_voltage(VREG_VOLTAGE_0_95);
    set_sys_clock_khz(16000, true);

    while (1)
    {
        sleep_ms(5000);
    }

    return 0;
}
