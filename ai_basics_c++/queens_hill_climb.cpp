#include "interface/state_local.hpp"
#include "problem/queens_swap.hpp"
#include "algorithm/hill_climb.hpp"

typedef StateLocalWrapper<QueensState> QueensStateLocal; 

typedef StateLocalWrapper<QueensSwapState> QueensSwapStateLocal;

double queens_swap_state_value_estimator(const QueensSwapStateLocal& state){
    return state.state().action_space().size() - state.state().conflicts();
}

int main(){
    QueensSwapState ss(8);
    QueensSwapStateLocal qss(ss);
    HillClimb<QueensSwapStateLocal> hcs(qss);
    hcs.search(queens_swap_state_value_estimator, 28, 100);
    return 0;
}