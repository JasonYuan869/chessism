#ifndef KNIGHTPIECE_H
#define KNIGHTPIECE_H

#include <vector>
#include "Move.h"
#include "BoardState.h"

using namespace std;

class KnightPiece : public Piece {
public:
    static const double value = 2.7;

    KnightPiece(int,int,bool);

    ~KnightPiece();

    vector <Move> getPieceMoves(BoardState &board);

    bool isAttacking(int x, int y, BoardState &board);
};

#endif
