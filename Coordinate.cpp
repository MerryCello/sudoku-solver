//
// Created by kevin on 7/13/2020.
//

#include <iostream>
#include "definitions.h"
#include "Coordinate.h"
using namespace std;

/**********************************************************
 * COORD::CHAR_TO_INY
 *
 **********************************************************/
int Coordinate::charToInt(char letter) {
    switch (letter) {
        case 'A': case 'B': case 'C': case 'D':case 'E': case 'F': case 'G':
        case 'H': case 'I':
            return letter - 65;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i':
            return letter - 97;
        default:
            return -1;
    }
}

/**********************************************************
 * << OPERATOR OVERLOAD
 *
 **********************************************************/
ostream & operator << (ostream & out, Coordinate c) {
    out << (char)(c.getX() + 65) << (c.getY() + 1);
    return out;
}

/**********************************************************
 * >> OPERATOR OVERLOAD
 *
 **********************************************************/
istream & operator >> (istream & in, Coordinate & c) {
    char first;
    char second;
    in >> first >> second;

    /// Case for input is something like a2 or A2
    if       (isalpha(first) &&
              Coordinate::charToInt(first) != -1 &&
              isdigit(second) && (second - 48) != 0) {
        c.setX(Coordinate::charToInt(first));
        c.setY((second - 49));

    /// Case for input is something like 2a or 2A
    } else if(isalpha(second) &&
              Coordinate::charToInt(second) != -1 &&
              isdigit(first) && (first - 48) != 0) {
        c.setX(Coordinate::charToInt(second));
        c.setY((first - 49));

    /// failed to input if coordinate don't make sense
    } else {
        cout << "ERROR: Coordinate \'" << first << second << "\' is invalid\n";
        in.setstate(ios::failbit);
    }

    return in;
}

bool operator!=(Coordinate lhs, Coordinate rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

bool operator==(Coordinate lhs, Coordinate rhs) {
    return !(lhs != rhs);
}
