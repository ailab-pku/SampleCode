#include <iostream>

#include "problem/queens.hpp"
#include "problem/queens_swap.hpp"

#include "algorithm/depth_first_search.hpp"
#include "algorithm/breadth_first_search.hpp"
#include "algorithm/heuristic_search.hpp"
#include "algorithm/uniform_cost_search.hpp"

double queens_state_value_estimator(const QueensState& s){
    return -s.cumulative_cost();
}

int main(){
    //QueensState state(8);
    //BreadthFirstSearch<QueensState> s(state);
    //DepthFirstSearch<QueensState> s(state);
    //UniformCostSearch<QueensState> s(state);
    //HeuristicSearch<QueensState> s(state);
    
    QueensSwapState state(8);
    UniformCostSearch<QueensSwapState> s(state);

    s.search();

    return 0;
}
