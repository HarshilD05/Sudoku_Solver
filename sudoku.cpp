#include<iostream>
#include<vector>
#include<array>

struct CellState {
    uint16_t state; // 0b CCCC 000V VVVV VVVV, C = count (4 bits), V = value flags (9 bits)

    CellState() : state(0x91FF) {} // All 9 possible, count = 9

    void set_possible(uint8_t val, bool possible) {
        if (val < 1 || val > 9) return;
        uint16_t mask = 1 << (val - 1);

        bool was_possible = (state & mask);

        if (possible && !was_possible) {
            state |= mask;
            set_count(get_count() + 1);
        } else if (!possible && was_possible) {
            state &= ~mask;
            set_count(get_count() - 1);
        }
    }

    bool is_possible(uint8_t val) const {
        if (val < 1 || val > 9) return false;
        return (state & (1 << (val - 1)));
    }

    uint8_t get_count() const { return (state >> 12); }

    void set_count(uint8_t cnt) {
        state = (state & 0x0FFF) | ((cnt & 0xF) << 12);
    }

    uint8_t get_single_value() const {
        if (get_count() != 1) return 0;
        for (uint8_t v = 1; v <= 9; ++v)
            if (is_possible(v)) return v;
        return 0;
    }
};

typedef std::vector<std::vector<CellState>> BoardState;
typedef std::vector<std::vector<uint8_t>> Mat9x9;

struct HiddenSingles_Data {
    int8_t val, row, col;

    HiddenSingles_Data(): val(0), row(-1), col(-1) {}
    HiddenSingles_Data(int8_t value, int8_t r, int8_t c): val(value), row(r), col(c) {} 
};

class Sudoku {
    // Basic Requirements
    Mat9x9 board;
    bool solved;

    // BackTrackSolver Requirements
    std::vector<std::vector<uint8_t>> empty_boxes;
    std::vector<std::vector<uint8_t>> empty_cells (Mat9x9 board);
        
