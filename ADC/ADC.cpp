#include "pico/multicore.h"
#include "./include/ADC.h"
#include "../Exceptions/include/NoSuchPort.h"
#include "../Exceptions/include/NotSupposedToReachThis.h"
#include "../Global.h"

ADC::ADC()
{
    capture_buf = new uint16_t *[2];
    capture_buf[0] = new uint16_t[CAPTURE_DEPTH];
    capture_buf[1] = new uint16_t[CAPTURE_DEPTH];
    adc_gpio_init(ACD_CHANNEL_0);
    adc_gpio_init(ACD_CHANNEL_1);
    adc_gpio_init(ACD_CHANNEL_2);
    adc_init();
    adc_select_input(0);

    adc_set_clkdiv(0);
    adc_set_temp_sensor_enabled(false);

    dma_chan = dma_claim_unused_channel(true);
    cfg = dma_channel_get_default_config(dma_chan);

    dma_chan1 = dma_claim_unused_channel(true);
    cfg1 = dma_channel_get_default_config(dma_chan1);
}

ADC::~ADC()
{
    delete capture_buf[0];
    delete capture_buf[1];
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
    //set channel to what core 0 requires
    if (multicore_fifo_rvalid())
    {
        int chan = multicore_fifo_pop_blocking();
        //std::cout << "usedIndex start freeRunning:" << usedIndex << std::endl;
        setADCSelect(chan);
    }

    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);

    channel_config_set_transfer_data_size(&cfg1, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg1, false);
    channel_config_set_write_increment(&cfg1, true);

    // Pace transfers based on availability of ADC samples
    channel_config_set_dreq(&cfg, DREQ_ADC);
    channel_config_set_dreq(&cfg1, DREQ_ADC);

    dma_channel_configure(dma_chan, &cfg,
                          capture_buf[0], // dst
                          &adc_hw->fifo,  // src
                          CAPTURE_DEPTH,  // transfer count
                          true            // start immediately
    );

    dma_channel_configure(dma_chan1, &cfg1,
                          capture_buf[1], // dst
                          &adc_hw->fifo,  // src
                          CAPTURE_DEPTH,  // transfer count
                          true            // start immediately
    );
}

void ADC::waitDMAFull()
{
    if (!usedIndex)
        dma_channel_wait_for_finish_blocking(dma_chan);
    else
        dma_channel_wait_for_finish_blocking(dma_chan1);

    //buffer full, now it can be read
    usedIndex = !usedIndex;
    //std::cout << "usedIndex:" << usedIndex << std::endl;
}

void ADC::setADCSelect(uint8_t chanel)
{
    if (chanel > 3 || chanel < 0)
    {
        std::cout << "Wrong channel selected, 0 will be selected\n";
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
    adc_run(true);
    waitDMAFull();
    adc_run(false);
}

void ADC::set_clkDiv(uint div)
{
    adc_set_clkdiv(div);
}

uint16_t ADC::getCaptureDepth()
{
    return CAPTURE_DEPTH;
}

uint16_t *ADC::getCaptureBuff()
{
    //std::cout << "getCaptureBuff usedIndex:" << !usedIndex << std::endl;
    return capture_buf[!usedIndex];
}

void ADC::printSamples()
{
    std::cout << "Print Samples\n";
    for (int i = 0; i < CAPTURE_DEPTH; ++i)
    {
        std::cout << capture_buf[i] << " ";
        if (i % 10 == 0)
            std::cout << std::endl;
    }
}