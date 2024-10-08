#include <array>
#include <bitset>
#include <iostream>
#include <string>

#include "SudokuBoard.h"

int main(int argc, char** argv)
{
    auto board = SudokuBoard();
    std::string boardstr = "5..26.873...5......2...3...48.3..69..9..125..751.......45.2....1..9.......61..48.";

    board.ReadBoardFromString(boardstr);
    board.SolveBoard();

    // if (board.ReadBoardInput(argv[1]))
    // {
    //     board.SolveBoard();
    // }
}
