#include "SudokuSolver.h"
#include "SudokuBoard.h"

#include <algorithm>
#include <cassert>
#include <vector>

namespace
{
    void RemoveTheseNumbers( std::vector<int>& values, const std::vector<int>& numbersToRemoveIfExists )
    {
        for( int n : numbersToRemoveIfExists )
        {
            auto it = std::find( values.begin(), values.end(), n );
            if( it != values.end() )
            {
                values.erase( it );
            }
        }
    }

    void RemoveNumbersFromPossibilities( const SudokuBoard& sudokuBoard, std::vector<int>& possibleValues, int row, int col )
    {
        RemoveTheseNumbers( possibleValues, sudokuBoard.GetNumbersOnRow( row ) );
        RemoveTheseNumbers( possibleValues, sudokuBoard.GetNumbersOnCol( col ) );
        RemoveTheseNumbers( possibleValues, sudokuBoard.GetNumbersIn3x3Grid( sudokuBoard.GetGridIndex( row, col) ) );

        if( sudokuBoard.GetBoardType() == KnightSudoku )
        {
            RemoveTheseNumbers( possibleValues, sudokuBoard.GetNumbersKnightsDistance( row, col ) );
        }
    }
}

SudokuSolver::SudokuSolver( SudokuBoard& sudokuBoard )
: _sudokuBoard( sudokuBoard )
{

}

bool SudokuSolver::SolveOneStep()
{
   if( _sudokuBoard.IsBoardSolved() || !_sudokuBoard.IsBoardValid())
      return false;

   if( SolveOneMissingValue() != false )
      return true;

   //This means the puzzle is even harder.  Let's copy the board; make a guess and see if it becomes invalid ever
   if( SolveOneTakingGuess() != false )
     return true;

    return false;
}

bool SudokuSolver::SolveOneMissingValue()
{
    if( _sudokuBoard.IsBoardSolved() || !_sudokuBoard.IsBoardValid())
      return false;

   for( int row = 0; row < 9; row++ )
   {
       for( int col = 0; col < 9; col++ )
       {
          int value = _sudokuBoard.GetAt( row, col );
          if( value != 0 )
             continue;

            std::vector<int> possibleValues{ 1, 2, 3, 4, 5, 6, 7, 8, 9};
            RemoveNumbersFromPossibilities( _sudokuBoard, possibleValues, row, col );

            if( possibleValues.size() == 1 )
            {
                _sudokuBoard.SetAt( row, col, possibleValues.front() );
                if( _sudokuBoard.IsBoardValid() == false )
                {
                    assert(false);
                }
                return true;
            }
       }
   }

   return false;
}

bool SudokuSolver::SolveOneTakingGuess()
{
    if( _sudokuBoard.IsBoardSolved() || !_sudokuBoard.IsBoardValid())
      return false;

    for( int row = 0; row < 9; row++ )
    {
       for( int col = 0; col < 9; col++ )
       {
          int value = _sudokuBoard.GetAt( row, col );
          if( value != 0 )
             continue;

            std::vector<int> possibleValues{ 1, 2, 3, 4, 5, 6, 7, 8, 9};
            RemoveNumbersFromPossibilities( _sudokuBoard, possibleValues, row, col );

            for( int possibleValue : possibleValues )
            {
                SudokuBoard copyOfBoard = _sudokuBoard;

                copyOfBoard.SetAt(row, col, possibleValue);

                SudokuSolver solver( copyOfBoard );
                while( solver.SolveOneStep() );
                if( copyOfBoard.IsBoardSolved() )
                {
                    _sudokuBoard.SetAt( row, col, possibleValue );
                    return true;
                }
            }
       }
   }

   return false;
}