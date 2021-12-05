#pragma once

class SudokuBoard;

class SudokuSolver
{
public:
   SudokuSolver( SudokuBoard& sudokuBoard );

   //Tries to place a single number.  Returns false if no replacement made
   bool SolveOneStep();
   bool SolveOneMissingValue();
   bool SolveOneTakingGuess();

private:
   SudokuBoard& _sudokuBoard;
};