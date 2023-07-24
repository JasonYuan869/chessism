#ifndef QUEENPIECE_H
#define QUEENPIECE_H

#include <vector>
#include "Move.h"
#include "BoardState.h"

using namespace std;

class QueenPiece : public Piece {
public:
    static const double value = 9;

    QueenPiece(int,int,bool);

    ~QueenPiece();

    vector <Move> getPieceMoves(BoardState &board) override;

    bool isAttacking(int x, int y, BoardState &board) override;
};

#endif
