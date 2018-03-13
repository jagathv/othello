#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *find_best_move(Board *b);
    int calculate_hueristic(Board *b, Move m, int ply);
    int no_move_hueristic(Board *b);
    int minimax(Board *node, int depth, int ply, int scor);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board *b;
    Side sid;
    Board *stables;
};

#endif
