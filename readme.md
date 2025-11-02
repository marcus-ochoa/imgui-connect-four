Using the provided code that can be downloaded from this github add a new class that inherits from game.cpp in the same way TicTacToe.cpp does and implement a working version of the game Connect 4. The game should be added as a fourth choice from the menu so the choices are Tic Tac Toe, Checkers, Othello and now Connect 4.

The game must be playable by both by 2 people and vs. an AI. Your implementation must check for the winning condition, and display that on the right hand side the same way the current setup does for tic-tac-toe. The stopGame() method is expected to clean up and delete any memory allocated.

Look at the new Grid.cpp class and see how it is used in Checkers.cpp for a good understanding about laying out a useable grid.

For extra credit, when a game piece is added to the board, make it animate into place instead of just appearing in it's final spot.

Graphic files are provided for the game pieces called yellow.png and red.png.

For the submission, create a new github based on the above code-base and provide a link to that along with a complete readme.md on how your implementation works.


## Changes made to allow compilation after getting code
Changed cmake line like last time\
Changed Sprite.cpp line like last time\
Added include in Game.cpp for use of std::fabs\
Added include in Grid.cpp for use of std::find\
Added include in Sprite.h for use of intptr_t\

