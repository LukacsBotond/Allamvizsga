#include "./include/characterDisplay.h"
#include <cstring>
#include <algorithm>

CHARACTERDISPLAY::CHARACTERDISPLAY(uint16_t bg_Color,
                                   uint16_t fg_Color) : bg_Color(bg_Color), fg_Color(fg_Color)
{
}

void CHARACTERDISPLAY::insertChar(uint8_t position, const uint8_t *charSet)
{
    for (int bit = 0; bit < 8; bit++)
    {
        uint8_t mask = 128 >> bit;
        std::cout << "mask" << (int)mask << std::endl;
        for (uint16_t i = 0; i < lineHeight; i++)
        {
            int id = i * 240 + position * 8 + bit;
            if (charSet[i] & mask)
                row[id] = fg_Color;
            std::cout << "pos: " << (int)id << "color :" << (int)row[id] << std::endl;
        }
    }
}

const uint8_t *CHARACTERDISPLAY::transChartoCharSet(char character) const
{
    return charset[character - ' '];
}

void CHARACTERDISPLAY::printLine(std::string str)
{
    // clean row*
    std::fill_n(row, rowSize, bg_Color);
    int size = str.size();
    const uint8_t *charset;
    // put string in a line
    for (int i = 0; i < size; i++)
    {
        std::cout << str[i] << " kodolva: ";
        charset = transChartoCharSet(str[i]);
        insertChar(i, charset);
        for (int j = 0; j < 8; j++)
        {
            std::cout << (int)charset[j] << " ";
        }
        std::cout << std::endl;
    }
    // dumpRow();
    writeLine();
}

//* DEBUG
void DISPLAYDRIVER::dumpRow()
{
    std::cout << "character write dump row\n";
    for (int i = 0; i < rowSize; i++)
    {
        std::cout << row[i] << " ";
        sleep_ms(1);
    }
}
