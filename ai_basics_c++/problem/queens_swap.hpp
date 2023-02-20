#pragma once

#include <cassert>
#include <vector>
#include <iostream>
#include <algorithm>

#include "queens.hpp"
#include "../interface/state.hpp"

typedef std::pair<int, int> QueensSwapAction;

class QueensSwapState : public StateBase<QueensSwapAction>{
private:
    int _n_queens;
    std::vector<int> _queens;
    int _conflicts;
    int _steps;

    void update_conflicts(){
        _conflicts = 0;
        for (int i = 0, xi, yi; i < _n_queens; ++ i){
            xi = i;
            yi = _queens[i];
            for (int j = i+1, xj, yj; j < _n_queens; ++ j){
                xj = j;
                yj = _queens[j];
                _conflicts += int(xi+yi == xj+yj or xi-yi == xj-yj);
            }
        }
    }

public:
    
    QueensSwapState(int n) : _n_queens(n), _queens(n), _steps(0) {
        for (int i = 0; i < _n_queens; ++ i){
            _queens[i] = i;
        }
        update_conflicts();
    }

    QueensSwapState() = default;

    int n_queens() const {return _n_queens;}
    std::vector<int> queens() const {return _queens;}
    int conflicts() const {return _conflicts;}

    double cost() const override {
        return success() ? 0 : 1;
    }

    double cumulative_cost() const override {
        return _steps;
    }

    bool success() const override {
        return _conflicts == 0;
    }

    bool fail() const override {
        return false;
    }

    void show() const override {
        for (int r = 0; r < _n_queens; ++ r){            
            for (int c = 0; c < _n_queens; ++ c){
                
                if (_queens[r] == c){
                    std::cout << "Q";
                } else {
                    std::cout << "+";
                }
            }
            std::cout << std::endl;
        }
        for (int r = 0; r < _n_queens; ++ r){
            std::cout << _queens[r] << ' ';
        }
        std::cout << std::endl << std::endl;
    }

    std::vector<QueensSwapAction> action_space() const override {
        static std::vector<QueensSwapAction> actions;

        if (actions.size() == 0){
            for (int i = 0; i < _n_queens; ++ i){
                for (int j = i+1; j < _n_queens; ++ j){
                    actions.push_back(std::make_pair(i, j));
                }
            }
        }

        return actions;
    }

    const QueensSwapState& next(const QueensSwapAction& action) const override {
        static QueensSwapState next_state;
        next_state = *this;

        std::swap(next_state._queens[action.first], next_state._queens[action.second]);
        next_state.update_conflicts();

        return next_state;
    }

    friend struct std::hash<QueensSwapState>;
    
    friend bool operator== (const QueensSwapState& s1, const QueensSwapState& s2){
        return s1._queens == s2._queens and s1._n_queens == s2._n_queens;
    }
};

template <> 
struct std::hash<QueensSwapState> {
    
    // Cantor Expansion
    size_t operator() (const QueensSwapState& s) const {
        
        static std::vector<size_t> factorial = {1};
        size_t sum = 0;
        
        if (factorial.size() < s._n_queens){
            for (size_t i = factorial.size(); i <= s._n_queens; ++ i){
                factorial.push_back(factorial.back() * i);
            }
        }

        for (int i = 0, reverse; i < s._n_queens; ++ i){
            
            reverse = 0;
            
            for (int j = i+1; j < s._n_queens; ++ j){
                if (s._queens[j] < s._queens[i]){
                    ++ reverse;
                }
            }

            sum += size_t(reverse) * factorial[s._n_queens-i-1];
        }
        return sum;
    }
};
