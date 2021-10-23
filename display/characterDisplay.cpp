#include "./include/characterDisplay.h"

const uint8_t* CHARACTERDISPLAY::transChartoCharSet(char character){
    return charset[character-40];
}

void CHARACTERDISPLAY::printLine(std::string str){
    int size = str.size();
    const uint8_t* charset;
    for(int i=0;i<size;i++){
        std::cout<<str[i] <<" kodolva: ";
        charset = transChartoCharSet(str[i]);
        for(int j=0;j<8;j++){
            std::cout<<(int)charset[j]<<" ";
        }
        std::cout<<std::endl;
    }
}
