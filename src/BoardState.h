#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <vector>
#include "Move.h"
#include "pieces/Piece.h"

class KingPiece;
class Piece;
class Move;

class BoardState {
public:
    std::vector<std::vector<Piece*>> board;

    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;
    KingPiece *whiteKing{};
    KingPiece *blackKing{};
    std::vector<Move> lastMoves;
    bool isWhiteTurn;

public:
    explicit BoardState(bool);

    // Do not allow copying
    BoardState(const BoardState& other) = delete;

    ~BoardState();

    // Gets whether there is a checkmate for the given color
    // Will call updateValidMoves() and getCheck() to determine this
    bool getCheckmate(bool white);

    // Returns KingPiece.checked for the current king
    // Does not update KingPiece.checked
    bool getCheck(bool white);

    // Gets whether pieces are attacking the king
    // Will update KingPiece.checked
    bool updateCheck(bool white);

    // Gets whether pieces are attacking the given location. If white == true we are seeing if there 
    // are black pieces which attack this tile
    bool getAttacked(int x, int y, bool white); 

    // Updates the validMoves vectors with the valid moves for the given color
    // Will call Piece::getPieceMoves(board) which returns a vector of moves
    void updateValidMoves(bool white);

    // Moves the piece with the given move
    bool movePiece(const Move& move);

    //moves piece only if it is a legal move
    bool movePieceIfLegal(const Move& move);

    // Sets the piece at the given location
    void setPiece(Piece *piece, int x, int y);

    // Removes the piece at the given location
    void removePiece(int x, int y);

    // Undoes the last move
    void undo();

    // Returns whether there are exactly one white and one black king,
    // so the game can start
    bool canStartGame() const;

    // Returns all valid moves for the current player
    // Concatenates all validMoves vectors for the current player
    std::vector<Move> allValidMoves() const;

    static Piece* makePiece(char, int, int);

};

#endif 
