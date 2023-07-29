#include "BoardState.h"
#include "PawnPiece.h"
#include "RookPiece.h"
#include "KnightPiece.h"
#include "BishopPiece.h"
#include "QueenPiece.h"
#include "KingPiece.h"
#include <vector>

using namespace std;

BoardState::BoardState(bool isWhiteTurn) : canStart{false},isWhiteTurn{isWhiteTurn} {
    board = vector<vector<Piece*>>(8,vector<Piece*>(8,nullptr));


    // Initialize the board
    board[0][0] = makePiece('R', 0, 0);
    board[0][1] = makePiece('N', 0, 1);
    board[0][2] = makePiece('B', 0, 2);
    board[0][3] = makePiece('Q', 0, 3);
    board[0][4] = makePiece('K', 0, 4);
    board[0][5] = makePiece('B', 0, 5);
    board[0][6] = makePiece('N', 0, 6);
    board[0][7] = makePiece('R', 0, 7);

    for (int i = 0; i < 8; i++) {
        board[1][i] = makePiece('P', 1, i);
    }

    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }

    for (int i = 0; i < 8; i++) {
        board[6][i] = makePiece('p', 6, i);
    }

    board[7][0] = makePiece('r', 7, 0);
    board[7][1] = makePiece('n', 7, 1);
    board[7][2] = makePiece('b', 7, 2);
    board[7][3] = makePiece('q', 7, 3);
    board[7][4] = makePiece('k', 7, 4);
    board[7][5] = makePiece('b', 7, 5);
    board[7][6] = makePiece('n', 7, 6);
    board[7][7] = makePiece('r', 7, 7);

    // Initialize the king pointers
    whiteKing = dynamic_cast<KingPiece *>(board[0][4]);
    blackKing = dynamic_cast<KingPiece *>(board[7][4]);

    // Initialize the piece vectors
    for (int i = 0; i < 8; i++) {
        whitePieces.push_back(board[0][i]);
        whitePieces.push_back(board[1][i]);

        blackPieces.push_back(board[6][i]);
        blackPieces.push_back(board[7][i]);
    }
}

BoardState::~BoardState() {
    // Delete pieces
    for (auto& piece : whitePieces) {
        delete piece;
    }
}

bool BoardState::getCheckmate(bool white) {
    KingPiece* king = white ? whiteKing : blackKing;

    // Update the valid moves for each piece
    updateValidMoves(white);

    if (!king->checked) {
        return false;
    }

    for (auto& piece : white ? whitePieces : blackPieces) {
        if (!piece->isAlive) {
            continue;
        }

        if (!piece->validMoves.empty()) {
            return false;
        }
    }
    return true;
}

bool BoardState::getCheck(bool white) {
    KingPiece* king = white ? whiteKing : blackKing;

    // Mutates the king's checked variable
    king->checked = getAttacked(king->position_x, king->position_y, !white);
    return king->checked;
}

bool BoardState::getAttacked(int x, int y, bool white) {
    for (auto& piece : white ? blackPieces : whitePieces) {
        if (piece->isAttacking(x, y, *this)) {
            return true;
        }
    }
    return false;
}

void BoardState::setPiece(Piece *piece, int x, int y) {
    if (board[y][x] != nullptr) {
        board[y][x]->isAlive = false;
    }

    board[y][x] = piece;
    piece->position_x = x;
    piece->position_y = y;

    bool isWhite = piece->isWhite;
    if (isWhite) {
        whitePieces.push_back(piece);
    } else {
        blackPieces.push_back(piece);
    }
}

void BoardState::removePiece(int x, int y) {
    if (board[y][x] != nullptr) {
        board[y][x]->isAlive = false;
    }

    board[y][x] = nullptr;
}

