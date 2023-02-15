from abc import ABCMeta, abstractmethod

class StateBase(metaclass=ABCMeta):
    
    @abstractmethod
    def action_space(self) -> list:
        raise NotImplementedError
    
    @abstractmethod
    def cost(self) -> float:
        raise NotImplementedError
    
    @abstractmethod
    def cumulative_cost(self) -> float:
        raise NotImplementedError
    
    @abstractmethod
    def success(self) -> bool:
        raise NotImplementedError
    
    @abstractmethod
    def fail(self) -> bool:
        raise NotImplementedError
    
    @abstractmethod
    def show(self) -> None:
        raise NotImplementedError
    
    @abstractmethod
    def next(self, action) -> "StateBase":
        raise NotImplementedError
    
    @abstractmethod
    def __eq__(self, state:"StateBase") -> bool:
        raise NotImplementedError
    
    @abstractmethod
    def __hash__(self) -> int:
        raise NotImplementedError
    
    @abstractmethod
    def __lt__(self, state) -> bool:
        raise NotImplementedError
    