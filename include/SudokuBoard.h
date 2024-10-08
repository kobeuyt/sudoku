#include <array>
#include <bitset>
#include <string>

using std::array;
using std::bitset;

class SudokuBoard
{
    public:
        SudokuBoard();
        ~SudokuBoard() = default;
        SudokuBoard(const SudokuBoard&) = default;
        SudokuBoard& operator=(const SudokuBoard&) = default;

        bool ReadBoardInput(const std::string& filepath);
        void ReadBoardFromString(const std::string& boardStr);

        array<array<char,9>,9>& GetBoard();
        size_t GetCell(const size_t& row, const size_t& col);
        void SolveBoard();
        bool Solve(array<array<char,9>,9>& unsolvedBoard,
                   size_t row,
                   size_t col,
                   array<bitset<9>,9>&,
                   array<bitset<9>,9>&,
                   array<bitset<9>,9>&);
        std::pair<size_t, size_t> NextEmptyPosition(array<array<char,9>,9>& board, 
                                                    size_t& row,
                                                    size_t& col);
        size_t GetNextRow(const size_t& row, const size_t& col);
        size_t GetNextCol(const size_t& col);
        void PrintBoard();
        void PrintBitset();

    private:
        array<array<char, 9>, 9> board;
        array<bitset<9>, 9> rows_state;
        array<bitset<9>, 9> cols_state;
        array<bitset<9>, 9> cell_state;
};
