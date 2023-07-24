#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <vector>
#include "Move.h"
#include "KingPiece.h"
#include "Piece.h"

using namespace std;

class BoardState {
    bool canStart; // true if there are exactly one white and one black king
public:
    Piece *board[8][8];

    vector<Piece *> whitePieces;
    vector<Piece *> blackPieces;
    KingPiece *whiteKing;
    KingPiece *blackKing;
    Move lastMove;

public:
    BoardState();

    ~BoardState();

    // Gets whether there is a checkmate for the given color
    // Will call updateValidMoves() and getCheck() to determine this
    bool getCheckmate(bool white);

    // Gets whether pieces are attacking the king
    // Will update KingPiece.checked
    bool getCheck(bool white);

    // Gets whether pieces are attacking the given location. If white == true we are seeing if there 
    // are white pieces which attack this tile
    bool getAttacked(int x, int y, bool white); 

    // Updates the validMoves vectors with the valid moves for the given color
    // Will call Piece::getPieceMoves(board) which returns a vector of moves
    void updateValidMoves(bool white);

    // Moves the piece with the given move
    bool movePiece(Move move);

    // Sets the piece at the given location
    void setPiece(Piece *piece, int x, int y);

    // Removes the piece at the given location
    void removePiece(int x, int y);

    // Returns whether there are exactly one white and one black king,
    // so the game can start
    bool canStartGame();
};

#endif 
