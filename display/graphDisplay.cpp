#include "./include/graphDisplay.h"

GRAPHDISPLAY::GRAPHDISPLAY(SPI *spi, uint16_t bg_Color, uint16_t fg_Color) : CHARACTERDISPLAY(spi, bg_Color, fg_Color)
{
}

GRAPHDISPLAY::~GRAPHDISPLAY()
{
    delete spi;
}

void GRAPHDISPLAY::plotArray(const std::vector<double> &y, const std::string &Yscale, const std::string &title,const double MaxX)
{
    if (y.size() == 0)
    { // there is nothing to draw
        return;
    }
    this->set_Continous_Write_Area();
    findMax(y);
    std::string maxYstr = std::to_string(MaxY);
    std::string maxXstr = std::to_string(MaxX);

    //  first line
    std::fill_n(row, rowSize, bg_Color);
    // mid lanes
    while (currentLine < maxLineNr - 2)
    {
        if (currentLine == 0)
        {
            insertChar(0, transChartoCharSet(maxYstr[0]));
            insertChar(1, transChartoCharSet(maxYstr[1]));
            insertChar(1, transChartoCharSet(maxYstr[2]));
        }
        if (currentLine == 1)
        {
            insertChar(0, transChartoCharSet(Yscale[0]));
            insertChar(1, transChartoCharSet(Yscale[1]));
            insertChar(2, transChartoCharSet(Yscale[2]));
        }
        insertChar(3, transChartoCharSet('|'));
        fill_Graph_Row(y);
        writeLine();
        std::fill_n(row, rowSize, bg_Color);
    }

    // n-1 lane
    std::fill_n(row, rowSize, bg_Color);
    for (uint8_t i = 3; i < this->lineWidth / 8; i++)
    {
        insertChar(i, transChartoCharSet('_'));
    }
    writeLine();

    // last Line
    std::fill_n(row, rowSize, bg_Color);
    insertChar(0, transChartoCharSet('0'));
    for(int i=7;i<title.size()+7; i++){
        insertChar(i, transChartoCharSet(title[i-7]));
    }

    uint8_t countPos = 0;
    for (uint8_t i = (lineWidth / 8) - 3; i < lineWidth / 8; i++)
    {
        insertChar(i, transChartoCharSet(maxXstr[countPos]));
        countPos++;
    }
    writeLine();
}


//* Private functions

void GRAPHDISPLAY::findMax(const std::vector<double> &y)
{
    auto max = max_element(std::begin(y), std::end(y));
    MaxY = max[0];
}

void GRAPHDISPLAY::fill_Graph_Row(const std::vector<double> &y)
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
            row[pos] = fg_Color;
        }
    }
}
