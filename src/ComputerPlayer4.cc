#include "ComputerPlayer4.h"
#include <cmath>

ComputerPlayer4::ComputerPlayer4(bool isWhite): Player{isWhite} {}
ComputerPlayer4::~ComputerPlayer4() {

}

int ComputerPlayer4::makeMove(BoardState& board) {
    string command;
    cin >> command;
    if (command == "move") {
    
        int best = 0;
        Move bestmove;
        int depth = 3;

        vector<Piece*> pieces;
        if (isWhite) {
            pieces = board.whitePieces;
        } else {
            pieces = board.blackPieces;
        }

        for (Piece* p : pieces) {
            vector<Move> moves = p->validMoves; // get valid moves from piece
            for (Move m : moves) {
                board.movePiece(m);
                int value = minimax(depth - 1, board, true);
                board.undo();
                if(value >= best) {
                    best = value;
                    bestmove = m;
                }
            }
        }

        if (bestmove.getTo().first != -1 && bestmove.getTo().second != -1) {
            board.movePiece(bestmove);
            return 1;
        }
    }
    
    return 0;
}

int ComputerPlayer4::evaluateBoard(BoardState& board) {
    int eval = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.board[i][j] != nullptr) {
                if ((board.board[i][j]->isWhite && isWhite) || 
                    (board.board[i][j]->isWhite && !isWhite)) {
                        eval += board.board[i][j]->value;
                } else {
                    eval -= board.board[i][j]->value;
                }
            }
        }
    }
    return eval;
}

int ComputerPlayer4::minimax(int depth, BoardState& board, bool isMaximizingPlayer) {
    if (depth == 0) {
        return -1 * evaluateBoard(board);
    }

    vector<Piece*> pieces;
    if (isWhite) {
        pieces = board.whitePieces;
    } else {
        pieces = board.blackPieces;
    }

    for (Piece* p : pieces) {
        vector<Move> moves = p->validMoves; // get valid moves from piece
        for (Move m : moves) {
            if (isMaximizingPlayer) {
                int best = 0;
                board.movePiece(m);
                best = fmax(best, minimax(depth - 1, board, !isMaximizingPlayer));
                board.undo();
                return best;
            } else {
                int best = 9999;
                board.movePiece(m);
                best = fmin(best, minimax(depth - 1, board, !isMaximizingPlayer));
                board.undo();
                return best;
            }
        }
    }
}