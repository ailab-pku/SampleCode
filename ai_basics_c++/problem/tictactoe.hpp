#pragma once

#include <vector>
#include <cassert>
#include <iostream>
#include <cinttypes>

#include "../interface/game_state.hpp"

class TicTacToeState : public NPlayerGameStateBase<int, 2> {
private:

    // round 0: X
    static constexpr int X = 0b01, O = 0b10;
    
    int32_t _state;
    int round;

    inline int take_at(int index) const {
        return (_state >> (index << 1)) & 0b11;
    }

    inline void put_at(int index){
        int piece = active_player() == 0 ? X : O;
        assert(index >= 0 and index < 9 and take_at(index) == 0);
        _state |= (piece << (index << 1));
    }

    inline bool full() const {
        return round == 9;
    }

    bool x_wins() const {
        return test_lines_of(X);
    }

    bool o_wins() const {
        return test_lines_of(O);
    }

    bool test_lines_of(int piece) const {
        const static int row_mask = 0b000000000000111111, column_mask = 0b000011000011000011;
        const static int lr_mask = 0b110000001100000011, rl_mask = 0b000011001100110000;
        
        if (round < 5){
            return false;
        }

        int row_target = (piece|(piece<<2)|(piece<<4));
        int column_target = (piece|(piece<<6)|(piece<<12));
        int lr_target (piece|(piece<<8)|(piece<<16));
        int rl_target ((piece<<4)|(piece<<8)|(piece<<12));
        
        return ((_state & row_mask) == row_target
            or ((_state >> 6) & row_mask) == row_target
            or ((_state >> 12) & row_mask) == row_target
            or (_state & column_mask) == column_target
            or ((_state >> 2) & column_mask) == column_target
            or ((_state >> 4) & column_mask) == column_target
            or (_state & lr_mask) == lr_target
            or (_state & rl_mask) == rl_target
        );
    }
public:
    
    TicTacToeState() : _state(0), round(0) {}

    int active_player() const override {
        return round & 1;
    }
    
    std::vector<int> action_space() const override {
        std::vector<int> actions;
        for (int i = 0; i < 9; ++ i){
            if (take_at(i) == 0){
                actions.push_back(i);
            }
        }
        return actions;
    }

    std::vector<double> outcomes() const override {
        static const std::vector<double> scores_x_wins {1, -1};
        static const std::vector<double> scores_o_wins {-1, 1};
        static const std::vector<double> scores_tie {0, 0};
        if (x_wins()){
            return scores_x_wins;
        } else if (o_wins()){
            return scores_o_wins;
        }
        return scores_tie;
    }

    bool done() const override {
        return full() or x_wins() or o_wins();
    }

    void show() const override {
        static const char pieces[] = "_XO";
        for (int i = 0; i < 3; ++ i){
            for (int j = 0; j < 3; ++ j){
                std::cout << pieces[take_at(i * 3 + j)];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    const TicTacToeState& next(const int& action) const override {
        static TicTacToeState next_state;
        next_state = *this;
        next_state.put_at(action);
        ++ next_state.round;
        return next_state;
    }

    friend struct std::hash<TicTacToeState>;

    friend bool operator== (const TicTacToeState& s1, const TicTacToeState& s2){
        return s1._state == s2._state;
    }
};

template<>
struct std::hash<TicTacToeState>{
    size_t operator()(const TicTacToeState& s) const {
        return s._state;
    }
};