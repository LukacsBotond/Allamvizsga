// #include "./include/BaseCalculate.h"
// #include "../Global.h"

// BASECALCULATE::BASECALCULATE(IVALUES *values, ICLEANINPUT *cleanup, ISWITCHCONTROLLER *controller, IADCORRECTER *adccorrecter)
// {
//     this->values = values;
//     this->cleanup = cleanup;
//     this->controller = controller;
//     this->adccorrecter = adccorrecter;
// #ifndef ADCDISABLE // disables the correction if the ADC is disabled
//     this->adccorrecter->loadIcalculate(this);
//     this->adccorrecter->init();
// #endif
// }

// BASECALCULATE::~BASECALCULATE()
// {
//     delete values;
//     delete cleanup;
//     delete controller;
//     delete adccorrecter;
// }

// void BASECALCULATE::SameOut3ChannelRepeat(const uint8_t sw1, const uint8_t sw2, const uint8_t sw3)
// {
//     std::vector<double> valuesVector;
//     std::string measurement;
//     measurement = std::to_string(sw1);
//     measurement += std::to_string(sw2);
//     measurement += std::to_string(sw3);
//     // std::cout << "test measurement, sw settings: " << measurement << "\n";
//     //  todo input chech, no short circuits
//     for (int i = 0; i < 3; i++)
//     {
//         // chose which ADC channel to read from
//         multicore_fifo_push_blocking((i + 1) % 3);
//         controller->setSwithcSetting(1, sw1);
//         controller->setSwithcSetting(2, sw2);
//         controller->setSwithcSetting(3, sw3);
//         // start ADCc
//         this->startSemaphoreRelease();

//         // WAIT for ADC
//         this->doneSemaphoreAquire();

//         // uint16_t *capture_buf = adc->getCaptureBuff();
//         // uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
//         valuesVector.push_back(cleanup->AVGVoltage(adccorrecter->offsetCorrection(adc->getCaptureBuff(), adc->getCaptureDepth()), adc->getCaptureDepth()));
//         // calculateResult();
//         /*
//         adc->printSamples();
//         std::cout << "corrected:\n";
//         capture_buf = adccorrecter->offsetCorrection(capture_buf, CAPTURE_DEPTH);
//         for (int index = 0; index < adc->getCaptureDepth(); index++)
//         {
//             std::cout << capture_buf[index] << " ";
//             if (index % 20 == 19)
//             {
//                 std::cout << std::endl;
//             }
//         }
// */
//         // drain
//         controller->setSwithcSetting(1, 5);
//         controller->setSwithcSetting(2, 5);
//         controller->setSwithcSetting(3, 5);
//     }
//     if (!setMeasurement(measurement, valuesVector))
//         std::cerr << "BaseCalculate addMeasurement fail" << std::endl;

//     // values->printMeasurements();
// }

// double BASECALCULATE::calcResistance(std::vector<std::string> &measurements)
// {
//     if (measurements.size() == 0)
//     {
//         std::cout << "no measurements were given\n";
//         throw NOSUCHMEASUREMENT("BASECALCULATE calcResistance empty measurements vector");
//     }

//     std::vector<double> measurementData;
//     // double voltageDrop = 0;
//     double mAmper;
//     double bestmAmper = 0;
//     double voltageDrop = 0;
//     std::string bestMeasurement = measurements.at(0);
//     for (int i = 0; i < measurements.size(); i += 2)
//     {
//         std::cout << "HERE end6" << i << std::endl;
//         std::cout << "measurementCalculate: " << i << " val: " << measurements[i] << "LENGHT:" << measurements.size() << std::endl;
//         //  std::string store = "405";
//         //   std::cout << "store: " << store << std::endl;

//         measurementData = this->values->getMeasurement(measurements[i]);
//         // first port is not used
//         if (measurements[i][0] - '0' == 0)
//         {
//             mAmper = (3.3 - measurementData[1]) / this->controller->getTotResistorFromMode(measurements[i][1] - '0');
//         }
//         else
//         { // 2. or 3. is not used
//             mAmper = (3.3 - measurementData[0]) / this->controller->getTotResistorFromMode(measurements[i][0] - '0');
//         }
//         if (mAmper > bestmAmper)
//         {
//             bestmAmper = mAmper;
//             bestMeasurement = measurements[i];
//         }
//     }
//     // this->values->printMeasurements();

