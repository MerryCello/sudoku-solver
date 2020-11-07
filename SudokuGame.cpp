//
// Created by kevin on 7/17/2020.
//

#include <iostream>
#include <vector>
#include "definitions.h"
#include "SudokuGame.h"
using namespace std;

void SudokuGame::promptFilePath(char *filePath) {
#ifdef DEBUG
    cout << "Where is your board located? " << filePath << endl;
#else
    cout << "Where is your board located? ";
    cin >> filePath;
#endif
}

void SudokuGame::play() {
    char choice;
    Coordinate coord;
    displayInstructions();
    cout << endl;
    cout << board;

    while(true) {
        cout << "\n> ";
        cin >> choice;
        switch (choice) {
            case '?':
                displayInstructions();
                cout << endl;
                break;
            case 'd':
            case 'D':
                cout << board;
                break;
            case 'e':
            case 'E':
               board.promptAndSetValueFor(promptCoordinate());
                break;
            case 'p':
            case 'P':
                coord = promptCoordinate();
                board.findPossibleValuesAt(coord);
                displayPossibleValues(coord, board.getSquareAt(coord).getPossibleValues());
                break;
            case 's':
            case 'S':
                solve();
                break;
            case 'q':
            case 'Q': {
#ifndef DEBUG
                char filename[256];
                cout << "What file would you like to write your board to: ";
                cin >> filename;
                board.saveToFile(filename);
#endif
                return;
            }
            default:
                cin.ignore('\n', 256);
                cin.clear();
                cout << "ERROR: Invalid option.\n";
                break;
        }
    }
}

void SudokuGame::displayInstructions() {
    cout << "Options:\n"
         << "   ?  Show these instructions\n"
         << "   D  Display the board\n"
         << "   E  Edit one square\n"
         << "   P  Possible values for a square\n"
         << "   S  Solve the Sudoku\n"
         << "   Q  Save and Quit\n";
}

void SudokuGame::displayPossibleValues(Coordinate coord, vector<int> *values) {
    if (values == nullptr)
        return;

    cout << "The possible values for \'" << coord << "\' are: ";
    for (auto value = values->begin(); value != values->end(); value++) {
        if (value == values->begin())
            cout << *value;
        else
            cout << ", " << *value;
    }
//    for(int firstTrueValue = 1; firstTrueValue <= 9; firstTrueValue++) {
//        if (values[firstTrueValue]) {
//            cout << firstTrueValue;
//            for(int value = firstTrueValue+1; value <= 9; value++) {
//                if(values[value])
//                    cout << ", " << value;
//            }
//            break;
//        }
//    }
    cout << endl;
}

Coordinate SudokuGame::promptCoordinate() {
    Coordinate coord;
    do {
        cin.clear();
        cout << "What are the coordinates of the square: ";
    } while(!(cin >> coord));
    return coord;
}

void SudokuGame::solve() {
   Board savedBoard;
   do {
      savedBoard = board;

      /// Iterate through each box of board
      bool endOfBoard = false;
      for (Coordinate boxStart(0, 0); !endOfBoard; nextBoxStart(&boxStart)) {

         /// Iterate through the box
         Coordinate coord;
         for (coord.setY(boxStart.getY()); coord.getY() < boxStart.getY()+3; coord.getY()++) {
            for (coord.setX(boxStart.getX()); coord.getX() < boxStart.getX()+3; coord.getX()++) {

               if (board.getSquareAt(coord).isEmpty()) {
                  board.findPossibleValuesAt(coord);

                  /// fill in single PVs
                  if (fillSinglePVFor(&(board.getSquareAt(coord))))
                     continue;

                  /// single out and fill PVs by process of elimination
                  if (fillByEliminationFor(&(board.getSquareAt(coord))))
                     continue;
               }
            }
         }
         endOfBoard = (coord.getX() == 9 && coord.getY() == 9);
      }
   } while(!isSolved(board) && savedBoard != board);

   if (!isSolved(board) && savedBoard == board)
      cout << "\nThe Sudoku was not solved...\n"
           << "Something is wrong with the algorithm...\n";
   else if (isSolved(board))
      cout << "\nThe Sudoku is solved!\n";
}

void SudokuGame::nextBoxStart(Coordinate *c) {
   c->getX() = (c->getX() + 3) % 9;
   c->getY() += (c->getX() ? 0 : 3);
}

bool SudokuGame::fillSinglePVFor(Square *squareInQuestion) {
   vector<int>* possibleValues = squareInQuestion->getPossibleValues();
   if (possibleValues->size() == 1) {
      squareInQuestion->setValue((*possibleValues)[0]);
      return true;
   }
   return false;
}

bool SudokuGame::fillByEliminationFor(Square *squareInQuestion) {
   Coordinate boxStart = Board::getBoxStartFor(squareInQuestion->getCoordinate());
   vector<Square*> otherEmptySquares;

   /// Step 1
   for (int y = boxStart.getY(); y < boxStart.getY()+3; y++) {
      for (int x = boxStart.getX(); x < boxStart.getX()+3; x++) {
         Square* pSquare = &(board.getSquareAt(Coordinate(x, y)));
         if (pSquare->isEmpty() && pSquare != squareInQuestion) {
            board.findPossibleValuesAt(pSquare->getCoordinate());
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
}

bool SudokuGame::isNotFoundInOthersPVs(int PVInQuestion, const vector<Square*>& otherSquares) {
   for (auto pOtherSquare : otherSquares) {
      for (auto otherPV : *pOtherSquare->getPossibleValues()) {
         if (PVInQuestion == otherPV)
            return false;
      }
   }
   return true;
}

bool SudokuGame::isSolved(Board board) {
   Coordinate coordInQuestion;
   for (coordInQuestion.setY(0);
        coordInQuestion.getY() < 9 ;
        coordInQuestion.getY()++    ) {
      for (coordInQuestion.setX(0);
           coordInQuestion.getX() < 9 ;
           coordInQuestion.getX()++    ) {

         /// Is not changeable?
         if (!(board.getSquareAt(coordInQuestion).isChangeable())) {
            continue;
            /// CoordInQuestion has empty value?
         } else if (board.getSquareAt(coordInQuestion).isEmpty()) {
            return false;
         }

         /// Check the row
         for (int x = 0; x < 9; x++) {
            if (x != coordInQuestion.getX() &&
                board.getValueAt(x, coordInQuestion.getY()) ==
                board.getValueAt(coordInQuestion)) {
               return false;
            }
         }

         /// Check the column
         for (int y = 0; y < 9; y++) {
            if (y != coordInQuestion.getY() &&
                board.getValueAt(coordInQuestion.getX(), y) ==
                board.getValueAt(coordInQuestion)) {
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
                  return false;
               }
            }
         }
      }
   }

   return true;
}

