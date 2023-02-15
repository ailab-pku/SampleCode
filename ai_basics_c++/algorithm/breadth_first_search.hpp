#pragma once

#include <type_traits>
#include <unordered_set>
#include <queue>

#include "../interface/state.hpp"

template<typename StateType> 
class BreadthFirstSearch{
private:

    StateType initial_state;

    typedef typename StateType::ActionBaseType ActionType;
    
    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

public:

    BreadthFirstSearch(const StateType& state) : initial_state(state) {}

    void search(){

        std::queue<StateType> states_queue;

        // to prevent duplicate states appearing in the queue
        std::unordered_set<StateType> explored_states;
        
        states_queue.push(initial_state);
        explored_states.insert(initial_state);

        StateType state, new_state;

        // when states_queue is empty, all states are explored, there's no new state for expansion
        while (not states_queue.empty()){

            state = states_queue.front();
            states_queue.pop();

            if (state.success()){
                state.show();
                continue;
            }
            
            if (state.fail()){
                continue;
            }

            // consider all valid actions
            for (ActionType action : state.action_space()){
                
                new_state = state.next(action);
                
                // if new_state is not explored
                if (explored_states.find(new_state) == explored_states.end()){
                    
                    states_queue.push(new_state);
                    explored_states.insert(new_state);
                }
            }
        }  
    }
};