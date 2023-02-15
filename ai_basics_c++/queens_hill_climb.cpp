#include "problem/queens.hpp"
#include "algorithm/hill_climb.hpp"

double queen_state_value_estimator(const QueensState& state){
    return state.success() ? (state.n_queens() << 1) : (state.action_space().size() + state.current_row());
}

int main(){
    QueensState s(8);
    HillClimb<QueensState> hc(s);
    hc.search(queen_state_value_estimator, 1000);
    return 0;
}