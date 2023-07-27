#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include "BoardState.h"

class HumanPlayer : public Player {
    public: 
        HumanPlayer(bool isWhite);
        int makeMove(BoardState& board) override;
};

#endif