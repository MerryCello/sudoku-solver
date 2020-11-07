cmake_minimum_required(VERSION 3.15)
project(sudoku)

set(CMAKE_CXX_STANDARD 14)

add_executable(sudoku main.cpp Coordinate.h Coordinate.cpp Board.cpp Board.h Square.cpp Square.h SudokuGame.cpp SudokuGame.h definitions.h)