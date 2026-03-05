#include "OrderBook.h"
#include "InputParser.h"
#include <iostream>
#include <string>

int main(){

    OrderBook ob; 

    std::string inputLine; 

    while (std::getline(std::cin, inputLine)){
        if(inputLine.empty()) continue; 

        Event e = getEvent(inputLine); 
        ob.processEvent(e);
    }
    
    std::cout <<"------------------------------------------------" << std::endl;

    return 0; 
}