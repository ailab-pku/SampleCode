from copy import deepcopy
from queue import LifoQueue
from interface.state import StateBase

class DepthFirstSearch:
    
    def __init__(self, state:StateBase):
        assert isinstance(state, StateBase)
        self.initial_state = deepcopy(state)
    
    def search(self) -> None:
        states_stack = LifoQueue()
        state = self.initial_state
        
        states_stack.put((state, 0))
        
        while not states_stack.empty():
            state, action_id = states_stack.get()
            
            if state.success():
                state.show()
                continue
            
            if state.fail():
                continue
            
            if action_id < len(state.action_space()):
                states_stack.put((state, action_id+1))
                new_state = state.next(state.action_space()[action_id])
                states_stack.put((new_state, 0))