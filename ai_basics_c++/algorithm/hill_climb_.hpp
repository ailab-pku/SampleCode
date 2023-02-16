#pragma once

#include <ctime>
#include <random>
#include <type_traits>

#include "../interface/state.hpp"

// hill climbing with roulette algorithm & random restart
template <typename StateType> 
class HillClimb{
private:
    
    StateType initial_state;

    mutable std::default_random_engine random_engine;
    mutable std::uniform_real_distribution<double> uniform_dist;

    typedef typename StateType::ActionBaseType ActionType;
    
    typedef double (*ValueEstimatorType) (const StateType&);

    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

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

    const StateType& sample_path(ValueEstimatorType value_of){
        
        static StateType state;
        StateType new_state;
        ActionType action;
        std::vector<double> values;
        int index;

        state = initial_state;

        while (not (state.fail() or state.success())){
            
            values.clear();
            
            for (ActionType action : state.action_space()){
                
                new_state = state.next(action);
                values.push_back(value_of(new_state));
            }
            
            index = select_index_with_values(values);
            action = state.action_space()[index];
            state = state.next(action);
        }

        return state;
    }

public:

    HillClimb(const StateType& state) : initial_state(state) {
        random_engine.seed(time(nullptr));
    }
    
    void search(ValueEstimatorType value_of, int iterations){

        StateType state;

        for (int i = 0; i < iterations; ++ i){
            
            state = sample_path(value_of);
            
            if (state.success()){
                state.show();
                break;
            }   
        }
    }
};