    // Requirements For State Based Solver
    BoardState boardState;
    void removeRowPossibility(uint8_t val, uint8_t row); 
    void removeColPossibility(uint8_t val, uint8_t col);
    void removeGridPossibilty(uint8_t val, uint8_t gridNo);
    void removeAdjPossibilty(uint8_t val, uint8_t row, uint8_t col);
    void initialiseBoardState();
    std::pair<int8_t, int8_t> ApplyNakedSingles();
    HiddenSingles_Data ApplyHiddenSingles ();
    


public : 
    Sudoku(Mat9x9 b) {
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

    void print_board ();

    void BackTrackSolve (uint8_t itt = 0);

    void StateBasedSolve();
};

void Sudoku :: print_board () {
    /*  Print the Sudoku Board   */
    for (uint8_t i = 0;i<9;i++) {
        for (uint8_t j = 0;j<9;j++) {
            std::cout<<static_cast<int>(board[i][j])<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}


/* Requirements for the BackTrack Solver */
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


/* Requirements for the State Based Solver  */
void Sudoku :: removeRowPossibility(uint8_t val, uint8_t row) {
    for (uint8_t col = 0; col < 9; ++col) {
        if (boardState[row][col].is_possible(val)) {
            boardState[row][col].set_possible(val, false);
        }
    }
}

void Sudoku :: removeColPossibility(uint8_t val, uint8_t col) {
    for (uint8_t row = 0; row < 9; ++row) {
        if (boardState[row][col].is_possible(val)) {
            boardState[row][col].set_possible(val, false);
        }
    }
}

void Sudoku :: removeGridPossibilty(uint8_t val, uint8_t gridNo) {
    uint8_t startRow = 3 * (gridNo / 3);
    uint8_t startCol = 3 * (gridNo % 3);

    for (uint8_t r = 0; r < 3; ++r) {
        for (uint8_t c = 0; c < 3; ++c) {
            if (boardState[startRow + r][startCol + c].is_possible(val)) {
                boardState[startRow + r][startCol + c].set_possible(val, false);
            }
        }
    }
}

void Sudoku :: removeAdjPossibilty(uint8_t val, uint8_t row, uint8_t col) {
    removeRowPossibility(val, row);
    removeColPossibility(val, col);
    uint8_t gridNo = (row / 3) * 3 + (col / 3);
    removeGridPossibilty(val, gridNo);
}

void Sudoku :: initialiseBoardState() {
    // Initialize state space from current board
    boardState.resize(9, std::vector<CellState>(9));
        
    // Fill Existing Board Cells
    for (uint8_t r = 0; r < 9; ++r) {
        for (uint8_t c = 0; c < 9; ++c) {
            if (board[r][c]) {
                // Remove the Possibilty of this Value Elsewhere in Row Column and Grid
                removeAdjPossibilty(board[r][c], r, c);

                // Set Possibility of Board Value
                boardState[r][c].state = 0;
                boardState[r][c].set_possible(board[r][c], true);
                boardState[r][c].set_count(1);
            }
        }
    }
}

std::pair<int8_t, int8_t> Sudoku :: ApplyNakedSingles() {
    // Naked Singles
    for (uint8_t r = 0; r < 9; ++r) {
        for (uint8_t c = 0; c < 9; ++c) {
            if (board[r][c] == 0 && boardState[r][c].get_count() == 1) {
                return {r, c};
            }
        }
    }

    return {-1, -1};
}

HiddenSingles_Data Sudoku :: ApplyHiddenSingles () {
    // Hidden Singles for Row and Column
    std::vector<int8_t> rowPossibiltyCount(9);
    std::vector<int8_t> colPossibiltyCount(9);

    for (int8_t i = 0;i < 9;i++) {
        
        // Reset for each Row and Col
        for (int8_t k = 0; k < 9; ++k) {
            rowPossibiltyCount[k] = 0;
            colPossibiltyCount[k] = 0;
        }

        // Row / Col Itteration
        for (int8_t j = 0;j < 9;j++) {
            // Only check if Row Block Not Filled
            if (boardState[i][j].get_count() != 1) {
                // Value Itteration
                for (int8_t val = 1; val <= 9; ++val) {
                    // Check if Value is Possible in Row
                    if (boardState[i][j].is_possible(val)) {
                        rowPossibiltyCount[val - 1]++;
                    }
                }
            }

            // Only check if Column Block Not Filled
            if (boardState[j][i].get_count() != 1) {
                // Value Itteration
                for (int8_t val = 1; val <= 9; ++val) {
                    // Check if Value is Possible in Column
                    if (boardState[j][i].is_possible(val)) {
                        colPossibiltyCount[val - 1]++;
                    }
                }
            }

        }

        // Check if Only a Single Value Count Found
        for (int8_t val = 1; val <= 9; ++val) {
            if (rowPossibiltyCount[val - 1] == 1) {
                // Find the Column where this Value is Possible
                for (int8_t j = 0; j < 9; ++j) {
                    if (boardState[i][j].is_possible(val)) {
                        return HiddenSingles_Data(val, i, j);
                    }
                }
            }
            if (colPossibiltyCount[val - 1] == 1) {
                // Find the Row where this Value is Possible
                for (int8_t j = 0; j < 9; ++j) {
                    if (boardState[j][i].is_possible(val)) {
                        return HiddenSingles_Data(val, j, i);
                    }
                }
            }
        }

    }


    // Hidden Singles for Grids
    std::vector<int8_t> gridPossibiltyCount(9);

    for (int8_t gridNo = 0; gridNo < 9; ++gridNo) {
        // Reset for each Grid
        for (int8_t k = 0; k < 9; ++k) {
            gridPossibiltyCount[k] = 0;
        }

        // Grid Itteration
        uint8_t startRow = 3 * (gridNo / 3);
        uint8_t startCol = 3 * (gridNo % 3);

        for (int8_t r = 0; r < 3; ++r) {
            for (int8_t c = 0; c < 3; ++c) {
                // Value Itteration
                for (int8_t val = 1; val <= 9; ++val) {
                    if (boardState[startRow + r][startCol + c].is_possible(val)) {
                        gridPossibiltyCount[val - 1]++;
                    }
                }
            }
        }

        // Check if Only a Single Value Count Found
        for (int8_t val = 1; val <= 9; ++val) {
            if (gridPossibiltyCount[val - 1] == 1) {
                // Find the Cell where this Value is Possible
                for (int8_t r = 0; r < 3; ++r) {
                    for (int8_t c = 0; c < 3; ++c) {
                        if (boardState[startRow + r][startCol + c].is_possible(val)) {
                            return HiddenSingles_Data(val, startRow + r, startCol + c);
                        }
                    }
                }
            }
        }

    }


    // If no Hidden Singles found
    return HiddenSingles_Data();
}



void Sudoku :: StateBasedSolve () {
    // Initialize the board state
    initialiseBoardState();

    // Loop until no more Naked Singles or Hidden Singles can be found
    bool progress = true;
    uint8_t r,c,val;


    while (progress) {
        progress = false;

        // Apply Naked Singles
        auto nakedSingle = ApplyNakedSingles();
        if (nakedSingle.first != -1) {
            r = nakedSingle.first;
            c = nakedSingle.second;
            val = boardState[r][c].get_single_value();

            // Fill the board and update state
            fill_board(r, c, val);
            boardState[r][c].set_possible(val, true);
            boardState[r][c].set_count(1);
            removeAdjPossibilty(val, r, c);
            progress = true;
            continue; // Restart the loop to recheck the board state
        }

        // Apply Hidden Singles
        HiddenSingles_Data hiddenSingle = ApplyHiddenSingles();
        if (hiddenSingle.row != -1) {
            r = hiddenSingle.row;
            c = hiddenSingle.col;
            val = hiddenSingle.val;

            // Fill the board and update state
            fill_board(r, c, val);
            boardState[r][c].set_possible(val, true);
            boardState[r][c].set_count(1);
            removeAdjPossibilty(val, r, c);
            progress = true;
            continue;
        }

        
    }

}