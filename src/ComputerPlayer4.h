#ifndef COMPUTERPLAYER4_H
#define COMPUTERPLAYER4_H

#include "Player.h"
#include "BoardState.h"

class ComputerPlayer4 : public Player {
    public: 
        ComputerPlayer4(bool isWhite);
        ~ComputerPlayer4();
        int makeMove(BoardState& board) override;
        int evaluateBoard(BoardState& board);
        int minimax(int depth, BoardState& board, bool isMaximizingPlayer);
};

#endif
