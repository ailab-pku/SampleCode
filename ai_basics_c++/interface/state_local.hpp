#pragma once

#include <vector>

#include "state.hpp"

class StateLocalBase{
public:
    
    StateLocalBase() = default;
    virtual ~StateLocalBase() = default;

    virtual int neighbor_count() const = 0;
    virtual const StateLocalBase& neighbor(int neighbor_index) const = 0;
    virtual void show() const = 0;
};

template<typename StateType>
class StateLocalWrapper : public StateLocalBase{
private:
    
    StateType _state;
    typedef typename StateType::ActionBaseType ActionType;
    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

public:

    StateLocalWrapper() = default;
    StateLocalWrapper(const StateType& state) : _state(state) {}
    
    const StateType& state() const {return _state;}

    int neighbor_count() const override {
        return _state.action_space().size();
    }

    const StateLocalWrapper& neighbor(int neighbor_index) const override {
        static StateLocalWrapper next_state_local;
        next_state_local._state = _state.next(_state.action_space()[neighbor_index]);
        return next_state_local;
    }

    void show() const override {
        _state.show();
    }
};

