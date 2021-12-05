#pragma once

#include <ostream>
#include <string>
#include <vector>

class SudokuBoard
{
public:
   SudokuBoard( const std::string& placements );

   int GetAt( int row, int col ) const;
   void SetAt( int row, int col, int value );

   bool IsBoardValid() const;
   bool IsBoardSolved() const;

   std::vector<int> GetNumbersOnRow( int row ) const;
   std::vector<int> GetNumbersOnCol( int col ) const;
   std::vector<int> GetNumbersIn3x3Grid( int gridIndex ) const;

   int GetGridIndex( int row, int col ) const;

   friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& sudokoBoard);

private:
   std::vector<int> _placements;
};