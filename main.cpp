/**********************************************************
 * @author Kevin Foniciello
 **********************************************************/

#include <iostream>
#include <vector>
#include "definitions.h"
#include "Board.h"
#include "SudokuGame.h"
using namespace std;

#ifdef SOLVER
void solve(Board & board);
void nextBoxStart(Coordinate *c);
bool fillSinglePVFor(Square *squareInQuestion);
bool fillByEliminationFor(Square *squareInQuestion, Board *board);
bool isNotFoundInOthersPVs(int PVInQuestion, const vector<Square*>& otherSquares);
bool isSolved(Board board);
#endif

/**
 *
 * @param argc
 * @param argv
 * @return
 */
#ifdef DEBUG
int main(int argc, char* argv[]) {
   if (argc == 1) {
      cout << "Expected at least one argument.\n";
      return 0;
   }

   for (int i = 1; i < argc; i++) {
      char* filePath = argv[i];
#else
int main() {
   char filePath[256];
#endif
      SudokuGame::promptFilePath(filePath);
      SudokuGame game((Board(filePath)));

      if (!(game.getBoard().hasReadSuccessfully()))
         return 0;

#ifdef SOLVER
      solve(game.getBoard());
#endif
      /// In DEBUG and SOLVER mode, comment out to see all boards being solved
      game.play();
#ifdef DEBUG
   } /// for loop
#endif

   return 0;
}

#ifdef SOLVER
/** Iterate through boxes
 *     1. for squares that have single PVs
 *     2. to single out the PVs by process of elimination
 *
 *  Currently, same as SudokuFame::Solve (07-NOV-2020)
 */
void solve(Board & board) {
    Board savedBoard;
    do {
        savedBoard = board;

#ifdef DEBUG
        cerr << board << endl;
#endif
        /// Iterate through each box of board
        bool endOfBoard = false;
        for (Coordinate boxStart(0, 0); !endOfBoard; nextBoxStart(&boxStart)) {

            /// Iterate through the box
            Coordinate coord;
            for (coord.setY(boxStart.getY()); coord.getY() < boxStart.getY()+3; coord.getY()++) {
                for (coord.setX(boxStart.getX()); coord.getX() < boxStart.getX()+3; coord.getX()++) {

//                    cerr << board.getValueAt(x, y) << " ";

                    if (board.getSquareAt(coord).isEmpty()) {
                        board.findPossibleValuesAt(coord);

                        /// fill in single PVs
                        if (fillSinglePVFor(&(board.getSquareAt(coord))))
                            continue;

                        /// single out and fill PVs by process of elimination
                        if (fillByEliminationFor(&(board.getSquareAt(coord)), &board))
                            continue;
                    }
                }
//                cerr << endl;
            }
            endOfBoard = (coord.getX() == 9 && coord.getY() == 9);
//            cerr << endl;
        }
    } while(!isSolved(board) && savedBoard != board);

    if (!isSolved(board) && savedBoard == board)
        cout << "\nThe Sudoku was not solved...\n"
             << "Something is wrong with the algorithm...\n";
    else if (isSolved(board))
        cout << "\nThe Sudoku is solved!\n"
             << "Here is the solution:\n";

/*    cerr << Coordinate(1, 2) << " = " << board.getValueAt(Coordinate(1, 2)) << endl;

//    for (int y = 0; y < 9; y++) {
//        for (int x = 0; x < 9; x++) {
//            if (board.getSquareAt(Coordinate(x, y)).isChangeable()) {
//                SudokuGame::displayPossibleValues(
//                        Coordinate(x, y),
//                        board.findPossibleValuesAt(x, y)
//                );
//            }
//        }
//    }
*/
#ifdef DEBUG
    cout << board << endl;
#endif
}

void nextBoxStart(Coordinate *c) {
    c->getX() = (c->getX() + 3) % 9;
    c->getY() += (c->getX() ? 0 : 3);
}

bool fillSinglePVFor(Square *squareInQuestion) {
    vector<int>* possibleValues = squareInQuestion->getPossibleValues();
    if (possibleValues->size() == 1) {
        squareInQuestion->setValue((*possibleValues)[0]);
        return true;
    }
    return false;
}

/**
 * Step 1: In a box, find all other empty squares, find their PVs, and store in a vector
 * Step 2: Compare PVs for squareInQuestion to other PVs of other squares in same box
 * Step 3: If a PV is not found in others' PVs then that's the value to plug in
 */
bool fillByEliminationFor(Square *squareInQuestion, Board *board) {
    Coordinate boxStart = Board::getBoxStartFor(squareInQuestion->getCoordinate());
    vector<Square*> otherEmptySquares;

    /// Step 1
    for (int y = boxStart.getY(); y < boxStart.getY()+3; y++) {
        for (int x = boxStart.getX(); x < boxStart.getX()+3; x++) {
            Square* pSquare = &(board->getSquareAt(Coordinate(x, y)));
            if (pSquare->isEmpty() && pSquare != squareInQuestion) {
                board->findPossibleValuesAt(pSquare->getCoordinate());
                otherEmptySquares.push_back(pSquare);
            }
        }
    }

    /// Step 2
    for (auto possibleValue : *squareInQuestion->getPossibleValues()) {
        if (isNotFoundInOthersPVs(possibleValue, otherEmptySquares)) {
    /// Step 3
            squareInQuestion->setValue(possibleValue);
            return true;
        }
    }

    return false;
/*    for (int possibleValue = 1; possibleValue <= 9; possibleValue++) {
//
//        /// Continue until a possible value is found
//        bool gotoNextPossibleValue = !(squareInQuestion.getPossibleValues()[possibleValue]);
//        if (gotoNextPossibleValue)
//            continue;
//
//        /// Iterate through rows and columns in a box
//        for (int column = boxStart.getX(), row = boxStart.getY();
//             column < boxStart.getX() + 3 && row < boxStart.getY() + 3;
//             column++, row++                                                 ) {
//
//            /// Iterate through the row
//            if (row != squareInQuestion.getCoordinate().getY()) {
//                for (int x = 0; x < 9; x++) {
//                    /// Cut it short if the PV is found in this row
//                    if (board.getValueAt(x, row) == possibleValue) {
//                        gotoNextPossibleValue = true;
//                        break;
//                    }
//                }
//                if (gotoNextPossibleValue)
//                    break;
//            }
//
//            /// Iterate through the column
//            if (column != squareInQuestion.getCoordinate().getX()) {
//                for (int y = 0; y < 9; y++) {
//                    /// Cut it short if the PV is found in this column
//                    if (board.getValueAt(column, y) == possibleValue) {
//                        gotoNextPossibleValue = true;
//                        break;
//                    }
//                }
//                if (gotoNextPossibleValue)
//                    break;
//            }
//        }
//        /// If not the right value
//        if (gotoNextPossibleValue) {
//            continue;
//        /// It is the right value, so fill it in!!!
//        } else {
//            squareInQuestion.setValue(possibleValue);
//            return true;
//        }
//    }
//
//    return false;
*/
}

bool isNotFoundInOthersPVs(int PVInQuestion, const vector<Square*>& otherSquares) {
    for (auto pOtherSquare : otherSquares) {
        for (auto otherPV : *pOtherSquare->getPossibleValues()) {
            if (PVInQuestion == otherPV)
                return false;
        }
    }
    return true;
}

bool isSolved(Board board) {
    Coordinate coordInQuestion;
    for (coordInQuestion.setY(0);
         coordInQuestion.getY() < 9 ;
         coordInQuestion.getY()++    ) {
        for (coordInQuestion.setX(0);
             coordInQuestion.getX() < 9 ;
             coordInQuestion.getX()++    ) {

//            cerr << board.getValueAt(coordInQuestion) << " ";

            /// Is not changeable?
            if (!(board.getSquareAt(coordInQuestion).isChangeable())) {
                continue;
            /// CoordInQuestion has empty value?
            } else if (board.getSquareAt(coordInQuestion).isEmpty()) {
//                cerr << endl;
                return false;
            }

            /// Check the row
            for (int x = 0; x < 9; x++) {
                if (x != coordInQuestion.getX() &&
                    board.getValueAt(x, coordInQuestion.getY()) ==
                    board.getValueAt(coordInQuestion)) {
//                    cerr << endl;
                    return false;
                }
            }

            /// Check the column
            for (int y = 0; y < 9; y++) {
                if (y != coordInQuestion.getY() &&
                    board.getValueAt(coordInQuestion.getX(), y) ==
                    board.getValueAt(coordInQuestion)) {
//                    cerr << endl;
                    return false;
                }
            }

            /// Check the box
            Coordinate boxStart = Board::getBoxStartFor(coordInQuestion);
            for (int y = boxStart.getY(); y < boxStart.getY() + 3; y++) {
                for (int x = boxStart.getX(); x < boxStart.getX() + 3; x++) {
                    if (Coordinate(x, y) != coordInQuestion &&
                        board.getValueAt(x, y) ==
                        board.getValueAt(coordInQuestion)) {
//                        cerr << endl;
                        return false;
                    }
                }
            }
        }
//        cerr << endl;
    }
//    cerr << endl;

    return true;
}
#endif
