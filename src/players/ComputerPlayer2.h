#ifndef COMPUTERPLAYER2_H
#define COMPUTERPLAYER2_H

#include "Player.h"
#include "../BoardState.h"

class ComputerPlayer2 : public Player {
public:
    explicit ComputerPlayer2(bool isWhite);

    ~ComputerPlayer2() override;

    MoveResult makeMove(BoardState &board) override;
};

#endif
