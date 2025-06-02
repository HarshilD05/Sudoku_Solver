/*
THE // comments are for testing code 
*/


#include<iostream>
#include<vector>

class Sudoku {
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> empty_boxes;
    bool solved;

    std::vector<std::vector<int>> empty_cells (std::vector<std::vector<int>> board);

public : 
    Sudoku(std::vector<std::vector<int>> b) {
        /*  Only accept 9x9 Sudoku Board    */
        if (b.size() == 9 && b[0].size() == 9 ) {
            board = b;
            empty_boxes = empty_cells(board);
            solved = false;
        }
        else std::cout<<"Invalid Sudoku Board..."<<std::endl<<"It Should be a 9x9 board"<<std::endl;
    }

    bool check_element (std::vector<std::vector<int>> board,int row,int col,int val);

    void fill_board (int row,int col,int val) {
        board[row][col] = val;
    }

    void print_board () {
        /*  Itterate through Rows   */
        for (auto r : board) {
            std::cout<<"\n";
            for (int e : r) {
                std::cout<< e <<" ";
            }
        }
        std::cout<<std::endl<<std::endl;
    }

    void BackTrackSolve (int itt = 0);
};

std::vector<std::vector<int>> Sudoku :: empty_cells (std::vector<std::vector<int>> board) {
    /* To be returned   */
    std::vector<std::vector<int>> empty;

    /*  Itterate through rows   */
    for (int i = 0;i<9;i++) {
        /*  Itterate Through Column */
        for (int j = 0;j<9;j++) {
            /*  Add coordinates of boxes with 0 */
            if (board[i][j] == 0) { 
                std::vector<int> e = {i,j};
                empty.push_back(e);
            }
        }
    }

    /* Printing empty cells */
    // for (auto p : empty) {
    //     std::cout<<"["<< p[0] <<","<< p[1] <<"] ";
    // }
    // std::cout<<std::endl;

    return empty;
}

bool Sudoku :: check_element (std::vector<std::vector<int>> board,int row,int col,int val) {
    /*  Elements which are already used */
    std::vector<bool> done(10, false);

    /*  Checking rows   */
    // std::cout<<"Checking Rows..."<<std::endl;
    done[val] = true;

    for (int c = 0;c<9;c++) {
        /*  check only for non 0 values */
        if (board[row][c]) {

            // std::cout<<"Val : "<< board[row][c] <<std::endl;
            // std::cout<<"[ ";
            // for (int d : done) std::cout<< d <<" ";
            // std::cout<<"]"<<std::endl;

            /*  if value already done invalid input */
            if (done[ board[row][c] ]) return false;
            
            done[board[row][c] ] = true;
        }
    }
    /*  Clear done for next check   */
    for (int i = 0;i<10;++i) {
        done[i] = false;
    }

    /*   checking Columns   */
    // std::cout<<"Checking Columns..."<<std::endl;

    /*  Already adding value to be added to be considered for repetetion    */
    done[val] = true;

    for (int r = 0;r<9;r++) {
        /*  check only for non 0 values */
        if (board[r][col]) {

            // std::cout<<"Val : "<< board[r][col] <<std::endl;
            // std::cout<<"[ ";
            // for (int d : done) std::cout<< d <<" ";
            // std::cout<<"]"<<std::endl;

            /*  if value already done invalid input */
            if (done[ board[r][col] ]) return false;
            
            done[board[r][col] ] = true;
        }
    }
    /*  Clear done for next check   */
    for (int i = 0;i<10;++i) {
        done[i] = false;
    }

    /*   Checking Grid  */
    int r = 3 * (row/3);
    int c = 3 * (col/3);

    // std::cout<<"Checking Grid ("<< r <<","<< c <<") ..."<<std::endl;

    /*  Already adding val to be considered for repetetion  */
    done[val] = true;

    /*  Itterating through 3 rows   */
    for (int a = 0;a<3;a++) {
        /*  Ittearating through 3 columns   */
        for (int b = 0;b<3;b++) {

            // std::cout<<"Val : "<< board[r+a][c+b] <<std::endl;
            // std::cout<<"[ ";
            // for (int d : done) std::cout<< d <<" ";
            // std::cout<<"]"<<std::endl;

            /*  check only non 0 value   */
            if (board[r+a][c+b]) {
                /*  if value already done invalid input */
                if (done[ board[r+a][c+b] ] ) return false;
                
                done[ board[r+a][c+b] ] = true;
            }

        }
    }

    return true;
}

void Sudoku :: BackTrackSolve (int itt) {
    /*  Only perform itterations for all empty cells    */
    if (itt < empty_boxes.size() ) {
        /*  Getting Coordinates of empty cell   */
        int r = empty_boxes[itt][0];
        int c = empty_boxes[itt][1];

        /*  Trying all values between 1-9   */
        for (int x = 1;x<10;x++) {
            if (solved) return;

            // std::cout<<"Itt : "<< itt <<"\t R : "<< r <<"\t C : "<< c <<std::endl;    
            // std::cout<<"X : "<< x <<std::endl;

            /*  If element valid then fill box  */
            if ( check_element(board,r,c,x) ) {
                fill_board(r,c,x);

                // std::cout<<"\t Valid !"<<std::endl;
                // print_board();

                /*  Next Itteration */
                BackTrackSolve(itt+1);
            }
            // else std::cout<<"\t Invalid!"<<std::endl;

            /*  Undo Fill if board not solved   */
            if ( !solved ) board[r][c] = 0;
        }

        // if (!solved) {
        //     std::cout<<"Going Back...."<<std::endl;
        //     print_board();
        // }

    }
    /*  If all empty cells filled then its Solved   */
    else solved = true;
}


