#include "./include/ACalculate.h"
#include "../Global.h"

ACALCULATE::ACALCULATE(IVALUES *values, ISWITCHCONTROLLER *controller, IADCORRECTER *adccorrecter)
{
    this->values = values;
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
    delete controller;
    delete adccorrecter;
}

uint16_t *ACALCULATE::ChannelMeasure(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3, uint32_t channelId, bool saveMeasurement)
{
    /*
    std::string measurement;
    measurement = std::to_string(sw1);
    measurement += std::to_string(sw2);
    measurement += std::to_string(sw3);
    std::cout << "test measurement, sw settings: " << measurement << "\n";
    */
    multicore_fifo_push_blocking(channelId);
    // prepare measuring
    this->startSemaphoreRelease(true);
    // std::cout << "ACALCULATE startPrepare\n";
    controller->prepareSwitchSetting(sw1, sw2, sw3);

    // measure
    this->startSemaphoreRelease();
    // std::cout << "ACALCULATE startMeasure\n";
    controller->setSwithcSetting(sw1, sw2, sw3);

    // WAIT for ADC
    this->doneSemaphoreAquire();
    this->doneSemaphoreAquire(true);
    uint16_t *capture_buf = adc->getCaptureBuff();
    uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();

    /*
        //! delete
        adc->printSamples();
        sleep_ms(200);
        //! end delete
    */
    // drain
    controller->setSwithcSetting(0, 0, 0);
    return adccorrecter->offsetCorrection(capture_buf, CAPTURE_DEPTH);
}
std::vector<double> ACALCULATE::SameOut3ChannelRepeat(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3, bool saveMeasurement)
{
    std::vector<double> valuesVector;
    std::string measurement;
    measurement = std::to_string(sw1);
    measurement += std::to_string(sw2);
    measurement += std::to_string(sw3);
    uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
    //std::cout << "test measurement, sw settings: " << measurement << "\n";
    for (uint8_t i = 3; i >= 1; --i)
    {
        valuesVector.push_back(cleanInput->AVGVoltage(this->ChannelMeasure(sw1, sw2, sw3, i - 1, saveMeasurement), CAPTURE_DEPTH));
    }
    if (saveMeasurement)
    {
        if (!setMeasurement(measurement, valuesVector))
            std::cerr << "ACALCULATE addMeasurement fail" << std::endl;
    }
    return valuesVector;
}

double ACALCULATE::calcResistance(std::vector<std::string> &measurements)
{
    if (measurements.size() == 0)
    {
        std::cout << "no measurements were given\n";
        throw NOSUCHMEASUREMENT("ACALCULATE calcResistance empty measurements vector");
    }
    std::vector<double> measurementData;
    double mAmper;
    double bestmAmper = 0;
    double voltageDrop = 0;
    std::string bestMeasurement = measurements.at(0);
    for (int i = 0; i < measurements.size(); i += 2)
    {
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

double ACALCULATE::diodeThreshold(std::string &measurement)
{
    std::vector<double> measurementData;
    double voltageDrop;
    measurementData = this->values->getMeasurement(measurement);
    if (measurement[0] - '0' == 0) // 0 is not used
    {
        voltageDrop = measurementData[1] - measurementData[2];
        if (voltageDrop < 0) // if negative make it positive
            voltageDrop *= -1;
        return voltageDrop;
    }
    else // 2. or 3. is not used
    {
        if (measurement[1] - '0' == 0) // 2. is not used
        {
            voltageDrop = measurementData[0] - measurementData[2];
            if (voltageDrop < 0) // if negative make it positive
                voltageDrop *= -1;
            return voltageDrop;
        }
        else
        { // 3. is not used
            voltageDrop = measurementData[0] - measurementData[1];
            if (voltageDrop < 0) // if negative make it positive
                voltageDrop *= -1;
            return voltageDrop;
        }
    }
    return 0;
}

double ACALCULATE::CalcCapacitance_nF(uint16_t *samples, uint16_t CAPTURE_DEPTH, double sapleRate, const uint8_t swMode)
{
    uint ChargeTime_ns = 0;
    uint ticktime = 1000000000 / (sapleRate);
    if (samples[CAPTURE_DEPTH - 2] < 3000)
    {
        //std::cout << "Capacitor charge too slowly, decrease the resistance or the sample frequency" << std::endl;
        throw CAPACITORSLOWCHARGE("Capacitor charge too slowly, decrease the resistance or the sample frequency");
    }

    for (uint i = 0; i < CAPTURE_DEPTH; i++)
    {
        ChargeTime_ns += ticktime;
        if (samples[i] >= 2855)
        {
            break;
        }
    }

    //std::cout << "ChargeTime_us: " << ChargeTime_ns << " resistance " << this->controller->getTotResistor(swMode) <<" ticTime: " << ticktime << std::endl;

    return ChargeTime_ns/this->controller->getTotResistor(swMode);
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
    return cleanInput->IsAnythingConnected(avgVoltage, portMode);
}