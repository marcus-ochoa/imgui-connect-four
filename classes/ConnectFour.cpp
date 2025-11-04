#include "ConnectFour.h"
#include <iostream>
#include <array>
#include <algorithm>

ConnectFour::ConnectFour()
{
    _grid = new Grid(7, 6);
}

ConnectFour::~ConnectFour()
{
    delete _grid;
}

Bit* ConnectFour::createPiece(Player* player) {
    Bit* bit = new Bit();
    bit->LoadTextureFromFile(player == getPlayerAt(RED_PLAYER) ? "red.png" : "yellow.png");
    bit->setOwner(player);
    return bit;
}

void ConnectFour::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 7;
    _gameOptions.rowY = 6;

    _gameOptions.AIMAXDepth = 5;

    _grid->initializeSquares(80, "square.png");

    if (gameHasAI()) {
        setAIPlayer(1);
    }

    startGame();
}

bool ConnectFour::actionForEmptyHolder(BitHolder &holder) {
    if (holder.bit()) return false;

    ChessSquare* square = static_cast<ChessSquare*>(&holder);
    int x = square->getColumn();
    int y = square->getRow();
    Player* currentPlayer = getCurrentPlayer();

    // Check for row placement
    for (int i = 5; i >= y; i--) {
        
        ChessSquare* currSquare = _grid->getSquare(x, i);

        if (!currSquare->bit()) {
            // Place the piece and move it down from the top square
            Bit* newPiece = createPiece(currentPlayer);
            newPiece->setPosition(_grid->getSquare(x, 0)->getPosition());
            currSquare->setBit(newPiece);
            newPiece->moveTo(currSquare->getPosition());
            break;
        }
    }

    endTurn();
    return true;
}

bool ConnectFour::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // You can't move anything in connect 4
    return false;
}

bool ConnectFour::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    // You can't move anything in connect 4
    return false;
}

//
// Free all the memory used by the game on the heap (just the grid squares)
//
void ConnectFour::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

//
// Helper function for the winner check
//
Player* ConnectFour::checkWindowForWinner(const int startX, const int startY)
{
    static const int winningLines[10][4] =  { {0,1,2,3}, {4,5,6,7}, {8,9,10,11}, {12,13,14,15}, // rows
                                                {0,4,8,12}, {1,5,9,13}, {2,6,10,14}, {3,7,11,15},  // cols
                                                {0,5,10,15}, {3,6,9,12} };     // diagonals

    for (int i = 0; i < 10; i++) {
        const int *line = winningLines[i];

        Player *player = nullptr;

        // Check line
        for (int j = 0; j < 4; j++) {
            int x = startX + (line[j] % 4);
            int y = startY + (line[j] / 4);

            Bit* bit = _grid->getSquare(x, y)->bit();
            Player *tmpPlayer = bit ? bit->getOwner() : nullptr;

            if ((!player) && (tmpPlayer)) player = tmpPlayer;

            if ((player) && (player == tmpPlayer)) continue;

            player = nullptr;
            break;
        }

        if (player) return player;
    }

    return nullptr;
}

Player* ConnectFour::checkForWinner()
{
    // Go through each window
    for (int startX = 0; startX < 4; startX++) {
        for (int startY = 0; startY < 3; startY++) {
            Player* player = checkWindowForWinner(startX, startY);
            if (player) return player;
        }
    }

    return nullptr;
}

bool ConnectFour::checkForDraw()
{
    bool isDraw = true;
    
    // Check to see if the board is full
    _grid->forEachSquare([&isDraw](ChessSquare* square, int x, int y) {
        if (!square->bit()) {
            isDraw = false;
        }
    });
    return isDraw;
}

//
// State strings
//
std::string ConnectFour::initialStateString()
{
    std::string state(42, '0');
    return state;
}

//
// This still needs to be tied into imguis init and shutdown
// We will read the state string and store it in each turn object
//
std::string ConnectFour::stateString()
{
    std::string s = initialStateString();
    int index = 0;
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        if (index < s.length()) {
            Bit* bit = square->bit();
            if (!bit) {
                s[index] = '0';
            } else if (bit->getOwner() == getPlayerAt(RED_PLAYER)) {
                s[index] = '1';
            } else {
                s[index] = '2';
            }

            index++;
        }
    });
    return s;
}

//
// This still needs to be tied into imguis init and shutdown
// When the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void ConnectFour::setStateString(const std::string &s)
{
    if (s.length() != 42) return;

    int index = 0;
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        if (index < s.length()) {
            char pieceType = s[index];
            square->destroyBit();

            if (pieceType == '1') {
                Bit* piece = createPiece(getPlayerAt(RED_PLAYER));
                piece->setPosition(square->getPosition());
                square->setBit(piece);
            } else if (pieceType == '2') {
                Bit* piece = createPiece(getPlayerAt(YELLOW_PLAYER));
                piece->setPosition(square->getPosition());
                square->setBit(piece);
            }
            index++;
        }
    });
}

