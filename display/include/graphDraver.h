#pragma once
#include "./characterDisplay.h"
#include <vector>
#include <algorithm>

//*Graph area is 294*224 in landscape mode
#define graph_Width 294
#define graph_Height 224
class GRAPHDRAVER : public CHARACTERDISPLAY
{
private:
    // uint8_t graph_mask[graph_Height][graph_Width]{0}; // this marks where the values are in the graph
    double MaxY = 0;
    void findMax(const std::vector<double> &y);
    // void convert_Array_To_graph_Mask(const std::vector<double> &y);
    // void convert_Array_To_graph_Mask(std::vector<double> &x, const std::vector<double> &y);
    void fill_Graph_Row(const std::vector<double> &y);

public:
    GRAPHDRAVER(SPI *spi, uint16_t bg_Color, uint16_t fg_Color);
    ~GRAPHDRAVER();

    /*
    Get the *current* measurements, while the x axys is the number of measurements.
    y value need to be under 100mA or it won't display properly
    @param  y: const std::vector<double>, the measurement array, not recommended to have
    over 300 values, won't be visible on the screen anyway
    */
    void plotArray(const std::vector<double> &y);
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

GRAPHDRAVER::GRAPHDRAVER(SPI *spi, uint16_t bg_Color, uint16_t fg_Color) : CHARACTERDISPLAY(spi, bg_Color, fg_Color)
{
}

GRAPHDRAVER::~GRAPHDRAVER()
{
    delete spi;
}

void GRAPHDRAVER::plotArray(const std::vector<double> &y)
{
    if (y.size() == 0)
    { // there is nothing to draw
        return;
    }
    this->set_Continous_Write_Area();
    //* value will be between 0-100
    //! test, make the border
    std::fill_n(row, rowSize, bg_Color);
    findMax(y);
    std::string maxstr = std::to_string(MaxY);
    // convert_Array_To_graph_Mask(y);
    //  first line
    //  std::cout << "currentLine" << (int)currentLine << std::endl;
    insertChar(0, transChartoCharSet(maxstr[0]));
    insertChar(1, transChartoCharSet(maxstr[1]));

    // mid lanes
    while (currentLine < maxLineNr - 2)
    {
        // std::cout << "currentLine" << (int)currentLine << std::endl;
        insertChar(2, transChartoCharSet('|'));
        fill_Graph_Row(y);
        writeLine();
        std::fill_n(row, rowSize, bg_Color);
        // sleep_ms(100);
    }

    // last lane
    std::fill_n(row, rowSize, bg_Color);
    for (uint8_t i = 2; i < this->lineWidth / 8; i++)
    {
        insertChar(i, transChartoCharSet('_'));
    }
    // std::cout << "currentLine" << (int)currentLine << std::endl;
    writeLine();
}
/*
void GRAPHDRAVER::plotArray(std::vector<double> &x, std::vector<double> &y)
{
}
*/
//* Private functions
void GRAPHDRAVER::findMax(const std::vector<double> &y)
{
    auto max = max_element(std::begin(y), std::end(y));
    MaxY = max[0];
}
/*
// TODO fix eveything
void GRAPHDRAVER::convert_Array_To_graph_Mask(const std::vector<double> &y)
{
    // unsigned int size = y.size();
    // const double step = (double)1.0 / size;
    // double currpos = 0;
    for (uint16_t i = 0; i < graph_Width; i++)
    {
        graph_mask[100][i] = 1;
        // currpos += step;
        // std::cout << "Currpos:" << currpos << " step " << step << "graph" << graph_mask[(int)y.at((int)currpos)][(int)currpos] << std::endl;
        // sleep_ms(5);
    }
    //! Delete
    /*
        for (int i = 0; i < graph_Height; i++)
        {
            for (int j = 0; j < graph_Width; j++)
            {
                if (graph_mask[i][j])
                {
                    std::cout << "i" << i << "j" << j << " " << graph_mask[i][j] << std::endl;
                    sleep_ms(5);
                }
            }
        }
        */
/*}
 */
void GRAPHDRAVER::fill_Graph_Row(const std::vector<double> &y)
{
    const uint8_t startLine = currentLine;
    const uint ySize = y.size();
    const double YStep = MaxY / (graph_Height / 8);
    const double XStep = graph_Width / (double)ySize;
    const double upperRange = MaxY - (YStep) * (double)(startLine);
    const double lowerRange = MaxY - (YStep) * (double)(startLine + 1);
    const double mult = (double)YStep / (double)7.0;
    for (uint i = 0; i < ySize; i++)
    {
        if (y.at(i) >= lowerRange && y.at(i) <= upperRange)
        {
            double height = y.at(i) - lowerRange;
            int Yposition = ((7 - (int)(height / mult))) * lineWidth;
            int pos = (Yposition + 24 + (double)(XStep * i));
            // int pos = 0 + 24 + (double)(XStep * i);
            std::cout << "XSTEP: " << XStep << " offset: " << (double)(XStep * i) << " YPosition: " << Yposition << std::endl;
            row[pos] = fg_Color;
        }
    }
}
