#ifndef PAWNPIECE_H
#define PAWNPIECE_H

#include <vector>
#include "Move.h"
#include "BoardState.h"

using namespace std;

class PawnPiece : public Piece {
public:
    static const double value = 1;

    PawnPiece(int,int,bool);

    ~PawnPiece();

    vector <Move> getPieceMoves(BoardState &board) override;

    bool isAttacking(int x, int y, BoardState &board) override;
};

#endif
