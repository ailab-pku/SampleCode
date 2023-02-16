#pragma once

#include <ctime>
#include <cmath>
#include <random>
#include <type_traits>

#include "../interface/state_local.hpp"

template <typename StateLocalType> 
class SimulatedAnneal{
private:

    const double epsilon;    
    StateLocalType initial_state;
    
    mutable std::default_random_engine random_engine;
    mutable std::uniform_real_distribution<double> uniform_dist;
    mutable std::uniform_int_distribution<int> uniform_int_dist;
        
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
            
            index = uniform_int_dist(random_engine) % state.neighbor_count();
            
            new_state = state.neighbor(index);

            value_diff = value_of(new_state) - value_of(state);
            temperature = temperature_at(t);
            
            if (value_diff > 0 or uniform_dist(random_engine) < exp(value_diff / temperature)){
                state = new_state;
            }
        }
        
        return state;
    }

public:

    SimulatedAnneal(const StateLocalType& state, double _epsilon) : initial_state(state), uniform_dist(0, 1) , epsilon(_epsilon) {
        random_engine.seed(time(nullptr));
    }
    
    void search(ValueEstimatorType value_of, TemperatureScheduleType temperature_at, int iterations){
       
        StateLocalType state;
        
        for (int i = 0; i < iterations; ++ i){
            
            state = sample_path(value_of, temperature_at);
            state.show();
        }
    }
};