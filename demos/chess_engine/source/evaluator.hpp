#pragma once

#pragma once
#include "surge/surge.h"
#include "chess_utils.hpp"


template <Color Us>
int score_board(Position& pos) {
    int mv = 0;
    int our_mv = 0;
    int their_mv = 0;
    for (PieceType i = PAWN; i < NPIECE_TYPES-1; ++i) {
        our_mv += pop_count(pos.bitboard_of(Us, i)) * piece_values[i];
        their_mv += pop_count(pos.bitboard_of(~Us, i)) * piece_values[i];
    }
    mv += our_mv;
    mv -= their_mv;
    GameProgress progress = get_progress(pos, our_mv, their_mv);

    // Center control
    int cc = 0;
    Piece pd5 = pos.at(d5);
    Piece pe4 = pos.at(e4);
    Piece pd4 = pos.at(d4);
    Piece pe5 = pos.at(e5);

    if (pd5 != NO_PIECE) cc += (color_of(pd5) == Us) ? 25 : -25;
    if (pe4 != NO_PIECE) cc += (color_of(pe4) == Us) ? 25 : -25;
    if (pd4 != NO_PIECE) cc += (color_of(pd4) == Us) ? 25 : -25;
    if (pe5 != NO_PIECE) cc += (color_of(pe5) == Us) ? 25 : -25;

    // Knight placement
    const static Bitboard edges_mask = MASK_FILE[AFILE] | MASK_RANK[RANK1] | MASK_FILE[H_FILE] | MASK_RANK[RANK8];
    int np = 0;
    np -= pop_count(pos.bitboard_of(Us, KNIGHT) & edges_mask) * 50;
    np += pop_count(pos.bitboard_of(~Us, KNIGHT) & edges_mask) * 50;


    // Rook placement
    int rp = 0;
    rp += pop_count(pos.bitboard_of(Us, ROOK) & MASK_RANK[Us == WHITE ? RANK7 : RANK2]) * 30;
    Rank enemy_7th = (Us == WHITE) ? RANK2 : RANK7;
    rp -= pop_count(pos.bitboard_of(~Us, ROOK) & MASK_RANK[enemy_7th]) * 30;

    // King placement

    static constexpr int king_pcsq_table[64] = { 71, 86, 76, 71, 71, 76, 86, 71, 
                                                 86, 71, 58, 51, 51, 58, 71, 86, 
                                                 76, 58, 42, 32, 32, 42, 58, 76, 
                                                 71, 51, 32, 14, 14, 32, 51, 71, 
                                                 71, 51, 32, 14, 14, 32, 51, 71, 
                                                 76, 58, 42, 32, 32, 42, 58, 76, 
                                                 86, 71, 58, 51, 51, 58, 71, 86, 
                                                 71, 86, 76, 71, 71, 76, 86, 71 };

    
    int kp = 0;
    if (progress == MIDGAME) {
        kp += king_pcsq_table[bsf(pos.bitboard_of(Us, KING))];
        kp -= king_pcsq_table[bsf(pos.bitboard_of(~Us, KING))];
    }

    static constexpr int white_pawn_pcsq[64] = {
        70,70,70,70,70,70,70,70,
        60,60,60,60,60,60,60,60,
        50,50,50,50,50,50,50,50,
        40,40,40,40,40,40,40,40,
        30,30,30,30,30,30,30,30,
        20,20,20,20,20,20,20,20,
        10,10,10,10,10,10,10,10,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };

    static constexpr int black_pawn_pcsq[64] = {
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
        10,10,10,10,10,10,10,10,
        20,20,20,20,20,20,20,20,
        30,30,30,30,30,30,30,30,
        40,40,40,40,40,40,40,40,
        50,50,50,50,50,50,50,50,
        60,60,60,60,60,60,60,60,
        70,70,70,70,70,70,70,70 };

    int pp = 0;
    if (progress == ENDGAME) {
        if (Us == WHITE)
        {
            pp += white_pawn_pcsq[bsf(pos.bitboard_of(Us, PAWN))];
            pp -= black_pawn_pcsq[bsf(pos.bitboard_of(~Us, PAWN))];
        }
        else
        {
            pp += black_pawn_pcsq[bsf(pos.bitboard_of(Us, PAWN))];
            pp -= white_pawn_pcsq[bsf(pos.bitboard_of(~Us, PAWN))];
        }
    }

    // Check status
    int cs = 0;
    if (pos.in_check<Us>()) {
        cs = -20;
    }
    else if (pos.in_check<~Us>()) {
        cs = 20;
    }
    return mv + cc + np + rp + kp + cs + pp;
}

