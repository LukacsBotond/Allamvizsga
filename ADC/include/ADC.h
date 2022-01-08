#pragma once
#include "./IADC.h"
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
    ADC();
    ~ADC();

    void setupFIFO() override;
    void waitDMAFull() override;

    //* can throw exception
    void setADCSelect(int chanel) override;
    uint getADCSelect() override;
    void start_freeRunning() override;
    void stop_freeRunning() override;
    void set_clkDiv(uint div) override;
    uint16_t getCaptureDepth() override;
    uint16_t *getCaptureBuff() override;

    //! debug
    void printSamples() override;
};