#pragma once

#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>
#include <type_traits>

#include "../interface/state.hpp"
#include "../utils/show_path.hpp"

template<typename StateType>
class HeuristicSearch{
protected:

    StateType initial_state;

    typedef typename StateType::ActionBaseType ActionType;
    
    typedef double (*ValueEstimatorType) (const StateType&);

    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

public:
    
    HeuristicSearch(const StateType& state) : initial_state(state) {}

    void search(ValueEstimatorType value_of){
        
        auto cmp_state_less = [value_of] (const StateType& s1, const StateType& s2) -> bool {return value_of(s1) < value_of(s2);};
        std::priority_queue<StateType, std::vector<StateType>, decltype(cmp_state_less)> states_queue(cmp_state_less);
        std::unordered_map<StateType, double> best_value_of;
        std::unordered_map<StateType, StateType> last_state_of;
        StateType state, new_state;
        
        states_queue.push(initial_state);

        best_value_of[initial_state] = 0;

        while (not states_queue.empty()){

            state = states_queue.top();
            states_queue.pop();
            
            if (state.success()){
                break;
            }

            if (state.fail()){
                continue;
            }

            for (ActionType action : state.action_space()){

                new_state = state.next(action);
                
                if (best_value_of.find(new_state) == best_value_of.end() 
                    or value_of(new_state) > best_value_of[new_state]){
                    
                    best_value_of[new_state] = value_of(new_state);
                    states_queue.push(new_state);
                    
                    // record path
                    last_state_of[new_state] = state;
                }
            }
        }

        // output the best path
        if (state.success()){
            show_reversed_path(last_state_of, state);
        }
    }
};