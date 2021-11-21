#include "./include/BaseCalculate.h"

BASECALCULATE::BASECALCULATE(IVALUES *values, IADC *adc, ICLEANINPUT *cleanup, IASWITCH *sw1, IASWITCH *sw2, IASWITCH *sw3) : values(values), adc(adc), cleanup(cleanup), sw1(sw1), sw2(sw2), sw3(sw3) {}

BASECALCULATE::~BASECALCULATE()
{
    delete adc;
    delete values;
}

IVALUES *BASECALCULATE::calculateRes()
{
    uint16_t *capture_buf = adc->getCaptureBuff();
    uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();


    std::cout<<cleanup->AVGVoltage(capture_buf,CAPTURE_DEPTH)<<std::endl;
    /*
    std::cout << "BASECALCULATE test\n";

    for (int i = 0; i < CAPTURE_DEPTH; ++i)
    {
        printf("%-3d, ", capture_buf[i]);
        if (i % 10 == 9)
            printf("\n");
    }
    */
    return values;
}
