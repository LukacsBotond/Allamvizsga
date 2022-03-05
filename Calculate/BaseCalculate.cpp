#include "./include/BaseCalculate.h"
#include "../Global.h"

BASECALCULATE::BASECALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller) : values(values), cleanup(cleanup), controller(controller) {}

BASECALCULATE::~BASECALCULATE()
{
    delete values;
    delete cleanup;
    delete controller;
}

void BASECALCULATE::startMeasurements()
{
    /*
    sleep_ms(100);
    SameOut3ChannelRepeat(4, 0, 5);
    sleep_ms(100);
    SameOut3ChannelRepeat(5, 0, 4);
    sleep_ms(100);
    SameOut3ChannelRepeat(2, 0, 5);
    sleep_ms(100);
    SameOut3ChannelRepeat(5, 0, 2);
    //values->printMeasurements();
    std::cout << "resitor: " << calcResistance("405") << std::endl;
    std::cout << "resitor: " << calcResistance("205") << std::endl;
    */
}

void BASECALCULATE::SameOut3ChannelRepeat(uint8_t sw1, uint8_t sw2, uint8_t sw3)
{
    std::vector<double> valuesVector;
    std::string measurement;
    measurement = std::to_string(sw1);
    measurement += std::to_string(sw2);
    measurement += std::to_string(sw3);
    std::cout << "test measurement, sw settings: " << measurement << "\n";
    // todo input chech, no short circuits
    for (int i = 0; i < 3; i++)
    {
        // chose which ADC channel to read from
        multicore_fifo_push_blocking(i);

        this->startSemaphoreRelease();
        controller->setSwithcSetting(1, sw1);
        controller->setSwithcSetting(2, sw2);
        controller->setSwithcSetting(3, sw3);
        // start ADC

        // WAIT for ADC
        this->doneSemaphoreAquire();

        uint16_t *capture_buf = adc->getCaptureBuff();
        uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
        valuesVector.push_back(cleanup->AVGVoltage(capture_buf, CAPTURE_DEPTH));
        // calculateResult();
        //adc->printSamples();
        // drain
        controller->setSwithcSetting(1, 5);
        controller->setSwithcSetting(2, 5);
        controller->setSwithcSetting(3, 5);
        sleep_ms(100);
    }
    if (!this->values->addMeasurement(measurement, valuesVector))
        std::cerr << "BaseCalculate addMeasurement fail" << std::endl;
}

double BASECALCULATE::calcResistance(std::string measurement)
{
    std::vector<double> measurementData;
    std::vector<double> measurementData2;
    double voltageDrop = 0;
    // TODO check if resistor, not blindly calculate resistance
    try
    {
        measurementData = this->values->getMeasurement(measurement);
    }
    catch (NOSUCHMEASUREMENT &err)
    {
        std::cout << err.what() << std::endl;
        SameOut3ChannelRepeat(measurement[0] - '0', measurement[1] - '0', measurement[2] - '0');
        calcResistance(measurement);
    }
    // there is values from one side, check reverse side
    /*
    try
    {
        measurementData2 = this->values->getMeasurement(measurement);
    }
    catch (NOSUCHMEASUREMENT &err)
    {
        std::cout << err.what() << std::endl;
        SameOut3ChannelRepeat(measurement[2] - '0', measurement[1] - '0', measurement[0] - '0');
        calcResistance(measurement);
    }
    */
    double mAmper = 0;
    // first port is not used
    if (measurement[0] - '0' == 0)
    {

        mAmper = (3.3 - measurementData[1]) / this->controller->getTotResistorFromMode(measurement[1] - '0');
        return ((measurementData[1] - measurementData[2]) / mAmper);
    }
    else
    { // 2. or 3. is not used
        mAmper = (3.3 - measurementData[0]) / this->controller->getTotResistorFromMode(measurement[0] - '0');
        std::cout << "current: " << mAmper << " resistance: " << this->controller->getTotResistorFromMode(measurement[0] - '0') << std::endl;
        if (measurement[1] - '0' == 0) // 2. is not used
        {
            std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
            std::cout << "voltage drop on unkown resistor:" << measurementData[0] - measurementData[2] << std::endl;
            std::cout << "voltage drop on 2. resistor:" << measurementData[2] << std::endl;
            return ((measurementData[0] - measurementData[2]) / mAmper);
        }
        else
        { // 3. is not used
            std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
            std::cout << "voltage drop on unkown resistor:" << measurementData[0] - measurementData[1] << std::endl;
            std::cout << "voltage drop on 2. resistor:" << measurementData[1] << std::endl;
            return ((measurementData[0] - measurementData[1]) / mAmper);
        }
    }
}

std::vector<double> BASECALCULATE::getMeasurement(std::string measurement)
{
    return this->values->getMeasurement(measurement);
}

bool BASECALCULATE::IsAnythingConnected(double avgVoltage, uint8_t portMode){
    return this->cleanup->IsAnythingConnected(avgVoltage,portMode);
}

//* --------------------- Private functions -------------
void BASECALCULATE::calculateResult()
{
    uint16_t *capture_buf = adc->getCaptureBuff();
    uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
    // std::cout << cleanup->AVGVoltage(capture_buf, CAPTURE_DEPTH) << std::endl;
    //  uint8_t sw1Out = controller->getSwithcSetting(1);// aswitch1->getOutput();
    //  uint8_t sw2Out = controller->getSwithcSetting(2);//aswitch2->getOutput();
    //  uint8_t sw3Out = controller->getSwithcSetting(3);//aswitch3->getOutput();
    uint res1 = controller->getswTotResistorSetting(1);
    uint res2 = controller->getswTotResistorSetting(2);
    uint res3 = controller->getswTotResistorSetting(3);

    // std::cout << "ActiveResistances: sw1: " << res1 << " sw2: " << res2 << " sw3: " << res3 << std::endl;
}
