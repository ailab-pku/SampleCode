#pragma once

#include <vector>

#include "random_variables.hpp"

// Roulette algorithm
int roulette_selection(const std::vector<double>& values){
    
    double sum = 0, r;
    int index = 0;
    
    for (int i = 0; i < values.size(); ++ i){
        
        sum += values[i];
        
        r = RandomVariables::uniform_real();
        
        if (r < values[i] / sum){
            index = i;
        }         
    }

    return index;
}