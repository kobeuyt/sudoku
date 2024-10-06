#include <array>
#include <bitset>
#include <string>

using std::array;
using std::bitset;

class SudokuBoard
{
    public:
        SudokuBoard();

        bool ReadBoardInput(const std::string& filepath);
        void SetBoard();
        size_t GetCell(const size_t& row, const size_t& col);
        void SolveBoard();
        bool Solve(array<array<int,9>,9>& unsolvedBoard,
                   size_t row,
                   size_t col,
                   array<bitset<9>,9>&,
                   array<bitset<9>,9>&,
                   array<bitset<9>,9>&);
        std::pair<size_t, size_t> NextEmptyPosition(array<array<int,9>,9>& board, 
                                                    size_t& row,
                                                    size_t& col);
        size_t GetNextRow(const size_t& row, const size_t& col);
        size_t GetNextCol(const size_t& col);
        void PrintBoard();
        void PrintBitset();

    private:
        array<array<int, 9>, 9> board;
        array<bitset<9>, 9> rows_state;
        array<bitset<9>, 9> cols_state;
        array<bitset<9>, 9> cell_state;
};
