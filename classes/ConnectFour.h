#pragma once
#include "Game.h"

//
// connect four game
//

//
// the main game class
//
class ConnectFour : public Game
{
public:
    ConnectFour();
    ~ConnectFour();

    // set up the board
    void        setUpBoard() override;

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder &holder) override;
    bool        canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool        canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void        stopGame() override;

	void        updateAI() override;
    bool        gameHasAI() override { return true; }
    Grid* getGrid() override { return _grid; }
private:
    // Player constants
    static const int RED_PLAYER = 0;
    static const int YELLOW_PLAYER = 1;

    //Bit *       PieceForPlayer(const int playerNumber);
    Bit *       createPiece(Player* player);
    Player*     checkWindowForWinner(int startX, int startY);
    int         negamax(std::string& state, int depth, int alpha, int beta, int playerColor);

    bool isAIBoardFull(const std::string& state);
    int evaluateAIBoard(const std::string& state, int playerColor, int depth);
    int findMoveForColumn(const std::string& state, int col);

    Grid*       _grid;
};

