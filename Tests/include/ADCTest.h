#include "../../ADC/include/ADC.h"
#include "../../Exceptions/include/ASKmeasurement.h"

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
    // gpio_set_irq_enabled_with_callback(6, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (1)
    {
        sleep_ms(1000);
    }
}

class ADCTest
{
private:
    void adcRun();
    void getHZtest();

public:
    ADCTest();
    ~ADCTest();
};

ADCTest::ADCTest()
{
    // start the ADC on the 2nd core
    // multicore_launch_core1(core1_entry);
    // startSemaphoreRelease();
    // doneSemaphoreAquire();
    getHZtest();
}

ADCTest::~ADCTest()
{
}

void ADCTest::getHZtest()
{
    std::cout << "clk div: 10\n";
    adc->set_clkDiv(10);
    std::cout << adc->get_clkHz() << std::endl;

    std::cout << "clk div: 96\n";
    adc->set_clkDiv(96);
    std::cout << adc->get_clkHz() << std::endl;

    std::cout << "clk div: 960\n";
    adc->set_clkDiv(960);
    std::cout << adc->get_clkHz() << std::endl;

    std::cout << "clk div: 9600\n";
    adc->set_clkDiv(9600);
    std::cout << adc->get_clkHz() << std::endl;

    std::cout << "clk div: 96000\n";
    adc->set_clkDiv(96000);
    std::cout << adc->get_clkHz() << std::endl;
}