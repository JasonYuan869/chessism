#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <memory>
#include "BoardState.h"
#include "players/Player.h"
#include "Subject.h"

#define NUM_FEATURES 6
enum Features {
    UNDO = 0,
    HELP = 1,
    SWITCH = 2,
    HINT = 3,
    FIFTY = 4,
    INSUFFICIENT = 5
};


class Game : public Subject {
    BoardState board;
    std::unique_ptr<Player> white;
    std::unique_ptr<Player> black;


    /*use bit encoding for each feature:
        1 << 0 is undo
        1 << 1 is help 
        1 << 2 is switch player
        1 << 3 is computer hint
        1 << 4 is checking for fifty move rule
        1 << 5 is insufficient material checking
    */
    int features;
    bool checkDraws();
public:
    Game(std::unique_ptr<Player>&& whitePlayer, std::unique_ptr<Player>&& blackPlayer);

    void setup();

    BoardState& getBoard();

    // Runs the game and returns the end state
    // blackScore += return
    // whiteScore += (1-return)
    double run();

    // Returns a unique_ptr to a player of the given type
    // or nullptr if the type is invalid
    static std::unique_ptr<Player> makePlayer(bool white, const std::string& playerType);
};

#endif
