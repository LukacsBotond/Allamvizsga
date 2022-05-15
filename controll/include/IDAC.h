#pragma once

#include "./spi.h"

#define DAC_INTERNAL_REFERENCE_ALLW_DOWN 0x00012000
#define DAC_INTERNAL_REFERENCE_DEF_UP 0x00010000
#define DAC_INTERNAL_REFERENCE_ALLW_UP 0x00011000

#define DAC_COMM_WRITE_BUFF_A 0x00
#define DAC_COMM_WRITE_BUFF_B 0x02
#define DAC_COMM_WRITE_BUFF_C 0x04
#define DAC_COMM_WRITE_BUFF_D 0x06

#define DAC_COMM_WRITE_BUFF_LOAD_A 0b00010000
#define DAC_COMM_WRITE_BUFF_LOAD_B 0b00010010
#define DAC_COMM_WRITE_BUFF_LOAD_C 0b00010100
#define DAC_COMM_WRITE_BUFF_LOAD_D 0b00010110

#define DAC_COMM_WRITE_BUFF_LOAD_ALL_A 0b00100000
#define DAC_COMM_WRITE_BUFF_LOAD_ALL_B 0b00100010
#define DAC_COMM_WRITE_BUFF_LOAD_ALL_C 0b00100100
#define DAC_COMM_WRITE_BUFF_LOAD_ALL_D 0b00100110

#define DAC_COMM_WRITE_LOAD_ALL 0b00110000
#define DAC_COMM_WRITE_LOAD_ALLCHANNEL 0b00110100 // write the same value to all channel and load it

class IDAC
{
public:
    virtual void setVoltageOnChannel(uint16_t voltage, uint8_t command) = 0;
};
