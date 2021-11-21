#include "./include/ADC.h"
#include "../Exceptions/include/NoSuchPort.h"

ADC::ADC()
{
    capture_buf = new uint16_t[CAPTURE_DEPTH];
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_init();
    adc_select_input(0);

    adc_set_clkdiv(0);
    adc_set_temp_sensor_enabled(false);
}

ADC::~ADC(){
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

    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);

    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);

    // Pace transfers based on availability of ADC samples
    channel_config_set_dreq(&cfg, DREQ_ADC);

    dma_channel_configure(dma_chan, &cfg,
                          capture_buf,   // dst
                          &adc_hw->fifo, // src
                          CAPTURE_DEPTH, // transfer count
                          true           // start immediately
    );
}

void ADC::waitDMAFull()
{
    dma_channel_wait_for_finish_blocking(dma_chan);
}


void ADC::adcSelect(int chanel)
{
    if (chanel > 3 || chanel < 0)
    {
        std::cout << "Wrong channel selected, 0 will be selected\n";
        adc_select_input(26);
        throw NOSUCHPORT("port must be 0/1/2");
    }
    adc_select_input(chanel);
}

void ADC::start_freeRunning()
{
    adc_run(true);
}
void ADC::stop_freeRunning()
{
    adc_run(false);
}

void ADC::set_clkDiv(uint div){
    adc_set_clkdiv(div);
}

uint16_t* ADC::getCaptureBuff(){

}


void ADC::printSamples()
{
    for (int i = 0; i < CAPTURE_DEPTH; ++i)
    {
        printf("%-3d, ", capture_buf[i]);
        if (i % 10 == 9)
            printf("\n");
    }
}