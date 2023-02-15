from typing import List
from copy import deepcopy

from interface.state import StateBase

class DirectedGraph:
    
    def __init__(self, n_nodes:int):
        self.n_nodes : List[int] = n_nodes
        self.edge_end : List[int] = list()
        self.weight : List[float] = list()
        self.first : List[int] = [None] * n_nodes
        self.next : List[int] = list()
    
    def add_edge(self, start_node:int, end_node:int, weight:float) -> None:
        self.edge_end.append(end_node)
        self.weight.append(weight)
        self.next.append(self.first[start_node])
        self.first[start_node] = len(self.edge_end) - 1
    
    def edge_indexes_starting_from(self, node:int) -> List[int]:
        edge_indexes:List[int] = list()
        
        index = self.first[node]
        while index is not None:
            edge_indexes.append(index)
            index = self.next[index]
        
        return edge_indexes
    

class DirectedGraphState(StateBase):
    
    def __init__(self, graph:DirectedGraph, current_node:int, target_node:int):
        self.graph = graph
        self.current_node = current_node
        self.target_node = target_node
        self.last_edge_index = None
        self._cumulative_cost:float = 0
    
    def cost(self) -> float:
        return 0 if self.last_edge_index is None else self.graph.weight[self.last_edge_index]

    def cumulative_cost(self) -> float:
        return self._cumulative_cost
    
    def action_space(self) -> List[int]:
        return self.graph.edge_indexes_starting_from(self.current_node)
    
    def success(self) -> bool:
        return self.current_node == self.target_node
    
    def fail(self) -> bool:
        return not self.success() and self.graph.first[self.current_node] is None
    
    def next(self, action:int) -> "DirectedGraphState":
        next_state = DirectedGraphState(self.graph, self.graph.edge_end[action], self.target_node)
        next_state.last_edge_index = action
        next_state._cumulative_cost = self._cumulative_cost + self.graph.weight[action]
        return next_state
    
    def show(self) -> None:
        print(f"At: {self.current_node}, from edge {self.last_edge_index}, distance: {self._cumulative_cost}")
        
    def __hash__(self) -> int:
        return self.current_node
    
    def __eq__(self, other:"DirectedGraphState") -> bool:
        return (self.current_node == other.current_node
            and self.target_node == other.target_node
            and self.graph == other.graph)
    
    def __lt__(self, state:"DirectedGraphState") -> bool:
        return self._cumulative_cost > state._cumulative_cost