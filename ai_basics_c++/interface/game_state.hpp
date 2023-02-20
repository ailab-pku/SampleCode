#pragma once

#include <vector>

template<typename ActionType>
class GameStateBase{
public:
    
    GameStateBase() = default;
    virtual ~GameStateBase() = default;

    typedef ActionType ActionBaseType;

    virtual int n_players() const = 0;
    
    virtual int active_player() const = 0;

    // action space of active player
    virtual std::vector<ActionType> action_space() const = 0;
    
    // outcome of each player when the game is over (done() == true)
    virtual std::vector<double> outcomes() const = 0;
    
    virtual bool done() const = 0;

    // print state info
    virtual void show() const = 0;

    // next state through action
    virtual const GameStateBase& next(const ActionType&) const = 0;

    // state hasher
    friend struct std::hash<GameStateBase>;
    
    // default comparators

    // operator== shall be redefined by derived classes.
    friend bool operator== (const GameStateBase& s1, const GameStateBase& s2){
        return s1.outcomes()[s1.active_player()] == s2.outcomes()[s2.active_player()];
    }
};

template<typename ActionType, int n>
class NPlayerGameStateBase : public GameStateBase<ActionType>{
protected:
    static constexpr int _n_players = n;
public:
    NPlayerGameStateBase() = default;
    virtual ~NPlayerGameStateBase() = default;

    int n_players() const override {return _n_players;}    
};

#include "state.hpp"

template<typename StateType>
class GameStateWrapper : public NPlayerGameStateBase<typename StateType::ActionBaseType, 1> {
private:

    StateType _state;
    typedef typename StateType::ActionBaseType ActionType;
    static_assert(std::is_base_of<StateBase<ActionType>, StateType>::value, "StateType not derived from StateBase.");

public:

    GameStateWrapper() = default;
    GameStateWrapper(const StateType& state) : _state(state) {}

    int active_player() const override {return 0;}
    
    std::vector<ActionType> action_space() const override {
        return _state.action_space();
    }
    
    std::vector<double> outcomes() const override {
        static std::vector<double> outcomes;
        double outcome = done() ? -_state.cumulative_cost() : 0;
        outcomes.clear();
        outcomes.push_back(outcome);
        return outcomes;
    }
    
    bool done() const override {
        return _state.success() or _state.fail();
    }
    
    void show() const override {
        _state.show();
    }
    
    const GameStateWrapper& next(const ActionType& action) const override {
        static GameStateWrapper next_state;
        next_state._state = _state.next(action);
        return next_state;
    }

    friend struct std::hash<GameStateWrapper>;

    friend bool operator== (const GameStateWrapper& s1, const GameStateWrapper& s2){
        return s1._state == s2._state;
    }
    
    friend bool operator!= (const GameStateWrapper& s1, const GameStateWrapper& s2){
        return s1._state != s2._state;
    }

    friend bool operator>= (const GameStateWrapper& s1, const GameStateWrapper& s2){
        return s1._state >= s2._state;
    }

    friend bool operator<= (const GameStateWrapper& s1, const GameStateWrapper& s2){
        return s1._state <= s2._state;
    }

    friend bool operator> (const GameStateWrapper& s1, const GameStateWrapper& s2){
        return s1._state > s2._state;
    }

    friend bool operator< (const GameStateWrapper& s1, const GameStateWrapper& s2){
        return s1._state < s2._state;
    }
};

template<typename ActionType>
struct std::hash<GameStateWrapper<ActionType> >{
    size_t operator()(const GameStateWrapper<ActionType>& s) const {
        std::hash<decltype(s._state)> hasher;
        return hasher(s._state);
    }
};