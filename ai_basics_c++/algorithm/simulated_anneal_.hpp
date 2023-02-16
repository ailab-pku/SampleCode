#pragma once

#include <ctime>
#include <cmath>
#include <random>
#include <type_traits>

#include "../interface/state.hpp"

template <typename StateType> 
class SimulatedAnneal{
private:

    const double epsilon;
    StateType initial_state;
    
    mutable std::default_random_engine random_engine;
    mutable std::uniform_real_distribution<double> uniform_dist;
    mutable std::uniform_int_distribution<int> uniform_int_dist;
    
    typedef typename StateType::ActionBaseType ActionType;
    
    typedef double (*ValueEstimatorType) (const StateType&);
    typedef double (*TemperatureScheduleType) (int);
    
    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

    const StateType& sample_path(ValueEstimatorType value_of, TemperatureScheduleType temperature_at){
        
        static StateType state;
        StateType new_state;
        ActionType action;
        double value_diff, temperature;

        state = initial_state;
        
        for (int t = 0; not (state.success() or state.fail()); ++ t){
            
            action = state.action_space()[uniform_int_dist(random_engine) % state.action_space().size()];
            
            new_state = state.next(action);

            value_diff = value_of(new_state) - value_of(state);
            temperature = temperature_at(t);
            
            if (temperature < epsilon){
                state = new_state;
                break;
            }

            if (value_diff > 0 or uniform_dist(random_engine) < exp(value_diff / temperature)){
                state = new_state;
            }
        }
        
        return state;
    }

public:

    SimulatedAnneal(const StateType& state, double _epsilon) : initial_state(state), uniform_dist(0, 1) , epsilon(_epsilon) {
        random_engine.seed(time(nullptr));
    }
    
    void search(ValueEstimatorType value_of, TemperatureScheduleType temperature_at, int iterations){
       
        StateType state;
        
        for (int i = 0; i < iterations; ++ i){
            
            state = sample_path(value_of, temperature_at);
            
            if (state.success()){
                state.show();
                break;
            }
        }
    }
};
