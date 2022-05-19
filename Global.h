#pragma once
#include "ADC/include/IADC.h"
#include "ADC/include/ADC.h"
#include "controll/include/IAswitch.h"
#include "./common/include/common.h"

//* power level
#define HIGH 1
#define LOW 0

//* pins

//* ADC channels
#define ACD_CHANNEL_0 26
#define ACD_CHANNEL_1 27
#define ACD_CHANNEL_2 28

//* DIPSPLAY PINS
#define DISP_SPI_CHANNEL 1
#define DISP_FREQ UINT32_MAX
//#define DISP_FREQ 2
#define DISP_CS 13
#define DISP_SCK 14
#define DISP_MOSI 15
#define DISP_RESET 12
#define DISP_DC 11

//* DAC pins
#define DAC_SPI_CHANNEL 0
#define DAC_FREQ 2
#define DAC_SCK 2
#define DAC_MOSI 3
#define DAC_LDAC 4
#define DAC_CS 5
#define DAC_RESET 6
#define DAC_ENABLE 7
#define DAC_RSTSEL 8

//* SWITCH pins
#define SWITHCH1_1 16
#define SWITHCH1_2 17
#define SWITHCH2_1 18
#define SWITHCH2_2 19
#define SWITHCH3_1 20
#define SWITHCH3_2 21

//* LED pins

#define RED_LED_PIN 9
#define GREEN_LED_PIN 10

//* resistors

#define RESISTOR_LOW 100
#define RESISTOR_MID 330
#define RESISTOR_HIGH 4700

//*Power save pin
#define POWERS_SAVE_PIN 23

class IADC;
extern IADC *adc;

class COMMON;
extern COMMON *commonClass;

//!DEBUG

//if commented out then the normal program runs without test cases
//if not then only the test cases run
//#define TESTS

//disables ADC read
//#define ADCDISABLE
