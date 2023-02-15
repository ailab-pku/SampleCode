from copy import deepcopy
from queue import PriorityQueue, LifoQueue
from typing import Callable
from interface.state import StateBase

class HeuristicSearch:
    
    ValueEstimatorType = Callable[[StateBase], float]
    
    def __init__(self, state:StateBase):
        assert isinstance(state, StateBase)
        self.initial_state = deepcopy(state)
    
    def search(self, value_of:ValueEstimatorType) -> None:
        states_queue = PriorityQueue()
        best_value_of = dict()
        last_state_of = dict()
        state = self.initial_state
        
        states_queue.put((0, state))
        best_value_of[state] = 0
        
        state.show()
        
        while not states_queue.empty():
            _, state = states_queue.get()
            
            if state.success():
                break
            
            if state.fail():
                continue
            
            for action in state.action_space():
                new_state = state.next(action)
                
                if (new_state not in best_value_of 
                    or value_of(new_state) > best_value_of[new_state]):
                    
                    best_value_of[new_state] = value_of(new_state)
                    states_queue.put((-value_of(new_state), new_state))
                    last_state_of[new_state] = state
        
        if state.success():
            path = LifoQueue()
            
            while state in last_state_of:
                path.put(state)
                state = last_state_of[state]
            
            while not path.empty():
                state = path.get()
                state.show()