#include "include/common.h"

using namespace std;

uint16_t common::swap_bytes(uint16_t color)
{
    return (color >> 8) | (color << 8);
}
