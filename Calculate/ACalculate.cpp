#include "./include/ACalculate.h"
#include "../Global.h"

ACALCULATE::ACALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller, IADCORRECTER *adccorrecter)
{
    this->values = values;
    this->cleanup = cleanup;
    this->controller = controller;
    this->adccorrecter = adccorrecter;
#ifndef ADCDISABLE // disables the correction if the ADC is disabled
    this->adccorrecter->loadIcalculate(this);
    this->adccorrecter->init();
#endif
}

ACALCULATE::~ACALCULATE()
{
    delete values;
    delete cleanup;
    delete controller;
    delete adccorrecter;
}

void ACALCULATE::SameOut3ChannelRepeat(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3)
{
    std::vector<double> valuesVector;
    std::string measurement;
    measurement = std::to_string(sw1);
    measurement += std::to_string(sw2);
    measurement += std::to_string(sw3);
    // std::cout << "test measurement, sw settings: " << measurement << "\n";
    //  todo input chech, no short circuits
    for (uint8_t i = 0; i < 3; i++)
    {
        // chose which ADC channel to read from
        // multicore_fifo_push_blocking(((-i) + 2) % 3);//reverse order
        if (i == 0)
        {
            multicore_fifo_push_blocking(2);
        }
        if (i == 1)
        {
            multicore_fifo_push_blocking(1);
        }
        if (i == 2)
        {
            multicore_fifo_push_blocking(0);
        }

        controller->setSwithcSetting(sw1, sw2, sw3);

        // start ADCc
        
        this->startSemaphoreRelease();
        // WAIT for ADC
        this->doneSemaphoreAquire();
        
        uint16_t *capture_buf = adc->getCaptureBuff();
        uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
        valuesVector.push_back(cleanup->AVGVoltage(adccorrecter->offsetCorrection(capture_buf, CAPTURE_DEPTH), CAPTURE_DEPTH));
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
        controller->setSwithcSetting(1, 1, 1);
    }
    if (!setMeasurement(measurement, valuesVector))
        std::cerr << "ACALCULATE addMeasurement fail" << std::endl;

    // values->printMeasurements();
}

double ACALCULATE::calcResistance(std::vector<std::string> &measurements)
{
    if (measurements.size() == 0)
    {
        std::cout << "no measurements were given\n";
        throw NOSUCHMEASUREMENT("ACALCULATE calcResistance empty measurements vector");
    }

    std::vector<double> measurementData;
    // double voltageDrop = 0;
    double mAmper;
    double bestmAmper = 0;
    double voltageDrop = 0;
    std::string bestMeasurement = measurements.at(0);
    for (int i = 0; i < measurements.size(); i += 2)
    {
        // std::cout << "measurementCalculate: " << i << " val: " << measurements[i] << "LENGHT:" << measurements.size() << std::endl;
        //  std::string store = "405";
        //   std::cout << "store: " << store << std::endl;

        //    measurementData = this->values->getMeasurement(measurements[i]);
        try
        {
            measurementData = this->values->getMeasurement(measurements[i]);
        }
        catch (NOSUCHMEASUREMENT &e)
        {
            std::cout << e.what() << std::endl;
        }
        // first port is not used
        if (measurements[i][0] - '0' == 0)
        {
            mAmper = (3.3 - measurementData[1]) / this->controller->getTotResistor(measurements[i][1] - '0');
        }
        else
        { // 2. or 3. is not used
            mAmper = (3.3 - measurementData[0]) / this->controller->getTotResistor(measurements[i][0] - '0');
        }
        if (mAmper > bestmAmper)
        {
            bestmAmper = mAmper;
            bestMeasurement = measurements[i];
        }
    }

    // this->values->printMeasurements();
    // std::cout << "bestMeasurement: " << bestMeasurement << " mAmper: " << bestmAmper << std::endl;

    measurementData = this->values->getMeasurement(bestMeasurement);
    // first port is not used
    if (bestMeasurement[0] - '0' == 0)
    {
        voltageDrop = measurementData[1] - measurementData[2];
        if (voltageDrop < 0) // if negative make it positive
            voltageDrop *= -1;
        // std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[1] << std::endl;
        // std::cout << "voltage drop on unkown resistor:" << voltageDrop << std::endl;
        // std::cout << "voltage drop on 2. resistor:" << measurementData[2] << std::endl;
        return ((voltageDrop) / bestmAmper);
    }
    else
    {                                      // 2. or 3. is not used
        if (bestMeasurement[1] - '0' == 0) // 2. is not used
        {
            voltageDrop = measurementData[0] - measurementData[2];
            if (voltageDrop < 0) // if negative make it positive
                voltageDrop *= -1;
            // std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
            // std::cout << "voltage drop on unkown resistor:" << voltageDrop << std::endl;
            // std::cout << "voltage drop on 2. resistor:" << measurementData[2] << std::endl;

            return ((voltageDrop) / bestmAmper);
        }
        else
        { // 3. is not used
            voltageDrop = measurementData[0] - measurementData[1];
            if (voltageDrop < 0) // if negative make it positive
                voltageDrop *= -1;
            // std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
            // std::cout << "voltage drop on unkown resistor:" << voltageDrop << std::endl;
            // std::cout << "voltage drop on 2. resistor:" << measurementData[1] << std::endl;

            return ((voltageDrop) / bestmAmper);
        }
    }
}

std::vector<double> ACALCULATE::getMeasurement(const std::string &measurement) const
{
    return this->values->getMeasurement(measurement);
}

bool ACALCULATE::setMeasurement(const std::string &measurement, const std::vector<double> &valuesVector)
{
    return this->values->addMeasurement(measurement, valuesVector);
}

void ACALCULATE::cleanMesurements()
{
    return this->values->cleanMeasurements();
}

bool ACALCULATE::IsAnythingConnected(const double avgVoltage, const uint8_t portMode)
{
    return this->cleanup->IsAnythingConnected(avgVoltage, portMode);
}