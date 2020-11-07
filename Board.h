//
// Created by kevin on 7/16/2020.
//

#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include "definitions.h"
#include "Square.h"
#include "Coordinate.h"


class Board {
private:
    Square squares[9][9];
    bool readSuccessfully;
public:
    /// Constructors
    Board()                         { readSuccessfully = false;                      }
    explicit Board(char filePath[]) { readSuccessfully = getBoardFromFile(filePath); }

    /// Getters
    bool getBoardFromFile(char* filePath);
    Square & getSquareAt(Coordinate c) { return squares[c.getY()][c.getX()]; }
    int getValueAt(Coordinate c) { return squares[c.getY()][c.getX()];       }
    int getValueAt(int x, int y) { return squares[y][x];                     }
    bool hasReadSuccessfully() const { return readSuccessfully;              }
    static Coordinate getBoxStartFor(Coordinate c);
    bool findPossibleValuesAt(Coordinate coord);

    /// Setters
    bool promptAndSetValueFor(Coordinate coord);
    void setValueAt(Coordinate c, int value) { squares[c.getY()][c.getX()].setValue(value); }

    /**
     *
     * @param filePath
     * @return
     */
    bool saveToFile(char filePath[]);

    /// Friends
    friend std::ostream & operator << (std::ostream & out, Board & b       );
    friend      bool      operator == (Board & lhs, Board & rhs);
    friend      bool      operator != (Board & lhs, Board & rhs);
};


#endif //SUDOKU_BOARD_H
