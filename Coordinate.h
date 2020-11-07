//
// Created by kevin on 7/11/2020.
//

#ifndef SUDOKU_COORDINATE_H
#define SUDOKU_COORDINATE_H

#include "definitions.h"

/**
 * @private x Corresponds to the letter values or column headers
 * @private y Corresponds to the number values or row numberings
 * @private chartToInt(c:char) Converts the letter to numbers for computing
 * @public Coordinate()
 * @public Coordinate(x:, y)
 * @public getX()
 * @public getY()
 * @public setX(dx)
 * @public setY(dy)
 * @friend operator <<(out, c)
 * @friend operator >>(in, c)
 */
class Coordinate {
private:
    int x;
    int y;
    static int charToInt(char c);

public:
    Coordinate(): x(0), y(0) {}
    Coordinate(int x, int y): x(x), y(y) {}

    int & getX() { return x; }
    int & getY() { return y; }

    void setX(int dx) { this->x = dx; }
    void setY(int dy) { this->y = dy; }

    /// operator overload
    /// @param out The output of the operator
    /// @param c   The coordinate to be outputted
    friend std::ostream & operator << (std::ostream & out, Coordinate c);

    /// operator overload
    /// @param in The input of the operator
    /// @param c  The coordinate to be inputted into. Takes formats such as a2, 2a, A2, or 2A
    friend std::istream & operator >> (std::istream & in, Coordinate & c);
    friend bool operator != (Coordinate lhs, Coordinate rhs);
    friend bool operator == (Coordinate lhs, Coordinate rhs);
};

#endif //SUDOKU_COORDINATE_H
