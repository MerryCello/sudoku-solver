//
// Created by kevin on 7/16/2020.
//

#include <iostream>
#include "definitions.h"
#include "Square.h"
using namespace std;


Square::Square(): value(0), changeable(true) {}

Square::Square(int value, bool isChangeable, Coordinate coord) :
        changeable(isChangeable),
        value(value),
        coordinate(coord) {}

ostream & operator << (ostream & out, Square & s) {
    out << s.value;
    return out;
}

istream & operator >> (istream & in, Square & s) {
    int value = 0;
    in >> value;
    if (value >= 0 && value <= 9) {
        s.value = value;
        s.changeable = s.isEmpty();
    } else { in.setstate(ios::failbit); }
    return in;
}
