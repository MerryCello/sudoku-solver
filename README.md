# Sudoku Solver
Based off the traditional [Sudoku][wiki-sudoku] puzzle. Built in C++ 14, it comes with a game and a debug mode. In the game mode, you can play it like normal, and if you get stuck or lazy, you can run the "solve" option to fill the board. Built with [object oriented programming][wiki-OOP] in mind.

## At Runtime
### Game Mode
![][game-play-1]

![][game-play-2]

## Requirements
1. JetBrain's [Clion][get-clion] to build the cmake files and run
2. A [C++ compiler][get-MinGW]

## Debug Mode
Comment in the DEBUG macro in [definitions.h][defs]

1. Allows for multiple board file to be run from the program arguments
2. Will show debug messages

[wiki-sudoku]: https://en.wikipedia.org/wiki/Sudoku
[wiki-OOP]: https://www.freecodecamp.org/news/object-oriented-programming-concepts-21bb035f7260/
[game-play-1]: ./docs/game-play.png
[game-play-2]: ./docs/game-play-finished.png
[get-clion]: https://www.jetbrains.com/clion/
[get-MinGW]: https://osdn.net/projects/mingw/releases/
[defs]: ./definitions.h