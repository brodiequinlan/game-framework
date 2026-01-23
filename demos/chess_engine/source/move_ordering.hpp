#pragma once
#include "surge/surge.h"


template <Color Us>
void order_moves(Position& p, MoveList<Us>& moves, transposition_table& tt, bool use_tt) {
    if (!use_tt) {
        std::sort(moves.begin(), moves.end(), [&](Move a, Move b) {
            int ascore = 0;
            int bscore = 0;

            if (a.is_capture())
                ascore += 10;
            if (b.is_capture())
                bscore += 10;

            if (a.flags() != QUIET) {
                ascore += 5;
            }
            if (b.flags() != QUIET) {
                bscore += 5;
            }
            return ascore > bscore;
            });
    }
    else {
        std::sort(moves.begin(), moves.end(), [&tt](Move& a, Move& b) {
            if(tt.selected_move() == a) return true;

            return tt.move_scores[a.str_d()] > tt.move_scores[b.str_d()];
        });
    }
    
}