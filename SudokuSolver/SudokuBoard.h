#pragma once

#include <ostream>
#include <string>
#include <vector>

enum BoardType
{
   Traditional,
   KnightSudoku
};

class SudokuBoard
{
public:
   SudokuBoard( const std::string& placements, BoardType boardType = Traditional );

   int GetAt( int row, int col ) const;
   void SetAt( int row, int col, int value );

   BoardType GetBoardType() const { return _boardType; }

   bool IsBoardValid() const;
   bool IsBoardSolved() const;

   std::vector<int> GetNumbersOnRow( int row ) const;
   std::vector<int> GetNumbersOnCol( int col ) const;
   std::vector<int> GetNumbersIn3x3Grid( int gridIndex ) const;
   std::vector<int> GetNumbersKnightsDistance( int row, int col ) const;

   int GetGridIndex( int row, int col ) const;

   friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& sudokoBoard);

private:
   std::vector<int> _placements;
   BoardType _boardType;
};