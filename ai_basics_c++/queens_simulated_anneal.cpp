#include "problem/queens.hpp"
#include "algorithm/simulated_anneal.hpp"

int n = 20;

typedef StateLocalWrapper<QueensState> QueensStateLocal; 

double queen_state_value_estimator(const QueensStateLocal& state){
    return state.neighbor_count() + state.state().current_row() * 1.5;
}

double temperature_schedule(int time){
    return exp(n - time);
}

int main(){

    QueensState s(n);
    SimulatedAnneal<QueensStateLocal> sa(s, 1e-8);

    sa.search(queen_state_value_estimator, temperature_schedule, 1000);

    return 0;
}