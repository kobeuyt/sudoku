#!/home/kobeuyt/dev/bin/python3

import ctypes
import os
import platform
import sys
from PyQt6 import QtWidgets, QtCore, QtGui

GUI_DIR = os.path.dirname(os.path.abspath(__file__))
BASE_DIR = os.path.dirname(GUI_DIR)
LIB_DIR = os.path.join(BASE_DIR, "bin")

class SudokuWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.main_widget = None
        self.main_layout = None

        # Action Buttons
        self.button_layout = None
        self.solve_button = None
        self.clear_button = None

        # Sudoku Board
        self.board_widget = None
        self.board_layout = None
        self.board_size_policy = None

        self.sudoku_lib = ctypes.CDLL(os.path.join(LIB_DIR, "libSudokuPy") + self.get_lib_ext())
        self.sudoku_lib.Solve.restype = ctypes.c_void_p
        self.sudoku_lib.Solve.argtypes = (ctypes.c_char_p,)

        self.setup_ui()
        self.solve_button.clicked.connect(self.solve_board)
        self.clear_button.clicked.connect(self.clear_board)


    def setup_ui(self):
        self.board_layout = QtWidgets.QGridLayout()
        
        for row in range(9):
            for col in range(9):
                cell = QtWidgets.QLineEdit()
                cell.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
                cell.setMinimumHeight(25)
                cell.setMaximumWidth(25)
                cell.setMaxLength(1)
                cell.setValidator(QtGui.QIntValidator(1, 9))
                self.board_layout.addWidget(cell, row, col)

        self.board_layout.setSpacing(0)

        self.board_widget = QtWidgets.QWidget()
        self.board_widget.setLayout(self.board_layout)

        self.board_size_policy = QtWidgets.QSizePolicy()
        self.board_size_policy.setHeightForWidth(True)
        self.board_widget.setSizePolicy(self.board_size_policy)

        self.button_layout = QtWidgets.QVBoxLayout()

        self.solve_button = QtWidgets.QPushButton()
        self.solve_button.setText("Solve")

        self.button_layout.addWidget(self.solve_button)

        self.clear_button = QtWidgets.QPushButton()
        self.clear_button.setText("Clear")
        
        self.button_layout.addWidget(self.clear_button)

        self.main_layout = QtWidgets.QGridLayout()
        self.main_layout.addWidget(QtWidgets.QLabel("Sudoku Solver"), 0, 0, 0, 2, QtCore.Qt.AlignmentFlag.AlignTop)
        self.main_layout.addWidget(self.board_widget, 1, 0)
        self.main_layout.addLayout(self.button_layout, 1, 1)

        self.main_widget = QtWidgets.QWidget(self)
        self.main_widget.setFixedSize(500, 500)
        self.main_widget.setLayout(self.main_layout)

        self.setCentralWidget(self.main_widget)
        self.setWindowTitle("Sudoku Solver")
        self.show()

    
    def solve_board(self):
        myBoard = ""
        for row in range(9):
            for col in range(9):
                cell = self.board_layout.itemAtPosition(row, col).widget().text()
                if not cell: 
                    cell = "."
                myBoard += cell
        solvedBoardPtr = self.sudoku_lib.Solve(myBoard.encode("utf-8"))
        solvedBoard = ctypes.string_at(ctypes.cast(solvedBoardPtr, ctypes.c_char_p)).decode("utf-8")
        self.sudoku_lib.FreeMem(solvedBoardPtr)
        solvedIndex = 0
        for row in range(9):
            for col in range(9):
                self.board_layout.itemAtPosition(row, col).widget().setText(solvedBoard[solvedIndex])
                solvedIndex += 1


    def clear_board(self):
        for row in range(9):
            for col in range(9):
                self.board_layout.itemAtPosition(row, col).widget().setText("")


    def get_lib_ext(self):
        system = platform.system()
        if system == "Linux":
            return ".so"
        elif system == "Darwin":
            return ".dylib"
        elif system == "Windows":
            return ".dll"
        else:
            return None


def main():
    app = QtWidgets.QApplication(sys.argv)
    main_window = SudokuWindow()

    return(app.exec())

if __name__ == "__main__":

    return_code = main()

    sys.exit(return_code)
