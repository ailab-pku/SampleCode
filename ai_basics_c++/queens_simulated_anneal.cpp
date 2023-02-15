#include "problem/queens.hpp"
#include "algorithm/simulated_anneal.hpp"

int n = 500;

double queen_state_value_estimator(const QueensState& state){
    return state.action_space().size() + state.current_row() * 1.5;
}

double temperature_schedule(int time){
    return exp(n - time);
}

int main(){

    QueensState s(n);
    SimulatedAnneal<QueensState> sa(s, 1e-8);

    sa.search(queen_state_value_estimator, temperature_schedule, 10000);

    return 0;
}