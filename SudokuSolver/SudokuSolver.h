#pragma once

#include "SudokuBoard.h"

class SudokuSolver
{
public:
   SudokuSolver( const SudokuBoard& sudokuBoard );

   //Tries to place a single number.  Returns false if no replacement made
   bool SolveOneStep();
   bool SolveOneMissingValue();
   bool SolveOne3x3OnlySpotForValue();
   bool SolveOneRowColSpotForValue();
   bool SolveOneTryingPossibilities();
   bool SolveOneTakingGuess();

   bool DidSolvePuzzle() const;

   const SudokuBoard& GetBoardSolving() const;

private:
   SudokuBoard _sudokuBoard;
};