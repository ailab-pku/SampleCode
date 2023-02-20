#pragma once

#include <stack>
#include <vector>
#include <cfloat>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include "../interface/game_state.hpp"
#include "../utils/show_path.hpp"

template<typename GameStateType> 
class FullGameSearch{
private:

    GameStateType initial_state;

    typedef typename GameStateType::ActionBaseType ActionType;
    
    static_assert(std::is_base_of<GameStateBase<ActionType>, GameStateType>::value, "GameStateType not derived from GameStateBase.");

public:

    FullGameSearch(const GameStateType& state) : initial_state(state) {}

    void search(){

        // items: (state, num of actions explored at state)
        std::stack<std::pair<GameStateType, int> > states_stack;

        // record next state of a state
        std::unordered_map<GameStateType, GameStateType> next_state_of;
        
        // outcomes of a state, if players are rational 
        std::unordered_map<GameStateType, std::vector<double> > outcomes_of;
        
        states_stack.push(std::make_pair(initial_state, 0));

        GameStateType state, new_state;
        std::pair<GameStateType, int> state_action;
        int action_id, best_action_id;
        double best_value;

        while (not states_stack.empty()){
            
            state_action = states_stack.top();
            states_stack.pop();
            state = state_action.first;

            // next action to try
            action_id = state_action.second;

            // if current state is terminal state
            if (state.done()){

                outcomes_of[state] = state.outcomes();
            
            } else if (action_id < state.action_space().size()){
                
                // next action when tracing back to current state
                states_stack.push(std::make_pair(state, action_id+1));

                // try current action, get new state
                new_state = state.next(state.action_space()[action_id]);
                states_stack.push(std::make_pair(new_state, 0));
  
            } else {
                // if current state is not terminal state but all actions are explored

                best_action_id = 0;
                best_value = -DBL_MAX;
                
                for (action_id = 0; action_id < state.action_space().size(); ++ action_id){
                    
                    new_state = state.next(state.action_space()[action_id]);
                    
                    if (outcomes_of[new_state][state.active_player()] > best_value){
                        
                        best_action_id = action_id;
                        best_value = outcomes_of[new_state][state.active_player()];
                    }
                }

                new_state = state.next(state.action_space()[best_action_id]);
                outcomes_of[state] = outcomes_of[new_state];
                next_state_of[state] = new_state;
            }
        }

        show_path(next_state_of, initial_state);
    }
};