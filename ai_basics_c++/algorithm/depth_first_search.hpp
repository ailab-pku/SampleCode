#pragma once

#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include <stack>

#include "../interface/state.hpp"
#include "../utils/show_path.hpp"

template <typename StateType> 
class DepthFirstSearch{
private:

    StateType initial_state;

    typedef typename StateType::ActionBaseType ActionType;
    
    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

public:

    DepthFirstSearch(const StateType& state) : initial_state(state) {}
    
    void search(bool tree_search=true, bool require_path=true){

        // items: (state, num of actions explored at state)
        std::stack<std::pair<StateType, int> > states_stack;

        // record previous state of a state
        std::unordered_map<StateType, StateType> last_state_of;

        // to prevent duplicate states appearing in the stack, if tree_search==false
        std::unordered_set<StateType> explored_states;
        
        states_stack.push(std::make_pair(initial_state, 0));
        explored_states.insert(initial_state);

        StateType state, new_state;
        std::pair<StateType, int> state_action;
        int action_id;

        while (not states_stack.empty()){

            state_action = states_stack.top();
            states_stack.pop();
            state = state_action.first;
            
            // next action to try
            action_id = state_action.second;

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
            
            // if not all the actions has been explored
            if (action_id < state.action_space().size()){

                // next action when tracing back to current state
                states_stack.push(std::make_pair(state, action_id+1));
                
                // try current action, get new state
                new_state = state.next(state.action_space()[action_id]);
                
                // if new state is not explored, start from new state
                if (tree_search){
                    
                    states_stack.push(std::make_pair(new_state, 0));
                    
                    if (require_path){
                        last_state_of[new_state] = state;
                    }
                } else if (explored_states.find(new_state) == explored_states.end()){
                    
                    states_stack.push(std::make_pair(new_state, 0));
                    explored_states.insert(new_state);
                    
                    if (require_path){
                        last_state_of[new_state] = state;
                    }
                }
            }
        }
    }
};