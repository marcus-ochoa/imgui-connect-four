# Notes

## Changes made to allow compilation after getting code
Changed cmake line like last time\
Changed Sprite.cpp line like last time\
Added include in Game.cpp for use of std::fabs\
Added include in Grid.cpp for use of std::find\
Added include in Sprite.h for use of intptr_t\

## Approach
I used the tic-tac-toe and othello classes provided as a basis for the connect four logic. I used the provided grid and squares for showing game state and a state string for representing state in the back end. To check for winning conditions and conduct board evaluations I scanned the board in 4x4 windows and checked each winning line much like the winning triples in tic-tac-toe. This scanning approach was discussed in class. The negamax implementation is straightforward and stops at terminal states or once a max depth is reached. Non-terminal states are scored with a simple evaluation giving 3 points for each 1-stone sequence, 9 points for each 2-stone sequence, and 27 points for each 3-stone sequence (blocked sequences give 0 points). Moves are also ordered pioritizing columns by centrality. Overall the AI seems to perform decently, although it is considerably slow even with pruning. I currently run it with a max depth of 5 which I personally have difficulty beating. I imagine using a faster board state representation such as a bitboard would allow the algorithm to explore deeper in a shorter amount of time.
