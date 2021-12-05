#pragma once

class SudokuBoard;

class SudokuSolver
{
public:
   SudokuSolver( SudokuBoard& sudokuBoard );

   //Tries to place a single number.  Returns false if no replacement made
   bool SolveOneStep();
   bool SolveOneMissingValue();
   bool SolveOne3x3OnlySpotForValue();
   bool SolveOneTakingGuess();

   bool DidSolvePuzzle() const;

private:
   SudokuBoard& _sudokuBoard;
};