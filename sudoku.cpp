/*
THE // comments are for testing code 
*/


#include<iostream>
#include<vector>

using namespace std;

class Sudoku {
    vector<vector<int>> board;
    vector<vector<int>> empty_boxes;
    bool solved;

    vector<vector<int>> empty_cells (vector<vector<int>> board);

    public : 
    Sudoku(vector<vector<int>> b) {
        /*  Only accept 9x9 Sudoku Board    */
        if (b.size() == 9 && b[0].size() == 9 ) {
            board = b;
            empty_boxes = empty_cells(board);
            solved = false;
        }
        else cout<<"Invalid Sudoku Board..."<<endl<<"It Should be a 9x9 board"<<endl;
    }

    bool check_element (vector<vector<int>> board,int row,int col,int val);

    void fill_board (int row,int col,int val) {
        board[row][col] = val;
    }

    void print_board () {
        /*  Itterate through Rows   */
        for (auto r : board) {
            cout<<"\n";
            for (int e : r) {
                cout<< e <<" ";
            }
        }
        cout<<endl<<endl;
    }

    void solve (int itt = 0);
};


int main () {
    vector<vector<int>> board = 
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
    
    cout<<"Unsolved Board : "<<endl;
    s1.print_board();

    s1.solve();

    cout<<"\nSolved Board : "<<endl;
    s1.print_board();

    return 0;
}

vector<vector<int>> Sudoku :: empty_cells (vector<vector<int>> board) {
    /* To be returned   */
    vector<vector<int>> empty;

    /*  Itterate through rows   */
    for (int i = 0;i<9;i++) {
        /*  Itterate Through Column */
        for (int j = 0;j<9;j++) {
            /*  Add coordinates of boxes with 0 */
            if (board[i][j] == 0) { 
                vector<int> e = {i,j};
                empty.push_back(e);
            }
        }
    }

    /* Printing empty cells */
    // for (auto p : empty) {
    //     cout<<"["<< p[0] <<","<< p[1] <<"] ";
    // }
    // cout<<endl;

    return empty;
}

bool Sudoku :: check_element (vector<vector<int>> board,int row,int col,int val) {
    /*  Elements which are already used */
    vector<int> done;

    /*  Checking rows   */
    // cout<<"Checking Rows..."<<endl;
    done.push_back(val);

    for (int c = 0;c<9;c++) {
        /*  check only for non 0 values */
        if (board[row][c]) {

            // cout<<"Val : "<< board[row][c] <<endl;
            // cout<<"[ ";
            // for (int d : done) cout<< d <<" ";
            // cout<<"]"<<endl;

            /*  if value already done invalid input */
            for (int d : done) {
                if (d == board[row][c]) return false;
            }
            done.push_back(board[row][c]);
        }
    }
    /*  Clear done for next check   */
    done.clear();

    /*   checking Columns   */
    // cout<<"Checking Columns..."<<endl;

    /*  Already adding value to be added to be considered for repetetion    */
    done.push_back(val);

    for (int r = 0;r<9;r++) {
        /*  check only for non 0 values */
        if (board[r][col]) {

            // cout<<"Val : "<< board[r][col] <<endl;
            // cout<<"[ ";
            // for (int d : done) cout<< d <<" ";
            // cout<<"]"<<endl;

            /*  if value already done invalid input */
            for (int d : done) {
                if (d == board[r][col]) return false;
            }
            done.push_back(board[r][col]);
        }
    }
    /*  Clear done for next check   */
    done.clear();

    /*   Checking Grid  */
    int r = 3 * (row/3);
    int c = 3 * (col/3);

    // cout<<"Checking Grid ("<< r <<","<< c <<") ..."<<endl;

    /*  Already adding val to be considered for repetetion  */
    done.push_back(val);

    /*  Itterating through 3 rows   */
    for (int a = 0;a<3;a++) {
        /*  Ittearating through 3 columns   */
        for (int b = 0;b<3;b++) {

            // cout<<"Val : "<< board[r+a][c+b] <<endl;
            // cout<<"[ ";
            // for (int d : done) cout<< d <<" ";
            // cout<<"]"<<endl;

            /*  check only non 0 value   */
            if (board[r+a][c+b]) {
                /*  if value already done invalid input */
                for (int d : done) {
                    if (d == board[r+a][c+b]) return false;
                }
                done.push_back(board[r+a][c+b]);
            }

        }
    }

    return true;
}

void Sudoku :: solve (int itt) {
    /*  Only perform itterations for all empty cells    */
    if (itt < empty_boxes.size() ) {
        /*  Getting Coordinates of empty cell   */
        int r = empty_boxes[itt][0];
        int c = empty_boxes[itt][1];

        /*  Trying all values between 1-9   */
        for (int x = 1;x<10;x++) {
            if (solved) return;

            // cout<<"Itt : "<< itt <<"\t R : "<< r <<"\t C : "<< c <<endl;    
            // cout<<"X : "<< x <<endl;

            /*  If element valid then fill box  */
            if ( check_element(board,r,c,x) ) {
                fill_board(r,c,x);

                // cout<<"\t Valid !"<<endl;
                // print_board();

                /*  Next Itteration */
                solve(itt+1);
            }
            // else cout<<"\t Invalid!"<<endl;

            /*  Undo Fill if board not solved   */
            if ( !solved ) board[r][c] = 0;
        }

        // if (!solved) {
        //     cout<<"Going Back...."<<endl;
        //     print_board();
        // }

    }
    /*  If all empty cells filled then its Solved   */
    else solved = true;
}


