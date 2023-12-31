#include "BoardState.h"
#include "pieces/PawnPiece.h"
#include "pieces/RookPiece.h"
#include "pieces/KnightPiece.h"
#include "pieces/BishopPiece.h"
#include "pieces/QueenPiece.h"
#include "pieces/KingPiece.h"
#include <vector>

using namespace std;

BoardState::BoardState(bool isWhiteTurn) : isWhiteTurn{isWhiteTurn} {
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
    for (auto& piece : blackPieces) {
        delete piece;
    }
}

bool BoardState::getCheckmate(bool white) {
    // Update the valid moves for each piece
    updateValidMoves(white);

    if (!updateCheck(white)) {
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
    return king->checked;
}

bool BoardState::updateCheck(bool white) {
    KingPiece* king = white ? whiteKing : blackKing;

    // Mutates the king's checked variable
    king->checked = getAttacked(king->position_x, king->position_y, white);
    return king->checked;
}

bool BoardState::getAttacked(int x, int y, bool white) {
    for (auto& piece : white ? blackPieces : whitePieces) {
        if (piece->isAlive && piece->isAttacking(x, y, *this)) {
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

    if (piece->getType() == PieceType::KING) {
        if (isWhite) {
            whiteKing = dynamic_cast<KingPiece *>(piece);
        } else {
            blackKing = dynamic_cast<KingPiece *>(piece);
        }
    }
}

void BoardState::removePiece(int x, int y) {
    Piece* piece = board[y][x];

    if (piece != nullptr) {
        piece->isAlive = false;
        if (piece == whiteKing) {
            whiteKing = nullptr;
        } else if (piece == blackKing) {
            blackKing = nullptr;
        }
    }

    board[y][x] = nullptr;
}

void BoardState::updateValidMoves(bool white) {
    KingPiece* king = white ? whiteKing : blackKing;
    for (auto piece : white ? whitePieces : blackPieces) {
        vector<Move> moves;

        if (!piece->isAlive){
            piece->validMoves = moves;
            continue;
        }

        for (auto& move : piece->getPieceMoves(*this)) {
            // Simulate the move
            if (movePiece(move)) {
                // Is our king checked?
                if (!getAttacked(king->position_x, king->position_y, white)) {
                    // Add the move to the vector of moves
                    moves.push_back(move);
                }

                // Undo the move
                undo();
            }
        }

        // Update the validMoves vector
        piece->validMoves = moves;
    }
}


bool BoardState::movePieceIfLegal(const Move& move){
    int x = move.from.first;
    int y = move.from.second;
    Piece* pieceToMove = board[y][x];

    if (pieceToMove == nullptr || pieceToMove->isWhite != isWhiteTurn) {
        return false;
    }

    for (auto validMove : pieceToMove->validMoves){
        if (move.sameMoveAs(validMove)){
            return movePiece(validMove);
        }
    }

   return false;
}

bool BoardState::movePiece(const Move& move) {
    int x = move.from.first;
    int y = move.from.second;
    Piece* pieceToMove = board[y][x];

    if (pieceToMove == nullptr) {
        return false;
    }

    if (move.disabledCastle) {
        pieceToMove->canCastle = false;
    }

    int to_x = move.to.first;
    int to_y = move.to.second;

    //handle the capture
    if (!move.isCastle && move.capturedOrMovedPiece != nullptr){
       move.capturedOrMovedPiece->isAlive = false;
       int captured_x = move.capturedOrMovedPiece->position_x;
       int captured_y = move.capturedOrMovedPiece->position_y;
       board[captured_y][captured_x] = nullptr;
    }


    // either pawn promotion, or we actually move the piece
    if (move.promotion != '-'){
        pieceToMove->isAlive = false;
        board[to_y][to_x] = BoardState::makePiece(move.promotion, to_y, to_x);
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
    if (move.isCastle && move.capturedOrMovedPiece != nullptr){

        int rookStartx = move.rookFrom.first;
        int rookStarty = move.rookFrom.second;
        int rookEndx = move.rookTo.first;
        int rookEndy = move.rookTo.second;

        Piece* rook =  board[rookStarty][rookStartx];

        board[rookEndy][rookEndx] = rook;
        board[rookStarty][rookStartx] = nullptr;
        rook->setPosition(rookEndx,rookEndy);

        // Disable castling for the rook
        rook->canCastle = false;
    }
    lastMoves.push_back(move);
    isWhiteTurn = !isWhiteTurn;
    return true;
}

void BoardState::undo() {
    if (lastMoves.empty()){
        return;
    }
    //toggle the turn so that the logic is more similar to movePiece
    //we will undo movePiece in the opposite order compared to the order that
    //the last movePiece was done in
    isWhiteTurn = !isWhiteTurn;
    bool lastTurnIsWhite = isWhiteTurn;
    Move lastMove = lastMoves.back();
    lastMoves.pop_back();

    int startx = lastMove.from.first;
    int starty = lastMove.from.second;

    int endx = lastMove.to.first;
    int endy = lastMove.to.second;

    //castling logic
    if (lastMove.isCastle && lastMove.capturedOrMovedPiece != nullptr){

        int rookStartx = lastMove.rookFrom.first;
        int rookStarty = lastMove.rookFrom.second;
        int rookEndx = lastMove.rookTo.first;
        int rookEndy = lastMove.rookTo.second;

        Piece* rook =  board[rookEndy][rookEndx];

        board[rookStarty][rookStartx] = rook;
        board[rookEndy][rookEndx] = nullptr;
        rook->setPosition(rookStartx,rookStarty);
        rook->canCastle = true;
    }

    //pawn promotion or regular move
    if (lastMove.promotion != '-') {
        vector<Piece*>& pieces = lastTurnIsWhite ? whitePieces : blackPieces;
        Piece* promoted = pieces.back();
        pieces.pop_back();
        delete promoted;

        Piece* pawn = nullptr;
        for (auto piece : pieces){
            if (!piece->isAlive && piece->getPosition() == lastMove.from) {
                pawn = piece;
                break;
            }
        }

        pawn->isAlive = true;
        board[starty][startx] = pawn;
    } else {
        Piece* pieceThatMoved = board[endy][endx];
        board[starty][startx] = pieceThatMoved;
        pieceThatMoved->setPosition(startx,starty);

        if (lastMove.disabledCastle) {
            pieceThatMoved->canCastle = true;
        }
    }

    board[endy][endx] = nullptr;

    //capturing piece
    if (!lastMove.isCastle && lastMove.capturedOrMovedPiece != nullptr){
        lastMove.capturedOrMovedPiece->isAlive = true;
        int aliveX = lastMove.capturedOrMovedPiece->position_x;
        int aliveY = lastMove.capturedOrMovedPiece->position_y;
        board[aliveY][aliveX] = lastMove.capturedOrMovedPiece;
    }

}


bool BoardState::canStartGame() const {
    int whiteKingCount = 0;
    int blackKingCount = 0;
    for (auto& piece : whitePieces) {
        if (piece->isAlive && piece->getType() == PieceType::KING) {
            whiteKingCount++;
        }
    }
    for (auto& piece : blackPieces) {
        if (piece->isAlive && piece->getType() == PieceType::KING) {
            blackKingCount++;
        }
    }

    return whiteKingCount == 1 && blackKingCount == 1;
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

std::vector<Move> BoardState::allValidMoves() const {
    const vector<Piece*>& pieces = isWhiteTurn ? whitePieces : blackPieces;
    vector<Move> moves;
    for (auto& piece : pieces) {
        moves.insert(moves.end(), piece->validMoves.begin(), piece->validMoves.end());
    }
    return moves;
}