void BoardState::undo() {
    if (lastMoves.empty()){
        return;
    }
    //toggle the turn so that the logic is more similar to movePiece
    //we will undo movePiece in the opposite order compared to the order that
    //movePiece was done in 
    isWhiteTurn = !isWhiteTurn;
    bool lastTurnIsWhite = isWhiteTurn;
    Move lastMove = lastMoves.back(); 
    lastMoves.pop_back();

    int startx = lastMove.from.first;
    int starty = lastMove.from.second;

    int endx = lastMove.to.first;
    int endy = lastMove.to.second;

    //castling logic 
    //
    if (lastMove.isCastle){
        int rookStartx = lastMove.rookFrom.first;
        int rookStarty = lastMove.rookFrom.second;
        int rookEndx = lastMove.rookTo.first;
        int rookEndy = lastMove.rookTo.second;
        board[rookStarty][rookStartx] = board[rookEndy][rookEndx];
        board[rookEndy][rookEndx] = nullptr;
        board[rookEndy][rookEndx]->setPosition(rookStartx,rookStarty);
    }

    if (lastMove.promotion != '-'){
        board[endy][endx] = nullptr;
        if(lastTurnIsWhite){
            whitePieces.pop_back();
        } else {
            blackPieces.pop_back();
        }

        Piece* pawn = nullptr;
        if (lastTurnIsWhite){
            for (auto piece : whitePieces){
                if (!piece->isAlive && piece->getPosition() == lastMove.from){
                    pawn = piece;
                    break;
                }
            }
        } else {
            for (auto piece : blackPieces){
                if (!piece->isAlive && piece->getPosition() == lastMove.from){
                    pawn = piece;
                    break;
                }
            }
        }
        pawn->isAlive = true;
        board[starty][startx] = pawn;
    } else {
        Piece* pieceThatMoved = board[endy][endx];
        board[starty][startx] = pieceThatMoved;
        pieceThatMoved->setPosition(startx,starty);
    }

    board[endy][endx] = nullptr;

    if (!lastMove.isCastle){
        lastMove.capturedOrMovedPiece->isAlive = true;
        int aliveX = lastMove.capturedOrMovedPiece->position_x;
        int aliveY = lastMove.capturedOrMovedPiece->position_y;
        board[aliveY][aliveX] = lastMove.capturedOrMovedPiece;
    }

}

void BoardState::updateValidMoves(bool white) {

    for (auto& piece : white ? blackPieces : whitePieces) {
        vector<Move> moves;

        for (auto& move : piece->getPieceMoves(*this)) {
            // Simulate the move
            movePiece(move);

            // Is our king checked?
            if (!getCheck(white)) {
                // Add the move to the vector of moves
                moves.push_back(move);
            }

            // Undo the move
            undo();
        }

        // Update the validMoves vector
        piece->validMoves = moves;
    }
}

bool BoardState::movePiece(const Move& move) {
    int x = move.from.first;
    int y = move.from.second;
    Piece* pieceToMove = board[y][x];
    bool moveIsValid = false;

    if (pieceToMove == nullptr){
        return false;
    }

    for (auto validMove : pieceToMove->validMoves){
        if (move == validMove){
            moveIsValid = true;
            break;
        }
    }

    if (!moveIsValid){
        return false;
    }

    int to_x = move.to.first;
    int to_y = move.to.second;

    //handle the capture
    if (!move.isCastle){
       move.capturedOrMovedPiece->isAlive = false;
       board[to_y][to_x] = nullptr;
    }


    // either pawn promotion, or we actually move the piece
    if (move.promotion != '-'){
        pieceToMove->isAlive = false;
        board[y][x] = nullptr;
        board[to_y][to_x] = BoardState::makePiece(move.promotion,to_x,to_y);
        if (isWhiteTurn){
            whitePieces.push_back(board[to_y][to_x]);
        } else {
            blackPieces.push_back(board[to_y][to_x]);
        }
    } else {
        board[to_y][to_x] = pieceToMove;
        pieceToMove->setPosition(to_x,to_y);
    }

    board[y][x] = nullptr;
    //castling
    if (move.isCastle){
        int rookStartx = move.rookFrom.first;
        int rookStarty = move.rookFrom.second;
        int rookEndx = move.rookTo.first;
        int rookEndy = move.rookTo.second;
        board[rookEndy][rookEndx] = board[rookStarty][rookStartx];
        board[rookStarty][rookStartx] = nullptr;
        board[rookEndy][rookEndx]->setPosition(rookEndx,rookEndy);
    }

    return true;
}

bool BoardState::canStartGame() const {
    return canStart;
}


Piece *BoardState::makePiece(char piece, int y, int x) {
    switch (piece) {
        case 'P':
            return new PawnPiece(x, y, true);
        case 'p':
            return new PawnPiece(x, y, false);
        case 'R':
            return new RookPiece(x, y, true);
        case 'r':
            return new RookPiece(x, y, false);
        case 'N':
            return new KnightPiece(x, y, true);
        case 'n':
            return new KnightPiece(x, y, false);
        case 'B':
            return new BishopPiece(x, y, true);
        case 'b':
            return new BishopPiece(x, y, false);
        case 'Q':
            return new QueenPiece(x, y, true);
        case 'q':
            return new QueenPiece(x, y, false);
        case 'K':
            return new KingPiece(x, y, true);
        case 'k':
            return new KingPiece(x, y, false);
        default:
            return nullptr;
    }
}

