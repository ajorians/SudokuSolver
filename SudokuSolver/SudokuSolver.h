#pragma once

#include "SudokuBoard.h"

class SudokuSolver
{
public:
   SudokuSolver( const SudokuBoard& sudokuBoard );

   //Tries to place a single number.  Returns false if no replacement made
   bool SolveOneStep();

   bool SolveOneMissingValue();//If only one possible value for a spot it will use it
   bool SolveOne3x3OnlySpotForValue();//Will check it's values if any other spot in 3x3 area works
   bool SolveOneRowColSpotForValue();//Will check if it's values if any other spot in row/col work
   bool SolveOneTryingPossibilities();//Will use it possible values and make sure other spots have possibilites and board is still valid
   bool SolveOneTakingGuess();

   bool DidSolvePuzzle() const;

   const SudokuBoard& GetBoardSolving() const;

private:
   SudokuBoard _sudokuBoard;
};