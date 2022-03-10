#include "./include/BaseCalculate.h"
#include "../Global.h"

BASECALCULATE::BASECALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller, IADCORRECTER *adccorrecter) : values(values), cleanup(cleanup), controller(controller), adccorrecter(adccorrecter)
{
    this->adccorrecter->loadIcalculate(this);
    this->adccorrecter->init();
}

BASECALCULATE::~BASECALCULATE()
{
    delete values;
    delete cleanup;
    delete controller;
    delete adccorrecter;
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
        multicore_fifo_push_blocking((i + 1) % 3);
        controller->setSwithcSetting(1, sw1);
        controller->setSwithcSetting(2, sw2);
        controller->setSwithcSetting(3, sw3);
        // start ADCc
        this->startSemaphoreRelease();

        // WAIT for ADC
        this->doneSemaphoreAquire();

        // uint16_t *capture_buf = adc->getCaptureBuff();
        // uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
        valuesVector.push_back(cleanup->AVGVoltage(adccorrecter->offsetCorrection(adc->getCaptureBuff(), adc->getCaptureDepth()), adc->getCaptureDepth()));
        // calculateResult();
        /*
        adc->printSamples();
        std::cout << "corrected:\n";
        capture_buf = adccorrecter->offsetCorrection(capture_buf, CAPTURE_DEPTH);
        for (int index = 0; index < adc->getCaptureDepth(); index++)
        {
            std::cout << capture_buf[index] << " ";
            if (index % 20 == 19)
            {
                std::cout << std::endl;
            }
        }
*/
        // drain
        controller->setSwithcSetting(1, 5);
        controller->setSwithcSetting(2, 5);
        controller->setSwithcSetting(3, 5);
    }
    if (!this->values->addMeasurement(measurement, valuesVector))
        std::cerr << "BaseCalculate addMeasurement fail" << std::endl;

    // values->printMeasurements();
}

double BASECALCULATE::calcResistance(std::vector<std::string> &measurements)
{

    std::vector<double> measurementData;
    double voltageDrop = 0;
    double mAmper = 0;
    double bestmAmper = 0;
    std::string bestMeasurement;
    for (int i = 0; i < measurements.size(); i += 2)
    {
        measurementData = this->values->getMeasurement(measurements[i]);
        // first port is not used
        if (measurements[i][0] - '0' == 0)
        {
            mAmper = (3.3 - measurementData[1]) / this->controller->getTotResistorFromMode(measurements[i][1] - '0');
        }
        else
        { // 2. or 3. is not used
            mAmper = (3.3 - measurementData[0]) / this->controller->getTotResistorFromMode(measurements[i][0] - '0');
        }

        if (mAmper > bestmAmper)
        {
            bestmAmper = mAmper;
            bestMeasurement = measurements[i];
        }
    }

    measurementData = this->values->getMeasurement(bestMeasurement);
    // first port is not used
    if (bestMeasurement[0] - '0' == 0)
    {
        std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[1] << std::endl;
        std::cout << "voltage drop on unkown resistor:" << measurementData[1] - measurementData[2] << std::endl;
        std::cout << "voltage drop on 2. resistor:" << measurementData[2] << std::endl;
        return ((measurementData[1] - measurementData[2]) / bestmAmper);
    }
    else
    {                                      // 2. or 3. is not used
        if (bestMeasurement[1] - '0' == 0) // 2. is not used
        {
            std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
            std::cout << "voltage drop on unkown resistor:" << measurementData[0] - measurementData[2] << std::endl;
            std::cout << "voltage drop on 2. resistor:" << measurementData[2] << std::endl;
            return ((measurementData[0] - measurementData[2]) / bestmAmper);
        }
        else
        { // 3. is not used
            std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
            std::cout << "voltage drop on unkown resistor:" << measurementData[0] - measurementData[1] << std::endl;
            std::cout << "voltage drop on 2. resistor:" << measurementData[1] << std::endl;
            return ((measurementData[0] - measurementData[1]) / bestmAmper);
        }
    }
}

std::vector<double> BASECALCULATE::getMeasurement(std::string measurement)
{
    return this->values->getMeasurement(measurement);
}

bool BASECALCULATE::IsAnythingConnected(double avgVoltage, uint8_t portMode)
{
    return this->cleanup->IsAnythingConnected(avgVoltage, portMode);
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
