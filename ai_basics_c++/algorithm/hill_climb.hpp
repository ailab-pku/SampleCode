#pragma once

#include <ctime>
#include <random>
#include <type_traits>
#include <iostream>

#include "../interface/state_local.hpp"
#include "../utils/selection.hpp"

// hill climbing with roulette algorithm & random restart
template <typename StateLocalType> 
class HillClimb{
private:
    
    StateLocalType initial_state;
 
    typedef double (*ValueEstimatorType) (const StateLocalType&);

    static_assert(std::is_base_of<StateLocalBase, StateLocalType>::value, "StateLocalType not derived from StateLocalBase.");

    const StateLocalType& sample_path(ValueEstimatorType value_of, double target_value){
        
        static StateLocalType state;
        StateLocalType new_state;
        std::vector<double> values;
        int index;

        state = initial_state;

        while (value_of(state) < target_value and state.neighbor_count() != 0){
            
            values.clear();
            
            for (index = 0; index < state.neighbor_count(); ++ index){
                
                new_state = state.neighbor(index);
                values.push_back(value_of(new_state));
            }
            
            index = roulette_selection(values);
            state = state.neighbor(index);
        }

        return state;
    }

public:

    HillClimb(const StateLocalType& state) : initial_state(state) {}
    
    void search(ValueEstimatorType value_of, float target_value, int iterations){

        StateLocalType state;

        for (int i = 0; i < iterations; ++ i){
            std::cout << "<begin>" << std::endl;
            state = sample_path(value_of, target_value);
            if (value_of(state) >= target_value){
                state.show();
            }
            std::cout << value_of(state) << std::endl;
            std::cout << "<end>" << std::endl;
        }
    }
};