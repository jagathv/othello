#include "player.hpp"
#include <vector>
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     sid = side;
     b = new Board();
     stables = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {

}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
     Move* ret = nullptr;
     Side opsid;
     if (sid == WHITE) {
       opsid = BLACK;
     } else {
       opsid = WHITE;
     }
     b->doMove(opponentsMove, opsid);
     // if (testingMinimax) {
     std::vector<Move*> possible_moves;
     for (size_t i = 0; i < 8; i++) {
       for (size_t j = 0; j < 8; j++) {
         Move *temp = new Move(i, j);
         if (b->checkMove(temp, sid)) {
           possible_moves.push_back(temp);
         }
       }
     }
     int maxi = -99999;
     for (size_t k = 0; k < possible_moves.size(); k++) {
       Board *temp_board = b->copy();
       int scrr = calculate_hueristic(temp_board, *possible_moves[k], 1);
       temp_board->doMove(possible_moves[k], sid);
       std::cerr << possible_moves[k]->getX() << " " << possible_moves[k]->getY() << '\n';
       if (maxi < minimax(temp_board, 2, 1, scrr)) {
         maxi = minimax(temp_board, 2, 1, scrr);
         ret = possible_moves[k];
       }
     }
     b->doMove(ret, sid);
     int xx = ret->getX();
     int yy = ret->getY();
     if ((xx == 0 || xx == 7) && (yy == 0 || yy == 7)) {
       stables->set(sid, xx, yy);
     }
     for (int i = -1; i < 2; i++) {
       for (int j = -1; j < 2; j++) {
         if (stables->onBoard(xx + i, yy + j)) {
           if (stables->get(sid, xx+i, yy+j)) {
             stables->set(sid, xx, yy);
           }
         }
       }
     }
     return ret;
   }
   // else {
   //   Move *ret = find_best_move(b);
   //   b->doMove(ret, sid);
   //   return ret;
   // }
     // This code was for the naiive player implementation
     // for (size_t i = 0; i < 8; i++) {
     //   for (size_t j = 0; j < 8; j++) {
     //     ret->setX(i);
     //     ret->setY(j);
     //     if (b->checkMove(ret, sid)) {
     //
     //       b->doMove(ret, sid);
     //       return ret;
     //     }
     //   }
     // }

    // return nullptr;

// }

int Player::minimax(Board *node, int depth, int ply, int scor) {
  Side temp_side = sid;
  if (ply == -1) {
    if (sid == WHITE) {
      temp_side = BLACK;
    } else {
      temp_side = WHITE;
    }
  }

  if (node->isDone() || depth <= 0) {

    return scor * ply;
  }
  int alpha = -999;
  std::vector<Move *> move_vec;
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      Move *temp = new Move(i, j);
      if (node->checkMove(temp, temp_side)) {
        move_vec.push_back(temp);
      }
    }
  }
  for (size_t k = 0; k < move_vec.size(); k++) {
    Board *temp_board = node->copy();
    int h = calculate_hueristic(temp_board, *move_vec[k], ply);
    temp_board->doMove(move_vec[k], temp_side);
    alpha = max(alpha, -minimax(temp_board, depth-1, -ply, h));
  }
  return alpha;
}

int Player::no_move_hueristic(Board *b) {
  int score = b->countWhite() - b->countBlack();
  return score;
}

int Player::calculate_hueristic(Board *b, Move m, int ply) {
  Side tempsid = sid;
  if (ply == -1) {
    if (sid == WHITE) {
      tempsid = BLACK;
    } else {
      tempsid = WHITE;
    }
  }
  Board *temp = b->copy();
  int x = m.getX();
  int y = m.getY();
  // if (ply == 1) {
  // temp->doMove(&m, sid);
  // } else {
  //   if (sid == BLACK) {
  //     temp->doMove(&m, WHITE);
  //   } else {
  //     temp->doMove(&m, BLACK);
  //   }
  // }
  temp->doMove(&m, tempsid);
  // int score = temp->countWhite() - temp->countBlack();
  // if (tempsid == BLACK) {
    // score *= -1;
  // }
  int white_moves = 0;
  int black_moves = 0;
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      Move *temporary_move = new Move(i, j);
      if (temp->checkMove(temporary_move, WHITE)) {
        white_moves++;
      }
      if (temp->checkMove(temporary_move, BLACK)) {
        black_moves++;
      }
    }
  }
  int score;
  // int total = 64 - temp->countBlack() - temp->countWhite();
  if (tempsid == sid) {
    score = temp->countWhite() - temp->countBlack();
    if (tempsid == BLACK) {
      score *= -1;
      // score -= (white_moves / 15) * abs(score);
    } else {
      // score -= (black_moves / 15) * abs(score);
    }
  } else {
    score = temp->countWhite() - temp->countBlack();
    if (tempsid == WHITE) {
      score *= -1;
      // score += (black_moves / 15) * abs(score);
    } else {
      // score += (white_moves / 15) * abs(score);
    }
  }
  if ((x == 0 || x == 7) && (y == 0 || y == 7)) {
    if (ply == 1) {
      score +=abs(score)/2;
    } else {
      score -= abs(score)/2;
    }
  }
  // bool edgepenalty = false;
  // int penalty = abs(score)/2;
  if (((x == 1 || x == 6) && (y == 0 || y == 7)) || ((y == 1 || y == 6) && (x == 0 || x == 7))) {
      if (ply == 1) {
        score -= abs(score)/2;
      } else {
        score += abs(score)/2;
      }
      // edgepenalty = true;
  }
  int xx = m.getX();
  int yy = m.getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      if (stables->onBoard(xx + i, yy + j)) {
        if (stables->get(tempsid, xx+i, yy+j)) {
          std::cerr << "error here" << '\n';
          if (ply == 1) {
            score += abs(score) / 4;
          } else {
            score -= abs(score) / 4;
          }
        }
      }
    }
  }
  int black_moves = 0;
  int white_moves = 0;
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      Move *mev = new Move(i, j);
      if (temp->checkMove(mev, BLACK) ){
        black_moves++;
      }
      if (temp->checkMove(mev, WHITE)) {
        white_moves++;
      }
    }
    /* code */
  }
  int total = 64 - temp->countBlack() - temp->countWhite();
  if (ply == 1) {
    if (sid == WHITE) {
      score -= (black_moves / (total+1)) * abs(score) ;
    } else {
      score -= (white_moves / (total+1)) * abs(score) ;
    }
  } else {
    if (tempsid == WHITE) {
      score += (black_moves / (total+1)) * abs(score)  ;
    } else {
      score += (white_moves / (total+1)) * abs(score) ;
    }
  }
  return score;
}

Move* Player::find_best_move(Board *b) {
  int max = -9999999;
  Move *ret = nullptr;
  Move m(0, 0);
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      m.setX(i);
      m.setY(j);

      if (b->checkMove(&m, sid)) {
        int score = calculate_hueristic(b, m, 1);
        // std::cerr << score << '\n';
        if (max < score) {
          max = score;
          ret->setX(i);
          ret->setY(j);
        }
      }
    }
  }
  return ret;
}
