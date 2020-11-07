//
// Created by kevin on 7/16/2020.
// Changing console color see https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c#answer-4053879
//

#include <iostream>
#include <fstream>
#include <windows.h>
#include "definitions.h"
#include "Board.h"
using namespace std;

#define YELLOW 14
#define RESET 7


bool Board::getBoardFromFile(char *filePath) {
    ifstream fin(filePath);

    if (fin.fail()) {
        cout << "ERROR: Failed to open file \"" << filePath << ".\"\n";
        return false;
    }

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {//auto & square : squares[y]) {
            if(fin.fail()) {
                cout << "ERROR: Invalid number in file \""
                     << filePath << "\":" << y << endl;
            }
            fin >> squares[y][x];
            squares[y][x].setCoordinate(Coordinate(x, y));
        }
    }

    fin.close();

    return true;
}

Coordinate Board::getBoxStartFor(Coordinate c) {
    return {(c.getX() / 3) * 3, (c.getY() / 3) * 3};
}

bool Board::findPossibleValuesAt(Coordinate coord) {
    Square* pSquare = &getSquareAt(coord);
    bool possibleValues[10];
    possibleValues[0] = false;
    for (int i = 1; i <= 9; i++)
        possibleValues[i] = true;

    if(!(pSquare->isChangeable())) {
        cout << "ERROR: Square \'" << coord << "\' is filled\n";
        return false;
    }

    /// Check the row
    for(int x = 0; x < 9; x++) {
        if (x != coord.getX() && getValueAt(x, coord.getY()))
            possibleValues[getValueAt(x, coord.getY())] = false;
//            (pSquare->getPossibleValues())[getValueAt(x, coord.getY())] = false;
    }

    /// Check the column
    for(int y = 0; y < 9; y++) {
        if (y != coord.getY() && getValueAt(coord.getX(), y))
            possibleValues[getValueAt(coord.getX(), y)] = false;
//            (pSquare->getPossibleValues())[getValueAt(coord.getX(), y)] = false;
    }

    /// Check the box
    Coordinate boxStart = getBoxStartFor(coord);
    for(int y = boxStart.getY(); y < boxStart.getY() + 3; y++) {
        for(int x = boxStart.getX(); x < boxStart.getX() + 3; x++) {
            if ((x != coord.getX() || y != coord.getY()) && getValueAt(x, y))
                possibleValues[getValueAt(x, y)] = false;
//                (pSquare->getPossibleValues())[getValueAt(x, y)] = false;
        }
    }

    /// Save found possible values in square
    pSquare->getPossibleValues()->clear();
    for (int possibleValue = 1; possibleValue <= 9; possibleValue++) {
        if (possibleValues[possibleValue])
            pSquare->getPossibleValues()->push_back(possibleValue);
    }

    return true;
}

bool Board::promptAndSetValueFor(Coordinate coord) {
    int value = 0;
    Square* pSquare = &getSquareAt(coord);

    /// Check if board is filled
    if(pSquare->isChangeable()) {
        cout << "What is the value at \'" << coord << "\': ";
        cin.clear();
        cin >> value;

        for(auto possibleValue : *pSquare->getPossibleValues()) {
            if (value >= 1 && value <= 9 &&
                value == possibleValue     ) {
                pSquare->setValue(value);
                return true;
            }
        }
        cout << "ERROR: Value \'" << value << "\' in square \'" << coord << "\' is invalid\n";
    } else {
        cout << "ERROR: Square \'" << coord << "\' is filled\n";
    }
    return false;
}

bool Board::saveToFile(char filePath[]) {
    ofstream fout(filePath);

    if(fout.fail()) {
        cout << "ERROR: Failed to open file \"" << filePath << ".\"\n";
        return false;
    }

    for(int y = 0; y < 9; y++) {
        for(int x = 0; x < 9; x++) {
            fout << squares[y][x];
            if (x == 8 && y != 8) {
                fout << endl;
            } else if (x != 8) {
                fout << " ";
            }
        }
    }

    fout.close();
    cout << "Board written successfully\n";
    return true;
}

std::ostream & operator << (std::ostream & out, Board & board) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    out << "   A B C D E F G H I";
    for (int y = 0; y < 9; y++) {
        out << endl << y + 1 << "  ";

        for (int x = 0; x < 9; x++) {
            if (board.getSquareAt(Coordinate(x, y)).isChangeable()) {
                SetConsoleTextAttribute(hConsole, YELLOW);
                out << (board.getValueAt(x, y) ? (char)(board.getValueAt(x, y)+'0') : (char)' ');
                SetConsoleTextAttribute(hConsole, RESET);
            }
            else { out << board.getValueAt(x, y); }

            if (x != 8)
                out << (x == 2 || x == 5 ? "|" : " ");
        }
        if (y == 2 || y == 5)
            out << "\n   -----+-----+-----";
    }
    out << endl;
    return out;
}

bool operator == (Board & lhs, Board & rhs) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (lhs.getValueAt(x, y) != rhs.getValueAt(x, y))
                return false;
        }
    }
    return true;
}

bool operator != (Board & lhs, Board & rhs) {
    return !(lhs == rhs);
}
