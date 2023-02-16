#pragma once

#include <ctime>
#include <random>
#include <type_traits>

#include "../interface/state_local.hpp"

// hill climbing with roulette algorithm & random restart
template <typename StateLocalType> 
class HillClimb{
private:
    
    StateLocalType initial_state;

    mutable std::default_random_engine random_engine;
    mutable std::uniform_real_distribution<double> uniform_dist;
    
    typedef double (*ValueEstimatorType) (const StateLocalType&);

    static_assert(std::is_base_of<StateLocalBase, StateLocalType>::value, "StateLocalType not derived from StateLocalBase.");

    // roulette algorithm
    int select_index_with_values(const std::vector<double>& values) const {
        
        double sum = 0, r;
        int index = 0;
        
        for (int i = 0; i < values.size(); ++ i){
            
            sum += values[i];
            
            r = uniform_dist(random_engine);
            
            if (r < values[i] / sum){
                index = i;
            }         
        }

        return index;
    }

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
            
            index = select_index_with_values(values);
            state = state.neighbor(index);
        }

        return state;
    }

public:

    HillClimb(const StateLocalType& state) : initial_state(state) {
        random_engine.seed(time(nullptr));
    }
    
    void search(ValueEstimatorType value_of, float target_value, int iterations){

        StateLocalType state;

        for (int i = 0; i < iterations; ++ i){
            state = sample_path(value_of, target_value);
            state.show();
        }
    }
};