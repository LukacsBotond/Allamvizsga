#include "pico/multicore.h"
#include "./include/ADC.h"
#include "../Exceptions/include/NoSuchPort.h"
#include "../Exceptions/include/NotSupposedToReachThis.h"
#include "../Global.h"

ADC::ADC()
{
    // capture_buf = new uint16_t *[2];
    capture_buf = new uint16_t[CAPTURE_DEPTH];

    if (capture_buf == nullptr)
    {
        std::cout << "failed to allocate memory to ADC::CAPTUREBUFF\n";
        throw NULLEXCEPT("failed to allocate memory to ADC::CAPTUREBUFF\n");
    }
    // capture_buf[1] = new uint16_t[CAPTURE_DEPTH];
    adc_gpio_init(ACD_CHANNEL_0);
    adc_gpio_init(ACD_CHANNEL_1);
    adc_gpio_init(ACD_CHANNEL_2);
    adc_init();
    adc_select_input(0);

    adc_set_clkdiv(0);
    adc_set_temp_sensor_enabled(false);

    dma_chan = dma_claim_unused_channel(true);
    cfg = dma_channel_get_default_config(dma_chan);

    // dma_chan1 = dma_claim_unused_channel(true);
    // cfg1 = dma_channel_get_default_config(dma_chan1);
}

ADC::~ADC()
{
    // delete capture_buf[0];
    //  delete capture_buf[1];
    delete capture_buf;
}

void ADC::setupFIFO()
{
    adc_fifo_setup(
        true, // Write each completed conversion to the sample FIFO
        true, // Enable DMA data request (DREQ)
        1,    // DREQ (and IRQ) asserted when at least 1 sample present
        true, // Set sample error bit on error
        false // Keep full 12 bits of each sample
    );
    // set channel to what core 0 requires
    if (multicore_fifo_rvalid())
    {
        int chan = multicore_fifo_pop_blocking();
        setADCSelect(chan);
    }

    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);
    /*
        channel_config_set_transfer_data_size(&cfg1, DMA_SIZE_16);
        channel_config_set_read_increment(&cfg1, false);
        channel_config_set_write_increment(&cfg1, true);
    */
    // Pace transfers based on availability of ADC samples
    channel_config_set_dreq(&cfg, DREQ_ADC);
    // channel_config_set_dreq(&cfg1, DREQ_ADC);

    dma_channel_configure(dma_chan, &cfg,
                          capture_buf,   // dst
                          &adc_hw->fifo, // src
                          CAPTURE_DEPTH, // transfer count
                          true           // start immediately
    );
    /*
        dma_channel_configure(dma_chan1, &cfg1,
                              capture_buf[1], // dst
                              &adc_hw->fifo,  // src
                              CAPTURE_DEPTH,  // transfer count
                              true            // start immediately
        );
        */

    // empty fifo array before next read
    while (!adc_fifo_is_empty())
    {
        adc_fifo_get();
    }
}

void ADC::waitDMAFull()
{
    // if (!usedIndex)
    dma_channel_wait_for_finish_blocking(dma_chan);
    // else
    //     dma_channel_wait_for_finish_blocking(dma_chan1);

    // buffer full, now it can be read
    // usedIndex = !usedIndex;
    // std::cout << "usedIndex:" << usedIndex << std::endl;
}

void ADC::setADCSelect(uint8_t chanel)
{
    if (chanel > 3 || chanel < 0)
    {
        std::cout << "Wrong channel selected, 0 will be selected\n";
        adc_select_input(ACD_CHANNEL_0);
        throw NOSUCHPORT("port must be 0/1/2");
    }
    // std::cout << "set ADC chan to: " << (int)chanel << " channel" << std::endl;
    adc_select_input(chanel);
}

uint ADC::getADCSelect()
{
    return adc_get_selected_input();
}

void ADC::start_freeRunning()
{
    // std::cout << "\ncurrent channel: " << adc_get_selected_input() << std::endl;
    gpio_put(POWERS_SAVE_PIN, HIGH);
    adc_run(true);
    // waitDMAFull();
    dma_channel_wait_for_finish_blocking(dma_chan);
    adc_run(false);
    gpio_put(POWERS_SAVE_PIN, LOW);
}

void ADC::set_clkDiv(uint div)
{
    adc_set_clkdiv(div);
}

// TODO check get_clkDiv
uint32_t ADC::get_clkHz()
{
    std::cout << clock_get_hz(clk_adc) << std::endl;
    return clock_get_hz(clk_adc);
}

uint16_t ADC::getCaptureDepth()
{
    return CAPTURE_DEPTH;
}

uint16_t *ADC::getCaptureBuff()
{
    // std::cout << "getCaptureBuff usedIndex:" << !usedIndex << std::endl;
    if (capture_buf == nullptr)
        throw NULLEXCEPT("ADC capturebuff is NULL");
    return capture_buf;
}

void ADC::printSamples()
{
    std::cout << "\nPrint Samples\n";
    for (int i = 0; i < CAPTURE_DEPTH; ++i)
    {
        std::cout << (uint16_t)capture_buf[i] << " ";
        if (i % 20 == 19)
            std::cout << std::endl;
    }
}

void ADC::setCaptureBuff(uint16_t *buff)
{
    if (buff == nullptr)
        throw NULLEXCEPT("setCaptureBuff is null");
    this->capture_buf = buff;
}
