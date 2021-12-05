#include "SudokuBoard.h"

#include <algorithm>

SudokuBoard::SudokuBoard( const std::string& placements )
{
    _placements.resize( 9*9, 0/*initial value*/ );

    auto it = _placements.begin();
    for( char ch : placements )
    {
        int value = ch - '0';
        *it = value;
        it++;
    }
}

int SudokuBoard::GetAt( int row, int col ) const
{
   int index = row*9 + col;
   return _placements[index];
}

void SudokuBoard::SetAt( int row, int col, int value )
{
   int index = row*9 + col;
   _placements[index] = value;
}

bool SudokuBoard::IsBoardValid() const
{
    //Any duplicates on same row
    for( int row=0; row < 9; row++)
    {
        std::vector<int> values = GetNumbersOnRow( row );
        if( std::unique( values.begin(), values.end() ) != values.end() )
        {
            return false;
        }
    }
    
    //Any duplicates on same column
    for( int col=0; col < 9; col++)
    {
        std::vector<int> values = GetNumbersOnCol( col );
        if( std::unique( values.begin(), values.end() ) != values.end() )
        {
            return false;
        }
    }

    //Any duplicates in same 3x3 grid
    for( int gridIndex=0; gridIndex < 9; gridIndex++)
    {
        std::vector<int> values = GetNumbersIn3x3Grid( gridIndex );
        if( std::unique( values.begin(), values.end() ) != values.end() )
        {
            return false;
        }
    }

    return true;
}

bool SudokuBoard::IsBoardSolved() const
{
    for( int row=0; row < 9; row++)
    {
        for( int col = 0; col < 9; col++)
        {
            int value = GetAt( row, col );
            if( value == 0 )
               return false;
        }
    }

    return true;
}

std::vector<int> SudokuBoard::GetNumbersOnRow( int row ) const
{
    std::vector<int> result;
    for( int col = 0; col<9; col++)
    {
        int value = GetAt( row, col );
        if( value == 0)
           continue;
        result.push_back( value );
    }
    return result;
}

std::vector<int> SudokuBoard::GetNumbersOnCol( int col ) const
{
   std::vector<int> result;
    for( int row = 0; row<9; row++)
    {
        int value = GetAt( row, col );
        if( value == 0)
           continue;
        result.push_back( value );
    }
    return result;
}

std::vector<int> SudokuBoard::GetNumbersIn3x3Grid( int gridIndex ) const
{
    std::vector<int> result;
    int row = (gridIndex / 3 ) * 3;
    int col = (gridIndex * 3) % 9;
    for( int xOffset = 0; xOffset<3; xOffset++)
    {
        for( int yOffset = 0; yOffset<3; yOffset++)
        {
            int value = GetAt( row + xOffset, col + yOffset );
            if( value == 0)
                continue;
            result.push_back( value );
        }
    }
    return result;
}

int SudokuBoard::GetGridIndex( int row, int col ) const
{
    if( row >= 0 && row < 3)
    {
        if( col >=0 && col < 3 )
        {
            return 0;
        }
        else if( col >=3 && col < 6 )
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    else if( row >= 3 && row < 6)
    {
        if( col >=0 && col < 3 )
        {
            return 3;
        }
        else if( col >=3 && col < 6 )
        {
            return 4;
        }
        else
        {
            return 5;
        }
    }
    else
    {
        if( col >=0 && col < 3 )
        {
            return 6;
        }
        else if( col >=3 && col < 6 )
        {
            return 7;
        }
        else
        {
            return 8;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const SudokuBoard& sudokoBoard)
{
    for( int row=0; row < 9; row++)
    {
        for( int col = 0; col < 9; col++)
        {
            int value = sudokoBoard.GetAt( row, col );
            if( value > 0 && value < 10 )
            {
                os << value;
            }
            else
            {
                os << '-';
            }

            if( col == 2 || col == 5 )
            {
                os << '|';
            }
        }
        os << std::endl;

        if( row == 2 || row == 5 )
        {
            os << "-----------" << std::endl;
        }
    }
    return os;
}