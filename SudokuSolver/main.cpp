#include <iostream>

#include "SudokuBoard.h"
#include "SudokuSolver.h"

int main()
{
   std::cout << "Enter board setup:" << std::endl;
   std::string placements;
   //std::getline(std::cin, placements);
   //placements = "000260701680070090190004500820100040004602900050003028009300074040050036703018000";
   placements = "000200000490006000800040200008009050906070000010800070000560100020000000309007005";

   SudokuBoard sudokuBoard( placements );
   if( sudokuBoard.IsBoardValid() )
   {
      std::cout << "Board is valid" << std::endl;
   }
   else
   {
      std::cout << "Board is invalid" << std::endl;
   }

   std::cout << sudokuBoard << std::endl;

   SudokuSolver sudokuSolver( sudokuBoard );
   while( sudokuSolver.SolveOneStep() )
   {
      std::cout << "Board:" << std::endl;
      std::cout << sudokuBoard << std::endl;
   }

   return 0;
}