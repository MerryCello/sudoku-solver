//
// Created by kevin on 7/16/2020.
//

#ifndef SUDOKU_SQUARE_H
#define SUDOKU_SQUARE_H

#include <vector>
#include "definitions.h"
#include "Coordinate.h"

class Square {
private:
    int value;
    bool changeable;
    std::vector<int> possibleValues;
    Coordinate coordinate;
public:
    /// Constructors
    Square();
    Square(int value, bool isChangeable, Coordinate c);

    /// Getters
    bool isChangeable() const             { return changeable;      }
    bool isEmpty() const                  { return value == 0;      }
    Coordinate getCoordinate()            { return coordinate;      }
    std::vector<int>* getPossibleValues() { return &possibleValues; }

    /// Setters
    void setValue(int v) { value = (changeable && v >= 1 && v <= 9 ? v : value); }
    void setCoordinate(Coordinate coord) { coordinate = coord;                   }

    /**
     *
     * @param out
     * @param s
     * @return
     */
    friend std::ostream &
    operator << (std::ostream & out, Square & s);

    /**
     *
     * @param in
     * @param s
     * @return
     */
    friend std::istream &
    operator >> (std::istream & in, Square & s);

    /**
     *
     * @return
     */
    operator int() const { return value; }

    friend bool operator == (Square & lhs, int & rhs) { return lhs.value == rhs; }
    friend bool operator == (int & lhs, Square & rhs) { return lhs == rhs.value; }
//    friend bool operator == (Square & lhs, Square & rhs);
    friend bool operator != (Square & lhs, int & rhs) { return !(lhs == rhs);    }
    friend bool operator != (int & lhs, Square & rhs) { return !(lhs == rhs);    }
//    friend bool operator != (Square & lhs, Square & rhs);
};


#endif //SUDOKU_SQUARE_H
