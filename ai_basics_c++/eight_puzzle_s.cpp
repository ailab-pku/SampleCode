#include "problem/eight_puzzle.hpp"
#include "algorithm/breadth_first_search.hpp"
#include "algorithm/depth_first_search.hpp"

int main(){
    std::vector<int> board = {1,0,3,4,2,6,7,5,8};
    EightPuzzleState state(board);
    
    BreadthFirstSearch<EightPuzzleState> bfs(state);
    DepthFirstSearch<EightPuzzleState> dfs(state);

    bfs.search();
    
    dfs.search(false);
    return 0;
}