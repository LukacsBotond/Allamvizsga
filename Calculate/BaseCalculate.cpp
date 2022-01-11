#include "./include/BaseCalculate.h"
#include "../Global.h"

BASECALCULATE::BASECALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller) : values(values), cleanup(cleanup), controller(controller) {}

BASECALCULATE::~BASECALCULATE()
{
    delete values;
    delete cleanup;
    delete controller;
}

void BASECALCULATE::calculateResult()
{
    uint16_t *capture_buf = adc->getCaptureBuff();
    uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
    std::cout << cleanup->AVGVoltage(capture_buf, CAPTURE_DEPTH) << std::endl;
    //uint8_t sw1Out = controller->getSwithcSetting(1);// aswitch1->getOutput();
    //uint8_t sw2Out = controller->getSwithcSetting(2);//aswitch2->getOutput();
    //uint8_t sw3Out = controller->getSwithcSetting(3);//aswitch3->getOutput();
    uint res1 = GetOutResitance(controller->getSwithcSetting(1));
    uint res2 = GetOutResitance(controller->getSwithcSetting(2));
    uint res3 = GetOutResitance(controller->getSwithcSetting(3));

    std::cout << "ActiveResistances: sw1: " << res1 << " sw2: " << res2 << " sw3: " << res3 << std::endl;
}

void BASECALCULATE::startMeasurements()
{
    sleep_ms(500);
    std::cout << "4 out 5 gnd\n\n";
    SameOut3ChannelRepeat(4, 0, 5);
    sleep_ms(500);
    std::cout << "5 gnd 4 out\n\n";
    SameOut3ChannelRepeat(5, 0, 4);
    sleep_ms(500);
    std::cout << "2 out 5 gnd\n\n";
    SameOut3ChannelRepeat(2, 0, 5);
    sleep_ms(500);
    std::cout << "5 gnd 2 out\n\n";
    SameOut3ChannelRepeat(5, 0, 2);


    values->printMeasurements();

}

//* --------------------- Private functions -------------

int BASECALCULATE::GetOutResitance(uint8_t OutPort)
{
    //high impedance is connected
    if (OutPort == 0)
    {
        return -1;
    } //low high resistor is connected
    else if (OutPort <= 2)
    {
        return controller->getResistorSetting(1, 0); // aswitch1->getResistor(0);
    }                                                //low reseistor is connected
    else if (OutPort <= 4)
    {
        return controller->getResistorSetting(1, 1); //aswitch1->getResistor(1);
    }
    else //both connected in parallel
    {
        uint res1 = controller->getResistorSetting(1, 0); //aswitch1->getResistor(0);
        uint res2 = controller->getResistorSetting(1, 1); //aswitch1->getResistor(1);
        return (res1 * res2) / (res1 + res2);
    }
}

void BASECALCULATE::SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3)
{
    std::vector<double> valuesVector;
    std::string measurement;
    measurement = std::to_string(sw1);
    measurement += std::to_string(sw2);
    measurement += std::to_string(sw3);
    std::cout << "test measurement, sw settings: " << measurement << "\n";
    //todo input chech, no short circuits
    for (int i = 0; i < 3; i++)
    {
        //chose which ADC channel to read from
        multicore_fifo_push_blocking(i);
        controller->setSwithcSetting(1, sw1);
        controller->setSwithcSetting(2, sw2);
        controller->setSwithcSetting(3, sw3);
        //start ADC
        this->startSemaphoreRelease();
        //WAIT for ADC
        this->doneSemaphoreAquire();

        uint16_t *capture_buf = adc->getCaptureBuff();
        uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
        valuesVector.push_back(cleanup->AVGVoltage(capture_buf, CAPTURE_DEPTH));

        //drain
        controller->setSwithcSetting(1, 5);
        controller->setSwithcSetting(2, 5);
        controller->setSwithcSetting(3, 5);
        sleep_ms(500);
    }

    this->values->addMeasurement(measurement, valuesVector);
}
