#include "SudokuBoard.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

SudokuBoard::SudokuBoard():
    board(),
    rows_state(),
    cols_state(),
    cell_state()
{
    rows_state = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    cols_state = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    cell_state = {0, 0, 0, 0, 0, 0, 0, 0, 0};
}

bool SudokuBoard::ReadBoardInput(const std::string& filepath)
{
    try
    {
        std::ifstream file;
        file.open(filepath);
        std::string line;
        char token;
        int row = 0;

        // Loop through file while we have input to read
        while (file)
        {
            // Grab next line of file to be processed
            std::getline(file, line);
            std::istringstream stream(line);
            std::vector<char> input;

            // istringstream automatically delimits whitespace
            while (stream >> token)
            {
                input.push_back(token);
            }

            // Copy input into row
            std::copy(input.begin(), input.end(), board[row].begin());
            row++;
        }
        file.close();

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to read " << filepath << e.what() << '\n';
    }

    return false;
}

void SudokuBoard::ReadBoardFromString(const std::string& boardStr)
{
    size_t index = 0;
    for (size_t row = 0; row < 9; row++)
    {
        for (size_t col = 0; col < 9; col++)
        {
            board[row][col] = boardStr[index];
            index++;
        }
    }
}

extern "C" 
{    
    const char* Solve(char* boardIn)
    {
        auto myBoard = SudokuBoard();
        std::string boardStr(boardIn);
        std::string solvedBoard = "";

        myBoard.ReadBoardFromString(boardStr);
        myBoard.SolveBoard();

        for (size_t row = 0; row < 9; row++)
        {
            for (size_t col = 0; col < 9; col++)
            {
                solvedBoard += myBoard.GetBoard()[row][col];
            }
        }

        const char* cstr = strdup(solvedBoard.c_str());
        return cstr;
    }

    void FreeMem(char* str)
    {
        free(str);
    }
}

array<array<char,9>,9>& SudokuBoard::GetBoard()
{
    return board;
}

size_t SudokuBoard::GetCell(const size_t& row, const size_t& col)
{
    return (row / 3) * 3 + col / 3;
}

void SudokuBoard::SolveBoard()
{
    char num;
    size_t state;
    for (size_t row = 0; row < board.size(); row++)
    {
        for (size_t col = 0; col < board[row].size(); col++)
        {
            if ((num = board[row][col]) != '.')
            {
                state = num - '1';
                rows_state[row].set(state);
                cols_state[col].set(state);
                cell_state[GetCell(row, col)].set(state);
            }
        }
    }
    Solve(board, 0, 0, rows_state, cols_state, cell_state);
}

bool SudokuBoard::Solve(array<array<char,9>,9>& unsolvedBoard,
                        size_t rowStart,
                        size_t colStart,
                        array<bitset<9>,9>& rowContains,
                        array<bitset<9>,9>& colContains,
                        array<bitset<9>,9>& cellContains)
{
    auto [row, col] = NextEmptyPosition(unsolvedBoard, rowStart, colStart);

    if (row == 9)
    {
        return true;
    }

    size_t cell = GetCell(row, col);
    bitset<9> contains = rowContains[row] | colContains[col] | cellContains[cell];
    if (contains.all())
    {
        return false;
    }

    for (size_t num = 0; num < 9; num++)
    {
        if (!contains[num])
        {
            unsolvedBoard[row][col] = static_cast<char>(num + '1');
            rowContains[row].set(num);
            colContains[col].set(num);
            cellContains[cell].set(num);
            if (Solve(unsolvedBoard, row, col, rowContains, colContains, cellContains))
            {
                return true;
            }

            rowContains[row].reset(num);
            colContains[col].reset(num);
            cellContains[cell].reset(num);
        }
    }
    unsolvedBoard[row][col] = '.';
    return false;
}

std::pair<size_t, size_t> SudokuBoard::NextEmptyPosition(array<array<char,9>,9>& unsolvedBoard, 
                                                         size_t& row, 
                                                         size_t& col)
{
    while (row != 9)
    {
        if (unsolvedBoard[row][col] == '.')
        {
            return {row, col};
        }
        row = GetNextRow(row, col);
        col = GetNextCol(col);
    }
    return {9, 0};
}

size_t SudokuBoard::GetNextRow(const size_t& row, const size_t& col)
{
    return row + (col + 1) / 9;
}

size_t SudokuBoard::GetNextCol(const size_t& col)
{
    return (col + 1) % 9;
}

void SudokuBoard::PrintBoard()
{
    for (auto row : board)
    {
        for (auto col : row)
        {
            std:: cout << col << ", ";
        }
        std::cout << std::endl;
    }
}

void SudokuBoard::PrintBitset()
{
    for (auto bit : rows_state)
    {
        std::cout << bit << " ";
    }
    std::cout << std::endl;
}
