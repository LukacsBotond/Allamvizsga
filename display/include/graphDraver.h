#pragma once
#include "./characterDisplay.h"
#include <vector>
#include <algorithm>

class GRAPHDRAVER : public CHARACTERDISPLAY
{
private:
    double findMax(const std::vector<double> &y);

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
    void plotArray(std::vector<double> &x, std::vector<double> &y);
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
    this->set_Continous_Write_Area();
    //* value will be between 0-100
    //! test, make the border
    std::fill_n(row, rowSize, bg_Color);
    std::string maxstr = std::to_string(findMax(y));
    // first line
    std::cout << "currentLine" << (int)currentLine << std::endl;
    insertChar(0, transChartoCharSet(maxstr[0]));
    insertChar(1, transChartoCharSet(maxstr[1]));
    insertChar(2, transChartoCharSet('|'));
    writeLine();
    // mid lanes

    while (currentLine < maxLineNr -2)
    {
        std::cout << "currentLine" << (int)currentLine << std::endl;
        std::fill_n(row, rowSize, bg_Color);
        insertChar(2, transChartoCharSet('|'));
        writeLine();
        sleep_ms(100);
    }
    // last lane
    std::fill_n(row, rowSize, bg_Color);
    for (uint8_t i = 2; i < this->lineWidth / 8; i++)
    {
        insertChar(i, transChartoCharSet('_'));
    }
    std::cout << "currentLine" << (int)currentLine << std::endl;
    writeLine();
}

void GRAPHDRAVER::plotArray(std::vector<double> &x, std::vector<double> &y)
{
}

//* Private functions
double GRAPHDRAVER::findMax(const std::vector<double> &y)
{
    auto max = max_element(std::begin(y), std::end(y));
    return max[0];
}