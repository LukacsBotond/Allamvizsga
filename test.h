#pragma once

#include "Global.h"

#ifdef TESTS

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

#include "Tests/include/TestPrinter.h"

#include "display/include/ili9341.h"
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

static struct semaphore startSemaphore1;
static struct semaphore doneSemaphore1;

static struct semaphore prepstartSemaphore1;
static struct semaphore prepdoneSemaphore1;



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

//! Test classes
void testCasesCaller()
{
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

    STATE::usedPins[0] = "";
    STATE::usedPins[1] = "";
    STATE::usedPins[2] = "";

    try
    {
        machine->setState(new RESISTOR(calc));
        machine->calculate();
        machine->setState(new CAPACITOR(calc));
        machine->calculate();
    }
    catch (POSSIBLYDIODE &e) // diode path
    {
        // std::cout << e.what() << std::endl;
        machine->setState(new DIODE(calc));
        machine->calculate();
        // check if 2 inverse diode
        if (STATE::usedPins.at(0).size() > 1 || STATE::usedPins.at(1).size() > 1)
        {
            printResult(machine->getResult(), machine->getMainResult());
        }
        machine->setState(new TRANSISTOR(calc));
        machine->calculate();
    }
    catch (NOTARESISTOR &e) // nothing found
    {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    printResult(machine->getResult(), machine->getMainResult());
    /*
    std::cout << "Print Resulst\n";
    std::map<std::string, double> ret = machine->getResult();
    gpio_put(GREEN_LED_PIN, LOW);
    for (auto it : ret)
    {
        std::cout << it.first << " " << it.second << std::endl;
    }

    for (auto it : STATE::usedPins)
    {
        std::cout << it.first << " " << it.second << std::endl;
    }
    */
}

//! END test cases
#endif // TESTS