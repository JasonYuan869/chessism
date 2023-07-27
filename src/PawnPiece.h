#ifndef PAWNPIECE_H
#define PAWNPIECE_H

#include <vector>
#include "Move.h"
#include "BoardState.h"



class PawnPiece : public Piece {
    void enPassant(BoardState &board,std::vector<Move>& moves) const;
    void addToMoveList(int,int,std::vector<Move>&) const;

public:
    static double value;

    PawnPiece(int,int,bool);

    ~PawnPiece();

    std::vector <Move> getPieceMoves(BoardState &board) const override;
    bool isAttacking(int x, int y, BoardState &board) const override;
    double getValue() override;

    Piece* promote();

};

#endif
