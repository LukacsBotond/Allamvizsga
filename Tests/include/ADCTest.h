#include "../../ADC/include/ADC.h"
#include "../../Exceptions/include/ASKmeasurement.h"
#include <iostream>

static struct semaphore startSemaphore1;
static struct semaphore doneSemaphore1;

void startSemaphoreRelease()
{
    sem_release(&startSemaphore1);
}

void doneSemaphoreAquire()
{
    sem_acquire_blocking(&doneSemaphore1);
}

void core1_entry()
{
    adc->setupFIFO();
    std::cout << "ADC start! \n";
    adc->set_clkDiv(0);
    while (1)
    {
        adc->setupFIFO();
        sem_acquire_blocking(&startSemaphore1);
        try
        {
            adc->start_freeRunning();
        }
        catch (ASKMEASUREMENT &e)
        {
            std::cout << e.what() << std::endl;
        }
        // adc->printSamples();
        sem_release(&doneSemaphore1);
    }
}

class ADCTest
{
private:
    TESTPRINTER *testprinter;
    void getHZtest();
    void setGetBuffTest();

public:
    ADCTest(TESTPRINTER *testprinter);
    ~ADCTest();
};

ADCTest::ADCTest(TESTPRINTER *testprinter)
{
    this->testprinter = testprinter;
    // start the ADC on the 2nd core
    multicore_launch_core1(core1_entry);
    // startSemaphoreRelease();
    // doneSemaphoreAquire();
    getHZtest();
    setGetBuffTest();
}

ADCTest::~ADCTest()
{
}

void ADCTest::getHZtest()
{
    //* clk div = 0
    std::cout << "clk div: 0\n";
    adc->set_clkDiv(0);
    if (commonClass->roughlyEqual(adc->get_clkHz(), 500000))
        testprinter->succesPrinter("ADC getHztest", "clk div 0 pass");
    else
        testprinter->errorPrinter("ADC getHztest", adc->get_clkHz(), 500000);

    //* clk div = 96
    std::cout << "clk div: 96\n";
    adc->set_clkDiv(96);
    if (commonClass->roughlyEqual(adc->get_clkHz(), 500000))
        testprinter->succesPrinter("ADC getHztest", "clk div 96 pass");
    else
        testprinter->errorPrinter("ADC getHztest", adc->get_clkHz(), 500000);

    //* clk div = 100
    std::cout << "clk div: 100\n";
    adc->set_clkDiv(100);
    if (commonClass->roughlyEqual(adc->get_clkHz(), 480000))
        testprinter->succesPrinter("ADC getHztest", "clk div 100 pass");
    else
        testprinter->errorPrinter("ADC getHztest", adc->get_clkHz(), 480000);

    //* clk div = 1000
    std::cout << "clk div: 1000\n";
    adc->set_clkDiv(1000);
    if (commonClass->roughlyEqual(adc->get_clkHz(), 48000))
        testprinter->succesPrinter("ADC getHztest", "clk div 1000 pass");
    else
        testprinter->errorPrinter("ADC getHztest", adc->get_clkHz(), 48000);

    //* clk div = 10000
    std::cout << "clk div: 10000\n";
    adc->set_clkDiv(10000);
    if (commonClass->roughlyEqual(adc->get_clkHz(), 4800))
        testprinter->succesPrinter("ADC getHztest", "clk div 10000 pass");
    else
        testprinter->errorPrinter("ADC getHztest", adc->get_clkHz(), 4800);

    //* clk div = 100000
    std::cout << "clk div: 100000\n";
    adc->set_clkDiv(100000);
    if (commonClass->roughlyEqual(adc->get_clkHz(), 480))
        testprinter->succesPrinter("ADC getHztest", "clk div 100000 pass");
    else
        testprinter->errorPrinter("ADC getHztest", adc->get_clkHz(), 480);
}

void ADCTest::setGetBuffTest()
{
    uint16_t *buff = new uint16_t[255];
    uint16_t *ret;
    uint16_t val = 20, zero = 0;
    int i;
    std::fill_n(buff, 255, val);
    adc->setCaptureBuff(buff, 255);
    std::fill_n(buff, 255, zero);
    ret = adc->getCaptureBuff();
    for (i = 0; i < 255; i++)
    {
        //std::cout << "i: " << i << std::endl;
        if (ret[i] != val)
        {
            testprinter->errorPrinter("ADCsetGetBuffTest fail at index" + std::to_string(i),ret[i], val);
            break;
        }
    }
    if (i >= 255)
        testprinter->succesPrinter("ADCsetGetBuffTest", "");
    val = 4096;
    std::fill_n(buff, 255, val);
    adc->setCaptureBuff(buff, 255);
    std::fill_n(buff, 255, zero);
    ret = adc->getCaptureBuff();
    for (i = 0; i < 255; i++)
    {
        if (ret[i] != val)
            testprinter->errorPrinter("ADCsetGetBuffTest fail at index" + std::to_string(i), ret[i], val);
    }
    if (i >= 255)
        testprinter->succesPrinter("ADCsetGetBuffTest", "");
    val = UINT16_MAX;
    std::fill_n(buff, 255, val);
    adc->setCaptureBuff(buff, 255);
    std::fill_n(buff, 255, zero);
    ret = adc->getCaptureBuff();
    for (i = 0; i < 255; i++)
    {
        if (ret[i] != val)
            testprinter->errorPrinter("ADCsetGetBuffTest fail at index" + std::to_string(i), ret[i], val);
    }
    if (i >= 255)
        testprinter->succesPrinter("ADCsetGetBuffTest", "");
    //* expected to not run succesfully

    // nullpointer test
    try
    {
        adc->setCaptureBuff(NULL, 255);
    }
    catch (NULLEXCEPT &e)
    {
        testprinter->succesPrinter("ADCsetGetBuffTest null pointer", e.what());
    }

    // smaller buffsize than allocated in ADC
    try
    {
        adc->setCaptureBuff(buff, 254);
    }
    catch (NULLEXCEPT &e)
    {
        testprinter->succesPrinter("ADCsetGetBuffTest smaller buffSize", e.what());
    }

    // bigger buffsize than allocated in ADC
    try
    {
        adc->setCaptureBuff(buff, 256);
    }
    catch (NULLEXCEPT &e)
    {
        testprinter->succesPrinter("ADCsetGetBuffTest smaller buffSize", e.what());
    }

    //
    try
    {
        adc->setCaptureBuff(buff, -1);
    }
    catch (NULLEXCEPT &e)
    {
        testprinter->succesPrinter("ADCsetGetBuffTest smaller buffSize", e.what());
    }
    try
    {
        adc->setCaptureBuff(buff, UINT16_MAX);
    }
    catch (NULLEXCEPT &e)
    {
        testprinter->succesPrinter("ADCsetGetBuffTest smaller buffSize", e.what());
    }
}
