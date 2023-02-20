#pragma once

#include <iostream>
#include <type_traits>

#include "../interface/population.hpp"
#include "../utils/random_variables.hpp"

template <typename PopulationType>
class GeneticAlgorithm{
private:
    
    PopulationType _population;

    typedef typename PopulationType::ChromosomeBaseType ChromosomeType;
    
    static_assert(std::is_base_of<PopulationBase<ChromosomeType>, PopulationType>::value, "PopulationType not derived from PopulationBase.");

public:

    GeneticAlgorithm(const PopulationType& init_population) : _population(init_population) {}
    
    std::vector<ChromosomeType> population() const {return _population.population();} 

    std::vector<double> adaptability() const {return _population.adaptability();}

    void evolve(int n){

        for (int i = 0; i < n; ++ i){

            std::cout << "<epoch " << i << " begin>" << std::endl;

            _population.cross();
            _population.mutate();
            _population.show();

            std::cout << "<epoch " << i << " end>" << std::endl;
        }
    }
};