#include "ComputerPlayer1.h"
#include "../utility.h"

using namespace std;
ComputerPlayer1::ComputerPlayer1(bool isWhite): Player{isWhite} {}
ComputerPlayer1::~ComputerPlayer1() {}

MoveResult ComputerPlayer1::makeMove(BoardState& board) {
    string command;
    cin >> command;

    if (cin.eof()) {
        throw -1;
    }

    if (command == "move") {
        vector<Move> moves = allValidMoves(board); // get valid moves from player

        int numMoves = moves.size();
        int randomMove = randomFrom(0, numMoves - 1);

        Move m = moves.at(randomMove);
        board.movePiece(m);
        return MoveResult::SUCCESS;
    } else if (command == "setup") {
        return MoveResult::SETUP;
    }

    return MoveResult::INVALID_MOVE;
}
