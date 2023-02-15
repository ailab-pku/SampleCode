from problem.directed_graph import *
from algorithm.breadth_first_search import *
from algorithm.depth_first_search import *
from algorithm.uniform_cost_search import *

if __name__ == "__main__":
    graph = DirectedGraph(6)

    graph.add_edge(0, 1, 1)
    graph.add_edge(1, 2, 1)
    graph.add_edge(2, 3, 1)
    graph.add_edge(3, 4, 1)
    graph.add_edge(4, 5, 1)
    graph.add_edge(5, 0, 1)
    graph.add_edge(0, 2, 1)
    graph.add_edge(0, 3, 6)
    
    state = DirectedGraphState(graph, 0, 3)
    
    bfs = BreadthFirstSearch(state)
    bfs.search()
    
    print()
    
    dfs = DepthFirstSearch(state)
    dfs.search()
    
    print()
    
    ucs = UniformCostSearch(state)
    ucs.search()
    