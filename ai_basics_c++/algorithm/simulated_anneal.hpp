#pragma once

#include <ctime>
#include <cmath>
#include <random>
#include <type_traits>
#include <iostream>

#include "../interface/state_local.hpp"
#include "../utils/random_variables.hpp"

template <typename StateLocalType> 
class SimulatedAnneal{
private:

    double epsilon;    
    StateLocalType initial_state;
        
    typedef double (*ValueEstimatorType) (const StateLocalType&);
    typedef double (*TemperatureScheduleType) (int);

    static_assert(std::is_base_of<StateLocalBase, StateLocalType>::value, "StateLocalType not derived from StateLocalBase.");

    const StateLocalType& sample_path(ValueEstimatorType value_of, TemperatureScheduleType temperature_at){
        
        static StateLocalType state;
        StateLocalType new_state;
        double value_diff, temperature;
        int index;

        state = initial_state;
        temperature = temperature_at(0);
        
        for (int t = 0; state.neighbor_count() > 0 and temperature >= epsilon; ++ t){
            
            index = RandomVariables::uniform_int() % state.neighbor_count();
            
            new_state = state.neighbor(index);

            value_diff = value_of(new_state) - value_of(state);
            temperature = temperature_at(t);
            
            if (value_diff > 0 or RandomVariables::uniform_real() < exp(value_diff / temperature)){
                state = new_state;
            }
        }
        
        return state;
    }

public:

    SimulatedAnneal(const StateLocalType& state) : initial_state(state) {}
    
    void search(ValueEstimatorType value_of, TemperatureScheduleType temperature_at, 
        int iterations, double target_value, double _epsilon=1e-10){
       
        StateLocalType state;
        
        epsilon = _epsilon;

        for (int i = 0; i < iterations; ++ i){
            std::cout << "<begin>" << std::endl;
            state = sample_path(value_of, temperature_at);
            if (value_of(state) >= target_value){
                state.show();
            }
            std::cout << value_of(state) << std::endl;
            std::cout << "<end>" << std::endl;
        }
    }
};