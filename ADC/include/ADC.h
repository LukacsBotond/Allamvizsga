#pragma once
#include "./IADC.h"

class ADC : public IADC
{
private:
    uint dma_chan;
    uint16_t CAPTURE_DEPTH = 255;
    uint16_t *capture_buf;

public:
    ADC();
    ~ADC();

    void setupFIFO() override;
    void waitDMAFull() override;

    //* can throw exception
    void adcSelect(int chanel) override;
    void start_freeRunning() override;
    void stop_freeRunning() override;
    void set_clkDiv(uint div) override;
    uint16_t getCaptureDepth() override;
    uint16_t *getCaptureBuff() override;
    //! debug
    void printSamples() override;
};