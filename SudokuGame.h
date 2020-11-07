//
// Created by kevin on 7/17/2020.
//

#ifndef SUDOKU_SUDOKUGAME_H
#define SUDOKU_SUDOKUGAME_H


#include <utility>
#include <vector>
#include "definitions.h"
#include "Board.h"

class SudokuGame {
private:
   Board board;

   /// Methods bellow are for the SudokuGame::solve method

   /**
    * Sets c to the coordinate to the next 3x3 box on the board
    * TODO: this should be a Board property
    * @param c
    */
   void nextBoxStart(Coordinate *c);

   /**
    * Will fill 1x1 square if there is only 1 Possible Value
    * for that square
    * @param squareInQuestion
    * @return
    */
   bool fillSinglePVFor(Square *squareInQuestion);

   /**
    * Step 1: In a box, find all other empty squares, find their PVs, and store in a vector
    * Step 2: Compare PVs for squareInQuestion to other PVs of other squares in same box
    * Step 3: If a PV is not found in others' PVs then that's the value to plug in
    * @param squareInQuestion
    * @return
    */
   bool fillByEliminationFor(Square *squareInQuestion);

   /**
    * Is this possible value the most probable?
    * This function answers that question by looking at the PVs in other related squares
    * See SudokuGame::fillByEliminationFor(squareInQuestion) for more context
    * @param PVInQuestion Possible values in question
    * @param otherSquares
    * @return
    */
   bool isNotFoundInOthersPVs(int PVInQuestion, const std::vector<Square*>& otherSquares);

   /**
    * Did the solver algorithm do its job?
    * @param board
    * @return
    */
   bool isSolved(Board board);
public:
    explicit SudokuGame(Board b) : board(std::move(b)) {}
    Board & getBoard() { return board; }
    void setBoard(Board b) { SudokuGame::board = std::move(b); }

    static void promptFilePath(char *filePath);
    void play();
    static void displayInstructions();
    static void displayPossibleValues(Coordinate coord, std::vector<int> *values);
    static Coordinate promptCoordinate();

    /**
     * Iterate through boxes
     *    1. for squares that have single PVs
     *    2. to single out the PVs by process of elimination
     *
     * Currently, same as Solve in main.cpp (07-NOV-2020)
     */
    void solve();
};


#endif //SUDOKU_SUDOKUGAME_H
