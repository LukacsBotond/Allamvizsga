#pragma once
#include "ADC/include/IADC.h"
#include "ADC/include/ADC.h"
#include "controll/include/IAswitch.h"

//* power level
#define HIGH 1
#define LOW 0

//* pins

//* ADC channels
#define ACD_CHANNEL_0 26
#define ACD_CHANNEL_1 27
#define ACD_CHANNEL_2 28

//* DIPSPLAY PINS
#define DISP_SPI_CHANNEL 0
#define DISP_FREQ 100000
#define DISP_MISO 4
#define DISP_CS 5
#define DISP_SCK 6
#define DISP_MOSI 7
#define DISP_RESET 8
#define DISP_DC 9

//* SWITCH pins
#define SWITHCH1_LOW 16
#define SWITHCH1_HIGH 17
#define SWITHCH2_LOW 18
#define SWITHCH2_HIGH 19
#define SWITHCH3_LOW 20
#define SWITHCH3_HIGH 21

//* resistors

#define RESISTOR_LOW 330
#define RESISTOR_MID 4700
#define RESISTOR_HIGH 0

class IADC;
extern IADC *adc;
//static queue_t ADCSelect_queue;
