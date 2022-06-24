#pragma once
#include "./characterDisplay.h"
#include <vector>
#include <algorithm>

//*Graph area is 294*224 in landscape mode
#define graph_Width 294
#define graph_Height 224
class GRAPHDISPLAY : public CHARACTERDISPLAY
{
private:
    // uint8_t graph_mask[graph_Height][graph_Width]{0}; // this marks where the values are in the graph
    double MaxY = 0;
    void findMax(const std::vector<double> &y);
    // void convert_Array_To_graph_Mask(const std::vector<double> &y);
    // void convert_Array_To_graph_Mask(std::vector<double> &x, const std::vector<double> &y);
    void fill_Graph_Row(const std::vector<double> &y);

public:
    GRAPHDISPLAY(SPI *spi, uint16_t bg_Color, uint16_t fg_Color);
    ~GRAPHDISPLAY();

    /*
    This function lineraly plots the Y vector
    Get the current measurements, it auto scales so max value will be the Y max value and the rest scales accordingly.
    MaxX will have a default value of 3.3, but it is changable, if other range is required.
    y value need to be under 100mA or it won't display properly
    @param  y: const std::vector<double>, the measurement array, recommended to have
    over 300 values, won't be visible on the screen anyway
    */
    void plotArray(const std::vector<double> &y, const std::string &Yscale, const std::string &title, const double MaxX = 3.3);
    /*
    Get the *current* measurements, while the x axys is the 2nd array.
    *Both aray must match in size!
    y value need to be under 100mA or it won't display properly
    @param  y: const std::vector<double>, the measurement array, not recommended to have
    over 300 values, won't be visible on the screen anyway
    @param  x: const std::vector<double>, the x array, need to have the same number of elements as y
    */
    // void plotArray(std::vector<double> &x, std::vector<double> &y);
};