#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include "BoardState.h"
#include "Piece.h"

class Move {
public:
    std::pair<int, int> to;
    std::pair<int, int> from;
    char promotion;

    Piece* capturedOrMovedPiece;


    //rookTo and rookFrom are only used for castling, otherwise they are (-1,-1).
    std::pair<int, int> rookTo;
    std::pair<int, int> rookFrom;
    bool isCastle;

    std::pair<int, int> getTo() const;

    std::pair<int, int> getFrom() const;

    Move(); // default, we need for computerPlayer4

    //move with no capture
    Move(std::pair<int, int> to, std::pair<int, int> from, char promotion = '-');
    
    //move with a capture
    Move(std::pair<int, int> to, std::pair<int, int> from, Piece* capturedOrMovedPiece, char promotion = '-');
    
    //castling
    Move(std::pair<int, int> to, std::pair<int, int> rookTo, std::pair<int, int> from, std::pair<int, int> rookFrom, Piece* capturedOrMovedPiece);

    // Does not delete lastMove or capturedPiece as they are not owned by this object
    ~Move();
    bool sameMoveAs(Move& other) const;

};

#endif
