#pragma once

#include <vector>

// state hasher is required
// Example:
/*
template <> 
struct std::hash<StateType> {
    size_t operator()(const StateType& x) const {
        return 0;
    }
};
*/

template<typename ActionType>
class StateBase{
public:
    
    StateBase() = default;
    virtual ~StateBase() = default;

    typedef ActionType ActionBaseType;

    virtual std::vector<ActionType> action_space() const = 0;
    
    virtual double cost() const = 0;
    
    virtual double cumulative_cost() const = 0;
    
    // if reach the goal
    virtual bool success() const = 0;

    // if fail the mission
    virtual bool fail() const = 0;

    // print state info
    virtual void show() const = 0;

    // next state through action
    virtual const StateBase& next(const ActionType&) const = 0;

    // state hasher
    friend struct std::hash<StateBase>;
    
    // default comparators

    // operator== shall be redefined by derived classes.
    friend bool operator== (const StateBase& s1, const StateBase& s2){
        return s1.cumulative_cost() == s2.cumulative_cost();
    }
};
