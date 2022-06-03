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

static struct semaphore startSemaphore1;
static struct semaphore doneSemaphore1;

static struct semaphore prepstartSemaphore1;
static struct semaphore prepdoneSemaphore1;

void ICALCULATE::startSemaphoreRelease(bool prep)
{
    if (prep)
    {
        sem_release(&prepstartSemaphore1);
    }
    else
    {
        sem_release(&startSemaphore1);
    }
}

void ICALCULATE::doneSemaphoreAquire(bool prep)
{
    if (prep)
    {
        sem_acquire_blocking(&prepdoneSemaphore1);
    }
    else
    {
        sem_acquire_blocking(&doneSemaphore1);
    }
}

IADC *adc = new ADC();
COMMON *commonClass = new COMMON();
ICALCULATE *IADCORRECTER::icalculate = nullptr;

IVALUES *ICALCULATE::values;
ICLEANINPUT *ICALCULATE::cleanup;
ISWITCHCONTROLLER *ICALCULATE::controller;
IADCORRECTER *ICALCULATE::adccorrecter;
std::vector<std::string> STATE::usedModes = {};
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
    ICLEANINPUT *cleanup = new ACLEANINPUT();
    IADCORRECTER *adccorrecter = new ADCCORRECTER();

    ICALCULATE *calc = new ACALCULATE(val, cleanup, controller, adccorrecter);
    std::cout << "Test0\n";
    MACHINE *machine = new MACHINE();
    machine->setState(new CAPACITOR(calc));
    std::cout << "Test1\n";
    sleep_ms(100);
    // TESTPRINTER *testprinter = new TESTPRINTER();
    std::cout << "Test2\n";
    machine->check();
    gpio_put(GREEN_LED_PIN, LOW);
}

//! END test cases
#endif // TESTS