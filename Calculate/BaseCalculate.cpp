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
    int actRes1, actRes2, actRes3;
    std::cout << cleanup->AVGVoltage(capture_buf, CAPTURE_DEPTH) << std::endl;
    uint8_t sw1Out = sw1->getOutput();
    uint8_t sw2Out = sw2->getOutput();
    uint8_t sw3Out = sw3->getOutput();
    uint res1 = GetOutResitance(sw1->getOutput());
    uint res2 = GetOutResitance(sw2->getOutput());
    uint res3 = GetOutResitance(sw3->getOutput());

    std::cout << "ActiveResistances: sw1: " << res1 << " sw2: " << res2 << " sw3: " << res3 << std::endl;

    


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

//* --------------------- Private functions -------------

int BASECALCULATE::GetOutResitance(uint8_t OutPort)
{
    //high impedance is connected
    if (OutPort == 0)
    {
        return -1;
    }//low high resistor is connected
    else if (OutPort <= 2)
    {
        return sw1->getResistor(0);
    }//low reseistor is connected
    else if (OutPort <= 4)
    {
        return sw1->getResistor(1);
    }
    else //both connected in parallel
    {
        uint res1 = sw1->getResistor(0);
        uint res2 = sw1->getResistor(1);
        return (res1 * res2) / (res1 + res2);
    }
}
