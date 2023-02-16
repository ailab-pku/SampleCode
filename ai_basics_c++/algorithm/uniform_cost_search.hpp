#pragma once

#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>
#include <type_traits>

#include "heuristic_search.hpp"
#include "../interface/state.hpp"

template<typename StateType>
class UniformCostSearch : protected HeuristicSearch<StateType>{
private:

    static double state_value_estimator(const StateType& state){
        return -state.cumulative_cost();
    }
    
public:
    
    UniformCostSearch(const StateType& state) : HeuristicSearch<StateType>(state) {}

    void search(){
        HeuristicSearch<StateType>::search(state_value_estimator);
    }
};