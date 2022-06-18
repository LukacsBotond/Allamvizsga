#include "pico/multicore.h"
#include "./include/ADC.h"
#include "../Exceptions/include/NoSuchPort.h"
#include "../Exceptions/include/NotSupposedToReachThis.h"
#include "../Global.h"

ADC::ADC()
{
    capture_buf = new uint16_t[CAPTURE_DEPTH];

    if (capture_buf == nullptr)
    {
        std::cout << "failed to allocate memory to ADC::CAPTUREBUFF\n";
        throw NULLEXCEPT("failed to allocate memory to ADC::CAPTUREBUFF\n");
    }
    adc_gpio_init(ACD_CHANNEL_0);
    adc_gpio_init(ACD_CHANNEL_1);
    adc_gpio_init(ACD_CHANNEL_2);
    adc_init();
    adc_select_input(0);

    adc_set_clkdiv(0);
    adc_set_temp_sensor_enabled(false);

    dma_chan = dma_claim_unused_channel(true);
    cfg = dma_channel_get_default_config(dma_chan);
}

ADC::~ADC()
{
    delete capture_buf;
}

void ADC::setupFIFO()
{
    // empty fifo array before next read
    while (!adc_fifo_is_empty())
    {
        adc_fifo_get();
    }

    adc_fifo_setup(
        true, // Write each completed conversion to the sample FIFO
        true, // Enable DMA data request (DREQ)
        1,    // DREQ (and IRQ) asserted when at least 1 sample present
        true, // Set sample error bit on error
        false // Keep full 12 bits of each sample
    );
    // set channel to what core 0 requires
    while (multicore_fifo_rvalid())
    {
        int chan = multicore_fifo_pop_blocking();
        setADCSelect(chan);
    }

    //Set the size of each DMA bus transfer
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);
    channel_config_set_dreq(&cfg, DREQ_ADC);
    dma_channel_configure(dma_chan, &cfg,
                          capture_buf,   // dst
                          &adc_hw->fifo, // src
                          CAPTURE_DEPTH, // transfer count
                          true           // start immediately
    );
}

void ADC::setADCSelect(const uint8_t chanel)
{
    if (chanel > 3)
    {
        std::cout << "Wrong channel selected" << (int)chanel <<" 0 will be selected\n";
        adc_select_input(ACD_CHANNEL_0);
        throw NOSUCHPORT("port must be 0/1/2");
    }
    adc_select_input(chanel);
}

uint ADC::getADCSelect()
{
    return adc_get_selected_input();
}

void ADC::start_freeRunning()
{
    gpio_put(POWERS_SAVE_PIN, HIGH);
    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_chan);
    adc_run(false);
    gpio_put(POWERS_SAVE_PIN, LOW);
}

void ADC::set_clkDiv(const uint div)
{
    if (div > 96)
        adc_div = div;
    else
        adc_div = 96;
    adc_set_clkdiv(div);
}

double ADC::get_clkHz()
{
    return (double)(clock_get_hz(clk_adc) / adc_div);
}

uint16_t ADC::getCaptureDepth()
{
    return CAPTURE_DEPTH;
}

uint16_t *ADC::getCaptureBuff()
{
    if (capture_buf == nullptr)
        throw NULLEXCEPT("ADC getCapturebuff is NULL");
    return capture_buf;
}

void ADC::printSamples()
{
    std::cout << "\nPrint Samples\n";
    std::cout << "Sampling rate: " << this->get_clkHz() << std::endl;
    std::cout << "current channel: " << this->getADCSelect() << std::endl
              << std::endl;
    for (int i = 0; i < CAPTURE_DEPTH; ++i)
    {
        std::cout << (uint16_t)capture_buf[i] << " ";
        if (i % 20 == 19)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}

void ADC::setCaptureBuff(uint16_t *buff, const uint16_t buffSize)
{
    if (buff == nullptr || buffSize != CAPTURE_DEPTH)
        throw NULLEXCEPT("ADC setCaptureBuff is null or buffSize it not equal to" + std::to_string(CAPTURE_DEPTH));
    memcpy(this->capture_buf, buff, (CAPTURE_DEPTH + 1) * sizeof(uint16_t));
}

