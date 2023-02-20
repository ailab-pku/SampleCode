#pragma once

#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <iostream>

#include "../interface/state.hpp"
#include "../utils/show_path.hpp"

template<typename StateType> 
class BreadthFirstSearch{
private:

    StateType initial_state;

    typedef typename StateType::ActionBaseType ActionType;
    
    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

public:

    BreadthFirstSearch(const StateType& state) : initial_state(state) {}

    void search(bool tree_search=true, bool require_path=true){

        std::queue<StateType> states_queue;

        // record previous state of a state
        std::unordered_map<StateType, StateType> last_state_of;

        // to prevent duplicate states appearing in the queue, if tree_search==false
        std::unordered_set<StateType> explored_states;
        
        states_queue.push(initial_state);
        explored_states.insert(initial_state);

        StateType state, new_state;

        // when states_queue is empty, all states are explored, there's no new state for expansion
        while (not states_queue.empty()){

            state = states_queue.front();
            states_queue.pop();

            if (state.success()){
                if (require_path){
                    show_reversed_path(last_state_of, state);
                } else {
                    state.show();
                }
                continue;
            }
            
            if (state.fail()){
                continue;
            }

            // consider all valid actions
            for (ActionType action : state.action_space()){
                
                new_state = state.next(action);
                
                // if new state is not explored, start from new state
                if (tree_search){

                    states_queue.push(new_state);
                    
                    if (require_path){
                        last_state_of[new_state] = state;
                    }
                } else if (explored_states.find(new_state) == explored_states.end()){
                    
                    states_queue.push(new_state);
                    explored_states.insert(new_state);

                    if (require_path){
                        last_state_of[new_state] = state;
                    }
                }
            }
        }  
    }
};