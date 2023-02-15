#include "problem/directed_graph.hpp"
#include "algorithm/depth_first_search.hpp"
#include "algorithm/breadth_first_search.hpp"
#include "algorithm/heuristic_search.hpp"
#include "algorithm/uniform_cost_search.hpp"

/*
double estimator(const DirectedGraphState& state) {
    return -state.cumulative_cost();
}
*/

int main(){
    DirectedGraph graph(6);
    graph.add_edge(0, 1, 8);
    graph.add_edge(1, 2, 4);
    
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 9);

    graph.add_edge(2, 3, 9);
    graph.add_edge(1, 3, 1);

    DirectedGraphState state(graph, 0, 3);

    //HeuristicSearch<DirectedGraphState> s(state);
    //BreadthFirstSearch<DirectedGraphState> s(state);
    //DepthFirstSearch<DirectedGraphState> s(state);
    UniformCostSearch<DirectedGraphState> s(state);

    //s.search(estimator);
    s.search();
    return 0;
}