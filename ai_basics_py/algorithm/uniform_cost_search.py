from interface.state import StateBase
from .heuristic_search import HeuristicSearch

class UniformCostSearch:
    
    def __init__(self, state:StateBase):
        self._heuristic_search = HeuristicSearch(state)
    
    def search(self) -> None:
        self._heuristic_search.search(lambda state: -state.cumulative_cost())