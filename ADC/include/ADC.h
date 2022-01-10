#pragma once
#include "./IADC.h"

class IADC
{
public:
    //check for new data on FIFO 
    virtual void setupFIFO() = 0;
    virtual void waitDMAFull() = 0;
    virtual void setADCSelect(uint8_t chanel) = 0;
    virtual uint getADCSelect() = 0;
    virtual void start_freeRunning() = 0;
    virtual void stop_freeRunning() = 0;
    virtual void set_clkDiv(uint div) = 0;
    virtual uint16_t getCaptureDepth() = 0;
    virtual uint16_t *getCaptureBuff() = 0;

    //! debug
    virtual void printSamples() = 0;
};

class ADC : public IADC
{
private:
    uint dma_chan;
    uint dma_chan1;
    uint16_t CAPTURE_DEPTH = 255;
    uint16_t **capture_buf;
    //holds which part of the capture_buff is used for writing
    //cannot be used for reading
    bool usedIndex = 0;

    dma_channel_config cfg;
    dma_channel_config cfg1;

public:
    //setup the DMA channels
    //allocate the storage arrays
    //inicialize the ADC
    ADC();
    ~ADC();

    void setupFIFO() override;
    void waitDMAFull() override;

    //* can throw exception
    void setADCSelect(uint8_t chanel) override;
    uint getADCSelect() override;
    void start_freeRunning() override;
    void stop_freeRunning() override;
    void set_clkDiv(uint div) override;
    uint16_t getCaptureDepth() override;
    uint16_t *getCaptureBuff() override;

    //! debug
    void printSamples() override;
};