//     // std::cout << "bestMeasurement: " << bestMeasurement <<" mAmper: " << bestmAmper << std::endl;

//     measurementData = this->values->getMeasurement(bestMeasurement);
//     // first port is not used
//     if (bestMeasurement[0] - '0' == 0)
//     {
//         voltageDrop = measurementData[1] - measurementData[2];
//         if (voltageDrop < 0) // if negative make it positive
//             voltageDrop *= -1;
//         // std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[1] << std::endl;
//         // std::cout << "voltage drop on unkown resistor:" << voltageDrop << std::endl;
//         // std::cout << "voltage drop on 2. resistor:" << measurementData[2] << std::endl;
//         return ((voltageDrop) / bestmAmper);
//     }
//     else
//     {                                      // 2. or 3. is not used
//         if (bestMeasurement[1] - '0' == 0) // 2. is not used
//         {
//             voltageDrop = measurementData[0] - measurementData[2];
//             if (voltageDrop < 0) // if negative make it positive
//                 voltageDrop *= -1;
//             // std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
//             // std::cout << "voltage drop on unkown resistor:" << voltageDrop << std::endl;
//             // std::cout << "voltage drop on 2. resistor:" << measurementData[2] << std::endl;
//             return ((voltageDrop) / bestmAmper);
//         }
//         else
//         { // 3. is not used
//             voltageDrop = measurementData[0] - measurementData[1];
//             if (voltageDrop < 0) // if negative make it positive
//                 voltageDrop *= -1;
//             // std::cout << "voltage drop on 1. resistor:" << 3.3 - measurementData[0] << std::endl;
//             // std::cout << "voltage drop on unkown resistor:" << voltageDrop << std::endl;
//             // std::cout << "voltage drop on 2. resistor:" << measurementData[1] << std::endl;

//             return ((voltageDrop) / bestmAmper);
//         }
//     }
// }

// std::vector<double> BASECALCULATE::getMeasurement(const std::string &measurement) const
// {
//     // std::cout << "measurementClaculate get: "
//     //           << " val: " << measurement << "LENGHT:" << measurement.size() << std::endl;
//     return this->values->getMeasurement(measurement);
// }

// bool BASECALCULATE::setMeasurement(const std::string &measurement, std::vector<double> valuesVector)
// {
//     // std::cout << "measurementCalculate set: "
//     //          << " val: " << measurement << "LENGHT:" << measurement.size() << std::endl;
//     return this->values->addMeasurement(measurement, valuesVector);
// }

// void BASECALCULATE::cleanMesurements()
// {
//     // std::cout << "measurementCalculate clear: \n";
//     return this->values->cleanMeasurements();
// }

// bool BASECALCULATE::IsAnythingConnected(const double avgVoltage, const uint8_t portMode)
// {
//     return this->cleanup->IsAnythingConnected(avgVoltage, portMode);
// }

// /*
// //* --------------------- Private functions -------------
// void BASECALCULATE::calculateResult()
// {
//     uint16_t *capture_buf = adc->getCaptureBuff();
//     uint16_t CAPTURE_DEPTH = adc->getCaptureDepth();
//     // std::cout << cleanup->AVGVoltage(capture_buf, CAPTURE_DEPTH) << std::endl;
//     //  uint8_t sw1Out = controller->getSwithcSetting(1);// aswitch1->getOutput();
//     //  uint8_t sw2Out = controller->getSwithcSetting(2);//aswitch2->getOutput();
//     //  uint8_t sw3Out = controller->getSwithcSetting(3);//aswitch3->getOutput();
//     uint res1 = controller->getswTotResistorSetting(1);
//     uint res2 = controller->getswTotResistorSetting(2);
//     uint res3 = controller->getswTotResistorSetting(3);

//     // std::cout << "ActiveResistances: sw1: " << res1 << " sw2: " << res2 << " sw3: " << res3 << std::endl;
// }
// */