int ConnectFour::findMoveForColumn(const std::string& state, int col)
{
    for (int row = 5; row >= 0; row--) {
        int index = row * 7 + col;
        if (state[index] == '0') {
            return index;
        }
    }

    return -1;
}

//
// Function called by the AI
//
void ConnectFour::updateAI() 
{
    int bestVal = -1000;
    BitHolder* bestMove = nullptr;
    std::string state = stateString();

    // Traverse all cells, evaluate minimax function for all empty cells
    for (int col = 0; col < 7; col++) {
        int index = findMoveForColumn(state, col);
        if (index < 0) continue;

        // Make the move
        state[index] = '2';
        int moveVal = -negamax(state, 0, -1000, +1000, HUMAN_PLAYER);
        // Undo the move
        state[index] = '0';
        // If the value of the current move is more than the best value, update best
        if (moveVal > bestVal) {
            bestMove = _grid->getSquareByIndex(index);
            bestVal = moveVal;
        }
    }

    // Make the best move
    if (bestMove) {
        actionForEmptyHolder(*bestMove);
    }
}

bool ConnectFour::isAIBoardFull(const std::string& state) {
    return state.find("0") == std::string::npos;
}

int ConnectFour::evaluateAIBoard(const std::string& state, int playerColor, int depth) {
    
    static const int winningLines[10][4] =  { {0,1,2,3}, {4,5,6,7}, {8,9,10,11}, {12,13,14,15}, // rows
                                                {0,4,8,12}, {1,5,9,13}, {2,6,10,14}, {3,7,11,15},  // cols
                                                {0,5,10,15}, {3,6,9,12} };     // diagonals

    char playerNum = playerColor == HUMAN_PLAYER ? '2' : '1'; // Evaluate for the opposite of passed in color

    int score = 0;

    // Scan the board in 4x4 windows
    for (int startX = 0; startX < 4; startX++) {
        for (int startY = 0; startY < 3; startY++) {

            // Evaluate window
            for (int i = 0; i < 10; i++) {
                const int *line = winningLines[i];

                int count = 0;

                for (int j = 0; j < 4; j++) {
                    int x = startX + (line[j] % 4);
                    int y = startY + (line[j] / 4);

                    int index = y * 7 + x;

                    char tmpPlayer = state[index];
                    if (tmpPlayer == playerNum) count++;

                    // If you are blocked, receive no score
                    if (tmpPlayer != playerNum && tmpPlayer != '0') {
                        count = 0;
                        break;
                    }
                }

                // Scoring
                if (count == 4) {
                    score = 900 - depth; // Subtract by depth to prioritize closer wins / losses
                    return score;
                }
                if (count == 3) score += 27;
                if (count == 2) score += 9;
                if (count == 1) score += 3;
            }
        }
    }

    return score;
}

int ConnectFour::negamax(std::string& state, int depth, int alpha, int beta, int playerColor)
{
    int bestVal = -1000;
    int boardScore = evaluateAIBoard(state, playerColor, depth);

    if (boardScore >= 800) {
        // Returning the value to the recursion above, so negate it
        // because it is the opposite of what we want
        return -boardScore;
    }
    bool boardFull = isAIBoardFull(state);

    if (boardFull) {
        // Draw
        return 0;
    }

    if (depth > _gameOptions.AIMAXDepth) {
        return -boardScore;
    }

    static const int colOrder[7] = {3, 2, 4, 1, 5, 0, 6};
    int moves[7] = {0, 0, 0, 0, 0, 0, 0};

    // Order moves prioritizing central columns
    for (int i = 0; i < 7; i++) {

        int col = colOrder[i];
        int index = findMoveForColumn(state, col);
        moves[i] = index;
    }

    // Traverse all cells, evaluate minimax function for all empty cells
    for (int col = 0; col < 7; col++) {

        int index = moves[col];
        if (index < 0) continue;

        // Make the move
        state[index] = playerColor == HUMAN_PLAYER ? '1' : '2';
        int moveVal = -negamax(state, depth + 1, -beta, -alpha, -playerColor);
        // Undo the move
        state[index] = '0';

        // If the value of the current move is more than the best value, update best
        if (moveVal > bestVal) {
            bestVal = moveVal;
        }

        // Alpha-beta pruning
        if (bestVal > alpha) {
            alpha = bestVal;
        }
        if (alpha >= beta) {
            break;
        }
    }

    return bestVal;
}