#include "problem/queens.hpp"
#include "problem/queens_swap.hpp"
#include "algorithm/simulated_anneal.hpp"

int n = 50;

typedef StateLocalWrapper<QueensState> QueensStateLocal; 
typedef StateLocalWrapper<QueensSwapState> QueensSwapStateLocal;

// target: n*(n-1)/2
double queens_swap_state_value_estimator(const QueensSwapStateLocal& state){
    return state.state().action_space().size() - state.state().conflicts();
}

double temperature_schedule(int time){
    static double start_temp_log = n/log(n);
    return exp(start_temp_log - double(time)/n);
}

int main(){
    QueensSwapState ss(n);
    QueensSwapStateLocal qss(ss);
    SimulatedAnneal<QueensSwapStateLocal> sas(qss);

    sas.search(queens_swap_state_value_estimator, temperature_schedule, 10, (n*(n-1))/2, 1e-10);

    return 0;
}