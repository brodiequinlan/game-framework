#pragma once

#pragma once
#include "surge/surge.h"
#include "chess_utils.hpp"
#include "surge/tables.h"
inline Bitboard attackers_from_color(const Position& pos, Color attacker, Square s, Bitboard occ)
{
    return (attacker == WHITE) ? pos.attackers_from<WHITE>(s, occ)
        : pos.attackers_from<BLACK>(s, occ);
}
template <Color Us>
int score_board(Position& pos)
{
    constexpr Color Them = ~Us;

    // ----------------------------
    // Basic board state
    // ----------------------------
    const Bitboard occ = pos.all_pieces<WHITE>() | pos.all_pieces<BLACK>();
    const Bitboard usBB = pos.all_pieces<Us>();
    const Bitboard thBB = pos.all_pieces<Them>();
    const Bitboard usP = pos.bitboard_of(Us, PAWN);
    const Bitboard thP = pos.bitboard_of(Them, PAWN);

    auto mirror_sq = [](int sq) constexpr -> int { return sq ^ 56; }; // rank flip for a1=0..h8=63

    auto pst = [&](const int* t, Color c, int sq) -> int {
        // tables are from White POV
        return (c == WHITE) ? t[sq] : t[mirror_sq(sq)];
        };

    auto file_of_sq = [](int sq) constexpr -> int { return sq & 7; };
    auto rank_of_sq = [](int sq) constexpr -> int { return sq >> 3; };

    // tempo (small)
    int tempo = (pos.turn() == Us) ? 10 : -10;

    // ----------------------------
    // Phase (0..24) for tapering
    // ----------------------------
    // N,B = 1, R = 2, Q = 4
    static constexpr int PHASE_W[6] = { 0, 1, 1, 2, 4, 0 };
    int phase = 0;
    for (PieceType pt = PAWN; pt < KING; ++pt) {
        phase += pop_count(pos.bitboard_of(Us, pt)) * PHASE_W[pt];
        phase += pop_count(pos.bitboard_of(Them, pt)) * PHASE_W[pt];
    }
    if (phase > 24) phase = 24;

    // ----------------------------
    // Material MG/EG
    // ----------------------------
    static constexpr int MG_VAL[6] = { 100, 320, 330, 500, 900, 0 };
    static constexpr int EG_VAL[6] = { 120, 300, 320, 520, 900, 0 };

    int mg = 0, eg = 0;
    for (PieceType pt = PAWN; pt < KING; ++pt) {
        mg += pop_count(pos.bitboard_of(Us, pt)) * MG_VAL[pt];
        mg -= pop_count(pos.bitboard_of(Them, pt)) * MG_VAL[pt];
        eg += pop_count(pos.bitboard_of(Us, pt)) * EG_VAL[pt];
        eg -= pop_count(pos.bitboard_of(Them, pt)) * EG_VAL[pt];
    }

    // ----------------------------
    // PSTs (MG/EG) for all pieces
    // (These are "good enough" starter PSTs; tune later via selfplay)
    // ----------------------------
    static constexpr int MG_PAWN[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
        10, 10, 10,-10,-10, 10, 10, 10,
         5,  5, 10, 20, 20, 10,  5,  5,
         0,  0,  0, 25, 25,  0,  0,  0,
         5, -5,-10, 10, 10,-10, -5,  5,
         5, 10, 10,-20,-20, 10, 10,  5,
        10, 10, 10,-10,-10, 10, 10, 10,
         0,  0,  0,  0,  0,  0,  0,  0
    };
    static constexpr int EG_PAWN[64] = {
         0,  0,  0,  0,  0,  0,  0,  0,
        20, 20, 20, 20, 20, 20, 20, 20,
        15, 15, 15, 15, 15, 15, 15, 15,
        10, 10, 10, 12, 12, 10, 10, 10,
         6,  6,  6,  8,  8,  6,  6,  6,
         3,  3,  3,  4,  4,  3,  3,  3,
         1,  1,  1,  0,  0,  1,  1,  1,
         0,  0,  0,  0,  0,  0,  0,  0
    };

    static constexpr int MG_KNIGHT[64] = {
       -50,-40,-30,-30,-30,-30,-40,-50,
       -40,-20,  0,  0,  0,  0,-20,-40,
       -30,  0, 10, 15, 15, 10,  0,-30,
       -30,  5, 15, 20, 20, 15,  5,-30,
       -30,  0, 15, 20, 20, 15,  0,-30,
       -30,  5, 10, 15, 15, 10,  5,-30,
       -40,-20,  0,  5,  5,  0,-20,-40,
       -50,-40,-30,-30,-30,-30,-40,-50
    };
    static constexpr int EG_KNIGHT[64] = {
       -40,-30,-20,-20,-20,-20,-30,-40,
       -30,-10,  0,  0,  0,  0,-10,-30,
       -20,  0, 10, 10, 10, 10,  0,-20,
       -20,  0, 10, 15, 15, 10,  0,-20,
       -20,  0, 10, 15, 15, 10,  0,-20,
       -20,  0, 10, 10, 10, 10,  0,-20,
       -30,-10,  0,  0,  0,  0,-10,-30,
       -40,-30,-20,-20,-20,-20,-30,-40
    };

    static constexpr int MG_BISHOP[64] = {
       -20,-10,-10,-10,-10,-10,-10,-20,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -10,  0,  5, 10, 10,  5,  0,-10,
       -10,  5,  5, 10, 10,  5,  5,-10,
       -10,  0, 10, 10, 10, 10,  0,-10,
       -10, 10, 10, 10, 10, 10, 10,-10,
       -10,  5,  0,  0,  0,  0,  5,-10,
       -20,-10,-10,-10,-10,-10,-10,-20
    };
    static constexpr int EG_BISHOP[64] = {
       -20,-10,-10,-10,-10,-10,-10,-20,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -10,  0, 10, 10, 10, 10,  0,-10,
       -10,  0, 10, 15, 15, 10,  0,-10,
       -10,  0, 10, 15, 15, 10,  0,-10,
       -10,  0, 10, 10, 10, 10,  0,-10,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -20,-10,-10,-10,-10,-10,-10,-20
    };

    static constexpr int MG_ROOK[64] = {
         0,  0,  0,  5,  5,  0,  0,  0,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
         5, 10, 10, 10, 10, 10, 10,  5,
         0,  0,  0,  0,  0,  0,  0,  0
    };
    static constexpr int EG_ROOK[64] = {
         0,  0,  0,  5,  5,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         5,  5,  5,  5,  5,  5,  5,  5,
         0,  0,  0,  0,  0,  0,  0,  0
    };

    static constexpr int MG_QUEEN[64] = {
       -20,-10,-10, -5, -5,-10,-10,-20,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
         0,  0,  5,  5,  5,  5,  0, -5,
       -10,  5,  5,  5,  5,  5,  0,-10,
       -10,  0,  5,  0,  0,  0,  0,-10,
       -20,-10,-10, -5, -5,-10,-10,-20
    };
    static constexpr int EG_QUEEN[64] = {
       -20,-10,-10, -5, -5,-10,-10,-20,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
         0,  0,  5,  5,  5,  5,  0, -5,
       -10,  0,  5,  5,  5,  5,  0,-10,
       -10,  0,  0,  0,  0,  0,  0,-10,
       -20,-10,-10, -5, -5,-10,-10,-20
    };

    static constexpr int MG_KING[64] = {
        20, 30, 10,  0,  0, 10, 30, 20,
        20, 20,  0,  0,  0,  0, 20, 20,
       -10,-20,-20,-20,-20,-20,-20,-10,
       -20,-30,-30,-40,-40,-30,-30,-20,
       -30,-40,-40,-50,-50,-40,-40,-30,
       -30,-40,-40,-50,-50,-40,-40,-30,
       -30,-40,-40,-50,-50,-40,-40,-30,
       -30,-40,-40,-50,-50,-40,-40,-30
    };
    static constexpr int EG_KING[64] = {
       -50,-30,-30,-30,-30,-30,-30,-50,
       -30,-10,  0,  0,  0,  0,-10,-30,
       -30,  0, 10, 15, 15, 10,  0,-30,
       -30,  0, 15, 25, 25, 15,  0,-30,
       -30,  0, 15, 25, 25, 15,  0,-30,
       -30,  0, 10, 15, 15, 10,  0,-30,
       -30,-10,  0,  0,  0,  0,-10,-30,
       -50,-30,-30,-30,-30,-30,-30,-50
    };

    // PST accumulate for Us/Them
    auto add_pst = [&](Color c, PieceType pt, const int* mgT, const int* egT, int sign) {
        Bitboard b = pos.bitboard_of(c, pt);
        while (b) {
            int sq = int(pop_lsb(&b));
            mg += sign * pst(mgT, c, sq);
            eg += sign * pst(egT, c, sq);
        }
        };

    add_pst(Us, PAWN, MG_PAWN, EG_PAWN, +1);
    add_pst(Them, PAWN, MG_PAWN, EG_PAWN, -1);
    add_pst(Us, KNIGHT, MG_KNIGHT, EG_KNIGHT, +1);
    add_pst(Them, KNIGHT, MG_KNIGHT, EG_KNIGHT, -1);
    add_pst(Us, BISHOP, MG_BISHOP, EG_BISHOP, +1);
    add_pst(Them, BISHOP, MG_BISHOP, EG_BISHOP, -1);
    add_pst(Us, ROOK, MG_ROOK, EG_ROOK, +1);
    add_pst(Them, ROOK, MG_ROOK, EG_ROOK, -1);
    add_pst(Us, QUEEN, MG_QUEEN, EG_QUEEN, +1);
    add_pst(Them, QUEEN, MG_QUEEN, EG_QUEEN, -1);

    // Kings
    {
        int usK = int(bsf(pos.bitboard_of(Us, KING)));
        int thK = int(bsf(pos.bitboard_of(Them, KING)));
        mg += pst(MG_KING, Us, usK);
        eg += pst(EG_KING, Us, usK);
        mg -= pst(MG_KING, Them, thK);
        eg -= pst(EG_KING, Them, thK);
    }

    // ----------------------------
    // Mobility using your attacks<PT>()
    // ----------------------------
    auto add_mobility = [&](PieceType pt, int mgW, int egW) {
        // template version for speed; expand via switch to keep it simple
        auto eval_side = [&](Color c, int sign) {
            Bitboard own = (c == Us) ? usBB : thBB;
            Bitboard b = pos.bitboard_of(c, pt);
            while (b) {
                Square s = pop_lsb(&b);
                Bitboard moves = attacks(pt, s, occ) & ~own; // uses your runtime overload
                int m = pop_count(moves);
                mg += sign * m * mgW;
                eg += sign * m * egW;
            }
            };
        eval_side(Us, +1);
        eval_side(Them, -1);
        };

    add_mobility(KNIGHT, 4, 4);
    add_mobility(BISHOP, 4, 4);
    add_mobility(ROOK, 2, 3);
    add_mobility(QUEEN, 1, 2);

    // ----------------------------
    // Pawn structure (doubled / isolated / connected / passed)
    // ----------------------------
    auto pawn_file_counts = [&](Bitboard pawns, int out[8]) {
        for (int i = 0; i < 8; ++i) out[i] = 0;
        Bitboard t = pawns;
        while (t) {
            int sq = int(pop_lsb(&t));
            out[file_of_sq(sq)]++;
        }
        };

    int usFileCnt[8], thFileCnt[8];
    pawn_file_counts(usP, usFileCnt);
    pawn_file_counts(thP, thFileCnt);

    // doubled pawns
    for (int f = 0; f < 8; ++f) {
        if (usFileCnt[f] > 1) { mg -= 12 * (usFileCnt[f] - 1); eg -= 10 * (usFileCnt[f] - 1); }
        if (thFileCnt[f] > 1) { mg += 12 * (thFileCnt[f] - 1); eg += 10 * (thFileCnt[f] - 1); }
    }

    auto is_isolated = [&](int f, const int cnt[8]) -> bool {
        bool left = (f > 0) && cnt[f - 1] > 0;
        bool right = (f < 7) && cnt[f + 1] > 0;
        return !left && !right;
        };

    // passed pawn helper (simple but effective)
    auto is_passed = [&](Color c, int sq) -> bool {
        int f = file_of_sq(sq);
        int r = rank_of_sq(sq);

        Bitboard enemyP = pos.bitboard_of(~c, PAWN);

        // any enemy pawn in front on same/adjacent files blocks "passed"
        for (int df = -1; df <= 1; ++df) {
            int ff = f + df;
            if (ff < 0 || ff > 7) continue;

            if (c == WHITE) {
                for (int rr = r + 1; rr <= 7; ++rr) {
                    int idx = rr * 8 + ff;
                    if (enemyP & (Bitboard(1) << idx)) return false;
                }
            }
            else {
                for (int rr = r - 1; rr >= 0; --rr) {
                    int idx = rr * 8 + ff;
                    if (enemyP & (Bitboard(1) << idx)) return false;
                }
            }
        }
        return true;
        };

    // connected pawns: neighbor pawn on adjacent file on same rank (cheap)
    auto is_connected = [&](Bitboard pawns, int sq) -> bool {
        int f = file_of_sq(sq);
        int r = rank_of_sq(sq);
        if (f > 0) {
            int idx = r * 8 + (f - 1);
            if (pawns & (Bitboard(1) << idx)) return true;
        }
        if (f < 7) {
            int idx = r * 8 + (f + 1);
            if (pawns & (Bitboard(1) << idx)) return true;
        }
        return false;
        };

    // evaluate pawn-by-pawn
    {
        Bitboard b = usP;
        while (b) {
            int sq = int(pop_lsb(&b));
            int f = file_of_sq(sq);
            int r = rank_of_sq(sq);

            if (is_isolated(f, usFileCnt)) { mg -= 10; eg -= 8; }
            if (is_connected(usP, sq)) { mg += 4;  eg += 6; }

            if (is_passed(Us, sq)) {
                int adv = (Us == WHITE) ? r : (7 - r); // 0..7
                mg += 8 + adv * 2;
                eg += 18 + adv * 6;
            }
        }

        b = thP;
        while (b) {
            int sq = int(pop_lsb(&b));
            int f = file_of_sq(sq);
            int r = rank_of_sq(sq);

            if (is_isolated(f, thFileCnt)) { mg += 10; eg += 8; }
            if (is_connected(thP, sq)) { mg -= 4;  eg -= 6; }

            if (is_passed(Them, sq)) {
                int adv = (Them == WHITE) ? r : (7 - r);
                mg -= 8 + adv * 2;
                eg -= 18 + adv * 6;
            }
        }
    }

    // ----------------------------
    // Bishop pair
    // ----------------------------
    if (pop_count(pos.bitboard_of(Us, BISHOP)) >= 2) { mg += 25; eg += 35; }
    if (pop_count(pos.bitboard_of(Them, BISHOP)) >= 2) { mg -= 25; eg -= 35; }

    // ----------------------------
    // Rooks: open/semi-open files + 7th rank
    // ----------------------------
    auto rook_terms = [&](Color c) -> int {
        Bitboard rooks = pos.bitboard_of(c, ROOK);
        Bitboard ourP = pos.bitboard_of(c, PAWN);
        Bitboard thP2 = pos.bitboard_of(~c, PAWN);
        int s = 0;

        while (rooks) {
            int sq = int(pop_lsb(&rooks));
            int f = file_of_sq(sq);
            int r = rank_of_sq(sq);

            Bitboard fileMask = MASK_FILE[f];
            bool ourPawnOnFile = (ourP & fileMask) != 0;
            bool theirPawnOnFile = (thP2 & fileMask) != 0;

            if (!ourPawnOnFile && !theirPawnOnFile) s += 18;      // open
            else if (!ourPawnOnFile && theirPawnOnFile) s += 10;  // semi-open

            if ((c == WHITE && r == 6) || (c == BLACK && r == 1)) s += 15; // 7th
        }
        return s;
        };

    int rt = rook_terms(Us) - rook_terms(Them);
    mg += rt;
    eg += rt / 2;

    // ----------------------------
    // King safety (mostly MG)
    // 1) pawn shield
    // 2) enemy attacks into king zone
    // 3) open king file penalty
    // ----------------------------
    auto king_safety_mg = [&](Color c) -> int {
        Square ksq = bsf(pos.bitboard_of(c, KING));
        int kf = int(file_of(ksq));
        int kr = int(rank_of(ksq));

        Bitboard ourPawns = pos.bitboard_of(c, PAWN);
        Bitboard allPawns = pos.bitboard_of(WHITE, PAWN) | pos.bitboard_of(BLACK, PAWN);

        int dir = (c == WHITE) ? 1 : -1;

        auto on_board = [](int rr, int ff) -> bool { return rr >= 0 && rr <= 7 && ff >= 0 && ff <= 7; };

        int shield = 0;
        for (int df = -1; df <= 1; ++df) {
            int ff = kf + df;
            int rr1 = kr + dir;
            int rr2 = kr + 2 * dir;

            if (on_board(rr1, ff)) shield += (ourPawns >> (rr1 * 8 + ff)) & 1ULL;
            if (on_board(rr2, ff)) shield += (ourPawns >> (rr2 * 8 + ff)) & 1ULL;
        }

        // max 6 shield points
        int score = 0;
        score -= (6 - shield) * 10;

        // open file on king file
        if ((allPawns & MASK_FILE[kf]) == 0) score -= 14;

        // enemy pressure into king zone (king + its ring)
        Bitboard zone = attacks<KING>(ksq, occ) | SQUARE_BB[ksq];

        // count enemy attackers: for each zone square, how many enemy pieces attack it?
        // (cheap-ish; 9 squares)
        int pressure = 0;
        Bitboard z = zone;
        while (z) {
            Square s = pop_lsb(&z);
            Bitboard at = attackers_from_color(pos, Color(~c), s, occ);
            pressure += sparse_pop_count(at);             // sparse_pop_count exists in your code
        }

        score -= pressure * 2;
        return score;
        };

    // only apply king safety strongly when not deep endgame
    int ksUs = king_safety_mg(Us);
    int ksThem = king_safety_mg(Them);
    mg += (ksUs - ksThem);

    // ----------------------------
    // Check status (tiny)
    // ----------------------------
    if (pos.in_check<Us>())   mg -= 20;
    if (pos.in_check<Them>()) mg += 20;

    // Tempo
    mg += tempo;

    // ----------------------------
    // Taper MG/EG
    // ----------------------------
    int blended = (mg * phase + eg * (24 - phase)) / 24;
    return blended;
}
