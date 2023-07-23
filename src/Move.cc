#include "Move.h"

using namespace std;

Move::Move(pair<int, int> to, pair<int, int> from, char promotion='-'): to{to}, from{from}, promotion{promotion} {}
Move::Move() {
    to = pair{-1, -1};
    from = pair{-1, -1};
    promotion = '-';
}
Move::Move(int to_x,int to_y, 
            int from_x,int from_y,
            char promotion = '-' ) : to{to_x,to_y}, from{from_x,from_y}, promotion{promotion}{}

Move::~Move() {
    
}

pair<int, int> Move::getTo() {
    return to;
}
