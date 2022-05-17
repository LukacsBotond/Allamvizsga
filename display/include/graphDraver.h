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
    bool graph_mask[graph_Height][graph_Width]{0}; // this marks where the values are in the graph

    double findMax(const std::vector<double> &y);
    void convert_Array_To_graph_Mask(const std::vector<double> &y);
    //void convert_Array_To_graph_Mask(std::vector<double> &x, const std::vector<double> &y);
    void fill_Graph_Row();

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
    //void plotArray(std::vector<double> &x, std::vector<double> &y);
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
    std::string maxstr = std::to_string(findMax(y));
    convert_Array_To_graph_Mask(y);
    // first line
    // std::cout << "currentLine" << (int)currentLine << std::endl;
    insertChar(0, transChartoCharSet(maxstr[0]));
    insertChar(1, transChartoCharSet(maxstr[1]));

    // mid lanes
    while (currentLine < maxLineNr - 2)
    {
        // std::cout << "currentLine" << (int)currentLine << std::endl;
        insertChar(2, transChartoCharSet('|'));
        fill_Graph_Row();
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
double GRAPHDRAVER::findMax(const std::vector<double> &y)
{
    auto max = max_element(std::begin(y), std::end(y));
    return max[0];
}
// TODO fix eveything
void GRAPHDRAVER::convert_Array_To_graph_Mask(const std::vector<double> &y)
{
    // unsigned int size = y.size();
    // const double step = (double)1.0 / size;
    // double currpos = 0;
    for (uint16_t i = 0; i < graph_Width; i++)
    {
        graph_mask[100][i] = true;
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
}

void GRAPHDRAVER::fill_Graph_Row()
{
    uint8_t startLine = this->currentLine;
    for (uint8_t i = 0; i < 8; i++)
    {
        for (uint16_t j = 0; j < graph_Width; j++)
        { // offset for the 3 starting character
            int pos = (((i - startLine) * lineHeight) * lineWidth) + (j + 24);

            if (graph_mask[i][j] > 0)
            {
                std::cout << "i" << i << "j" << j << " graph " << graph_mask[i][j] << " pos " << pos << std::endl;
                this->row[pos] == this->fg_Color;
            }
            /* --the plotArray function already fills the background
            else{
                this->row[pos] == this->bg_Color;
            }
            */
        }
    }
}
