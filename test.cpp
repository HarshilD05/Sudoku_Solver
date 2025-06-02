#include <iostream>
#include "sudoku.cpp"



int main () {
    std::vector<std::vector<uint8_t>> board = 
    {
        {2,0,9,  0,0,0,  6,0,0},
        {0,4,0,  8,7,0,  0,1,2},
        {8,0,0,  0,1,9,  0,4,0},

        {0,3,0,  7,0,0,  8,0,1},
        {0,6,5,  0,0,8,  0,3,0},
        {1,0,0,  0,3,0,  0,0,7},

        {0,0,0,  6,5,0,  7,0,9},
        {6,0,4,  0,0,0,  0,2,0},
        {0,8,0,  3,0,1,  4,5,0}
    };

    Sudoku s1 (board);
    
    std::cout<<"Unsolved Board : "<<std::endl;
    s1.print_board();

    s1.BackTrackSolve();

    std::cout<<"\nSolved Board : "<<std::endl;
    s1.print_board();

    return 0;
}