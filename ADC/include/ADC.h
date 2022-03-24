#pragma once
#include "./IADC.h"

class IADC
{
public:
    /*
    check for new data on FIFO
    if there is new data then use that value as the read channel
    prepare ADC for freerunning mode to insert values to memory through DAM channel
    */
    virtual void setupFIFO() = 0;

    /*
        @param chanel select active chanel to read from
        * CAN THROW NOSUCHPORT if incorrect port was given
    */
    virtual void setADCSelect(uint8_t chanel) = 0;
    /*
        @return which ADC channel is in use currently
    */
    virtual uint getADCSelect() = 0;
    /*
        start in freerunning mode, waits till the buffer fills up then returns
        this function blocks internally
    */
    virtual void start_freeRunning() = 0;

    /*
        @param change the adc clock divisor so lower clock can be achived
    */
    virtual void set_clkDiv(uint div) = 0;

    /*
        @return the ADC measurement frequency
    */
    virtual double get_clkHz() = 0;
    /*
        @return how many elements the array have
    */
    virtual uint16_t getCaptureDepth() = 0;
    /*
        @return the array
        * CAN THROW NULLEXCEPT if the array is null
    */
    virtual uint16_t *getCaptureBuff() = 0;

    //! debug
    /*
        print samples trough the serial port
    */
    virtual void printSamples() = 0;

    /*
        !for testing only
        @param buff must be CAPTURE_DEPTH size
    */
    virtual void setCaptureBuff(uint16_t *buff, uint16_t buffSize) = 0;
};

class ADC : public IADC
{
private:
    uint dma_chan;
    // uint dma_chan1;
    uint16_t CAPTURE_DEPTH = 255;
    uint16_t *capture_buf;
    // holds which part of the capture_buff is used for writing
    // cannot be used for reading
    bool usedIndex = 0;
    uint32_t adc_div = 96;
    dma_channel_config cfg;
    // dma_channel_config cfg1;

public:
    // setup the DMA channels
    // allocate the storage arrays
    // inicialize the ADC
    ADC();
    ~ADC();

    void setupFIFO() override;

    //* can throw exception
    void setADCSelect(uint8_t chanel) override;
    uint getADCSelect() override;
    void start_freeRunning() override;
    void set_clkDiv(uint div) override;
    double get_clkHz() override;
    uint16_t getCaptureDepth() override;
    uint16_t *getCaptureBuff() override;

    //! debug
    void printSamples() override;
    void setCaptureBuff(uint16_t *buff, uint16_t buffSize) override;
    
};