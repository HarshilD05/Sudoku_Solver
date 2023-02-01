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
    // To be returned
    vector<vector<int>> empty;

    for (int i = 0;i<9;i++) {
        for (int j = 0;j<9;j++) {
            if (board[i][j] == 0) { 
                vector<int> e = {i,j};
                empty.push_back(e);
            }
        }
    }

    // Printing empty cells
    // for (auto p : empty) {
    //     cout<<"["<< p[0] <<","<< p[1] <<"] ";
    // }
    // cout<<endl;

    return empty;
}

bool Sudoku :: check_element (vector<vector<int>> board,int row,int col,int val) {
    // Elements which are already used
    vector<int> done;

    // Checking rows
    // cout<<"Checking Rows..."<<endl;
    done.push_back(val);

    for (int c = 0;c<9;c++) {
        if (board[row][c]) {

            // cout<<"Val : "<< board[row][c] <<endl;
            // cout<<"[ ";
            // for (int d : done) cout<< d <<" ";
            // cout<<"]"<<endl;

            for (int d : done) {
                if (d == board[row][c]) return false;
            }
            done.push_back(board[row][c]);
        }
    }
    done.clear();

    // checking Columns
    // cout<<"Checking Columns..."<<endl;
    done.push_back(val);

    for (int r = 0;r<9;r++) {
        if (board[r][col]) {

            // cout<<"Val : "<< board[r][col] <<endl;
            // cout<<"[ ";
            // for (int d : done) cout<< d <<" ";
            // cout<<"]"<<endl;

            for (int d : done) {
                if (d == board[r][col]) return false;
            }
            done.push_back(board[r][col]);
        }
    }
    done.clear();

    // Checking Grid
    int r = 3 * (row/3);
    int c = 3 * (col/3);

    // cout<<"Checking Grid ("<< r <<","<< c <<") ..."<<endl;

    done.push_back(val);

    for (int a = 0;a<3;a++) {
        for (int b = 0;b<3;b++) {

            // cout<<"Val : "<< board[r+a][c+b] <<endl;
            // cout<<"[ ";
            // for (int d : done) cout<< d <<" ";
            // cout<<"]"<<endl;

            if (board[r+a][c+b]) {
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
    if (itt < empty_boxes.size() ) {

        int r = empty_boxes[itt][0];
        int c = empty_boxes[itt][1];


        for (int x = 1;x<10;x++) {
            if (solved) return;

            cout<<"Itt : "<< itt <<"\t R : "<< r <<"\t C : "<< c <<endl;    
            cout<<"X : "<< x <<endl;

            if ( check_element(board,r,c,x) ) {
                fill_board(r,c,x);

                cout<<"\t Valid !"<<endl;
                print_board();

                solve(itt+1);
            }
            else cout<<"\t Invalid!"<<endl;

            if ( !solved ) board[r][c] = 0;
        }

        if (!solved) {
            cout<<"Going Back...."<<endl;
            print_board();
        }

    }
    else solved = true;
}


