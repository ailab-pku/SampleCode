#include "interface/game_state.hpp"

#include "algorithm/full_game_search.hpp"
#include "problem/queens.hpp"
#include "problem/tictactoe.hpp"

int main(){
    /*QueensState state(4);
    GameStateWrapper<QueensState> wrapper(state);
    state.next(1);
    */

    TicTacToeState t;
    /*
    t = t.next(1);
    t.show();
    t = t.next(2);
    t.show();
    t = t.next(4);
    t.show();
    t = t.next(3);
    t.show();
    t = t.next(6);
    t.show();
    
    t = t.next(0);
    t.show();
    t = t.next(8);
    t.show();
    t = t.next(7);
    t.show();

    t = t.next(5);
    t.show();
    std::cout << t.done() << std::endl;*/

    FullGameSearch<TicTacToeState> fgs(t);

    fgs.search();
    
    return 0;
}