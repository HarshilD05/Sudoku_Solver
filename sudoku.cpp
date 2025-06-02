/*
THE // comments are for testing code 
*/


#include<iostream>
#include<vector>

class Sudoku {
    std::vector<std::vector<uint8_t>> board;
    std::vector<std::vector<uint8_t>> empty_boxes;
    bool solved;

    std::vector<std::vector<uint8_t>> empty_cells (std::vector<std::vector<uint8_t>> board);

public : 
    Sudoku(std::vector<std::vector<uint8_t>> b) {
        /*  Only accept 9x9 Sudoku Board    */
        if (b.size() == 9 && b[0].size() == 9 ) {
            board = b;
            empty_boxes = empty_cells(board);
            solved = false;
        }
        else std::cout<<"Invalid Sudoku Board..."<<std::endl<<"It Should be a 9x9 board"<<std::endl;
    }

    bool check_element (uint8_t row, uint8_t col, uint8_t val);

    void fill_board (uint8_t row, uint8_t col, uint8_t val) {
        this->board[row][col] = val;
    }

    void print_board () {
        /*  Itterate through Rows   */
        for (auto r : board) {
            std::cout<<"\n";
            for (uint8_t e : r) {
                e += '0';
                std::cout<< e <<" ";
            }
        }
        std::cout<<std::endl<<std::endl;
    }

    void BackTrackSolve (uint8_t itt = 0);
};

std::vector<std::vector<uint8_t>> Sudoku :: empty_cells (std::vector<std::vector<uint8_t>> board) {
    /* To be returned   */
    std::vector<std::vector<uint8_t>> empty;

    /*  Itterate through rows   */
    for (uint8_t i = 0;i<9;i++) {
        /*  Itterate Through Column */
        for (uint8_t j = 0;j<9;j++) {
            /*  Add coordinates of boxes with 0 */
            if (board[i][j] == 0) { 
                std::vector<uint8_t> e = {i,j}; 
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

bool Sudoku :: check_element (uint8_t row, uint8_t col, uint8_t val) {
    /*  Elements which are already used */
    std::vector<bool> done(10, false);

    /*  Checking rows   */
    // std::cout<<"Checking Rows..."<<std::endl;
    done[val] = true;

    for (uint8_t c = 0;c<9;c++) {
        /*  check only for non 0 values */
        if (this->board[row][c]) {

            // std::cout<<"Val : "<< this->board[row][c] <<std::endl;
            // std::cout<<"[ ";
            // for (uint8_t d : done) std::cout<< d <<" ";
            // std::cout<<"]"<<std::endl;

            /*  if value already done invalid input */
            if (done[ this->board[row][c] ]) return false;
            
            done[this->board[row][c] ] = true;
        }
    }
    /*  Clear done for next check   */
    for (uint8_t i = 0;i<10;++i) {
        done[i] = false;
    }

    /*   checking Columns   */
    // std::cout<<"Checking Columns..."<<std::endl;

    /*  Already adding value to be added to be considered for repetetion    */
    done[val] = true;

    for (uint8_t r = 0;r<9;r++) {
        /*  check only for non 0 values */
        if (this->board[r][col]) {

            // std::cout<<"Val : "<< this->board[r][col] <<std::endl;
            // std::cout<<"[ ";
            // for (uint8_t d : done) std::cout<< d <<" ";
            // std::cout<<"]"<<std::endl;

            /*  if value already done invalid input */
            if (done[ this->board[r][col] ]) return false;
            
            done[this->board[r][col] ] = true;
        }
    }
    /*  Clear done for next check   */
    for (uint8_t i = 0;i<10;++i) {
        done[i] = false;
    }

    /*   Checking Grid  */
    uint8_t r = 3 * (row/3);
    uint8_t c = 3 * (col/3);

    // std::cout<<"Checking Grid ("<< r <<","<< c <<") ..."<<std::endl;

    /*  Already adding val to be considered for repetetion  */
    done[val] = true;

    /*  Itterating through 3 rows   */
    for (uint8_t a = 0;a<3;a++) {
        /*  Ittearating through 3 columns   */
        for (uint8_t b = 0;b<3;b++) {

            // std::cout<<"Val : "<< this->board[r+a][c+b] <<std::endl;
            // std::cout<<"[ ";
            // for (uint8_t d : done) std::cout<< d <<" ";
            // std::cout<<"]"<<std::endl;

            /*  check only non 0 value   */
            if (this->board[r+a][c+b]) {
                /*  if value already done invalid input */
                if (done[ this->board[r+a][c+b] ] ) return false;
                
                done[ this->board[r+a][c+b] ] = true;
            }

        }
    }

    return true;
}

void Sudoku :: BackTrackSolve (uint8_t itt) {
    /*  Only perform itterations for all empty cells    */
    if (itt < empty_boxes.size() ) {
        /*  Getting Coordinates of empty cell   */
        uint8_t r = empty_boxes[itt][0];
        uint8_t c = empty_boxes[itt][1];

        /*  Trying all values between 1-9   */
        for (uint8_t x = 1;x<10;x++) {
            if (solved) return;

            // std::cout<<"Itt : "<< itt <<"\t R : "<< r <<"\t C : "<< c <<std::endl;    
            // std::cout<<"X : "<< x <<std::endl;

            /*  If element valid then fill box  */
            if ( check_element(r,c,x) ) {
                fill_board(r,c,x);

                // std::cout<<"\t Valid !"<<std::endl;
                // print_board();

                /*  Next Itteration */
                BackTrackSolve(itt+1);
            }
            // else std::cout<<"\t Invalid!"<<std::endl;

            /*  Undo Fill if board not solved   */
            if ( !solved ) this->board[r][c] = 0;
        }

        // if (!solved) {
        //     std::cout<<"Going Back...."<<std::endl;
        //     print_board();
        // }

    }
    /*  If all empty cells filled then its Solved   */
    else solved = true;
}


