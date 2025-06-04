# Sudoku Solver

This project implements two methods for solving Sudoku puzzles:

1. **Backtracking Algorithm**
2. **State Reduction using Naked and Hidden Singles**

## Methods Explained

### 1. Backtracking Algorithm
The backtracking algorithm is a brute-force approach that systematically explores all possible solutions for the Sudoku puzzle. It works by:
- Placing a number in an empty cell.
- Recursively attempting to solve the puzzle with the current placement.
- Backtracking if the placement leads to an invalid state.

This method guarantees a solution if one exists and has been observed to work significantly faster on easier Sudoku boards where roughly 50% of the board is already filled. 

### 2. State Reduction (Naked and Hidden Singles)
State reduction is a more efficient approach that reduces the complexity of the puzzle by applying logical techniques:
- **Naked Singles**: If a cell has only one possible value, it is directly filled with that value.
- **Hidden Singles**: If a number can only fit in one cell within a row, column, or box, it is placed in that cell.

While this method is faster than backtracking for many puzzles, it may fail for certain boards that require guesses or advanced techniques.

## Combining the Methods
Although a hybrid approach combining backtracking and state reduction can be used to solve more complex puzzles, this project does not currently implement such a mixed version. The focus here is on demonstrating the individual strengths of each method.

## Limitations
- The state reduction method alone cannot solve all Sudoku puzzles, especially those requiring guesses or advanced techniques.
- The backtracking approach, while effective, can be computationally expensive for highly complex puzzles.
- The hybrid approach, though promising, is not implemented in this project.

This project highlights the trade-offs between logical deduction and systematic exploration when solving Sudoku puzzles.
