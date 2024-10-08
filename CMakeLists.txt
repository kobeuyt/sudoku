cmake_minimum_required(VERSION 3.10)
project(Sudoku VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

aux_source_directory(${SRC_DIR} SRC)

add_executable(sudoku ${SRC})

add_library(SudokuPy SHARED ${SRC_DIR}/SudokuBoard.cpp)

target_include_directories(sudoku PUBLIC "${INCLUDE_DIR}")
target_include_directories(SudokuPy PRIVATE ${INCLUDE_DIR})
