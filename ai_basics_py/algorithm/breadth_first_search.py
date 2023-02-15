from copy import deepcopy
from queue import Queue
from interface.state import StateBase

class BreadthFirstSearch:
    
    def __init__(self, state:StateBase):
        assert isinstance(state, StateBase)
        self.initial_state = deepcopy(state)
    
    def search(self) -> None:
        states_queue = Queue()
        explored_states = set()
        state = self.initial_state
        
        states_queue.put(state)
        explored_states.add(state)
        
        while not states_queue.empty():
            state = states_queue.get()
            
            if state.success():
                state.show()
                continue
            
            if state.fail():
                continue
            
            for action in state.action_space():
                new_state = state.next(action)
                
                if new_state not in explored_states:
                    states_queue.put(new_state)
                    explored_states.add(new_state)
                    