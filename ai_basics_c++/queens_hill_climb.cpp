#include "interface/state_local.hpp"
#include "problem/queens.hpp"
#include "algorithm/hill_climb.hpp"

typedef StateLocalWrapper<QueensState> QueensStateLocal; 

double queen_state_value_estimator(const QueensStateLocal& state){
    return state.state().success() ? (state.state().n_queens() << 1) : (state.state().action_space().size() + state.state().current_row());
}

int main(){
    QueensState s(8);
    QueensStateLocal qs(s);
    HillClimb<QueensStateLocal> hc(qs);
    hc.search(queen_state_value_estimator, 0xffffff, 80);
    return 0;
}