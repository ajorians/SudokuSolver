#include "SudokuSolver.h"
#include "SudokuBoard.h"

#include <algorithm>
#include <cassert>
#include <tuple>
#include <utility>
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

    bool CouldSpotHaveValue( const SudokuBoard& sudokuBoard, int row, int col, int possibility )
    {
        assert( sudokuBoard.GetAt( row, col ) == 0 );

        SudokuBoard copyOfBoard = sudokuBoard;
        copyOfBoard.SetAt( row, col, possibility );
        return copyOfBoard.IsBoardValid();
    }

    bool CouldAnother3x3HaveValue( const SudokuBoard& sudokuBoard, int row, int col, int possibility )
    {
        int gridIndex = sudokuBoard.GetGridIndex( row, col );
        int row3x3 = (gridIndex / 3 ) * 3;
        int col3x3 = (gridIndex * 3) % 9;
        for( int xOffset = 0; xOffset<3; xOffset++)
        {
            for( int yOffset = 0; yOffset<3; yOffset++)
            {
                int thisRow = row3x3 + yOffset;
                int thisCol = col3x3 + xOffset;
                if( thisRow == row && thisCol == col )
                    continue;

                int value = sudokuBoard.GetAt( thisRow, thisCol );
                if( value != 0)
                    continue;

                if( CouldSpotHaveValue( sudokuBoard, thisRow, thisCol, possibility ) )
                    return true;
            }
        }

        return false;
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

    if( SolveOne3x3OnlySpotForValue() != false )
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

bool SudokuSolver::SolveOne3x3OnlySpotForValue()
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

            for( int possibility : possibleValues )
            {
                //Could this value work on a different spot in the same 3x3 grid?  If not it is this one
                if( CouldAnother3x3HaveValue( _sudokuBoard, row, col, possibility ) )
                    continue;
                
                _sudokuBoard.SetAt( row, col, possibility );
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

    std::vector< std::tuple< std::pair<int, int>, std::vector<int> > > possibilities;

    for( int row = 0; row < 9; row++ )
    {
       for( int col = 0; col < 9; col++ )
       {
          int value = _sudokuBoard.GetAt( row, col );
          if( value != 0 )
             continue;

            std::vector<int> possibleValues{ 1, 2, 3, 4, 5, 6, 7, 8, 9};
            RemoveNumbersFromPossibilities( _sudokuBoard, possibleValues, row, col );

            std::tuple< std::pair<int, int>, std::vector<int> > possibility( std::pair<int, int>( row, col ), possibleValues );
            possibilities.push_back( possibility );
       }
    }

    std::sort( possibilities.begin(), possibilities.end(), []( const auto& a, const auto& b )
    {
        return std::get<1>(a).size() < std::get<1>(b).size();
    });

    for( const auto& possibility : possibilities )
    {
        std::pair<int, int> location = std::get<0>( possibility );
        std::vector<int> possibleValues = std::get<1>( possibility );
        int row = location.first;
        int col = location.second;

        for( int possibleValue : possibleValues )
        {
            SudokuBoard copyOfBoard = _sudokuBoard;

            copyOfBoard.SetAt(row, col, possibleValue);

            SudokuSolver solver( copyOfBoard );
            while( solver.SolveOneStep() );
            if( solver.DidSolvePuzzle() )
            {
                int correctValue = solver._sudokuBoard.GetAt( row, col );
                _sudokuBoard.SetAt( row, col, correctValue );
                return true;
            }
        }
    }

   return false;
}

bool SudokuSolver::DidSolvePuzzle() const
{
   return _sudokuBoard.IsBoardSolved();
}