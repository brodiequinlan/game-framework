#pragma once

#include <bq.h>
#include "utils.hpp"
#include "surge/surge.h"

enum GameProgress {
	MIDGAME,
	ENDGAME
};

constexpr int piece_values[NPIECE_TYPES] = {
	100,    // PAWN
	300,    // KNIGHT
	305,    // BISHOP
	500,    // ROOK
	900,    // QUEEN
	2000000 // KING
};

struct search_statistics{
	int quinscience_depth_reached = 0;
	int ellapsed_ms 			  = 0;
	int depth 					  = 0;
	int score 					  = 0;
	long long nodes_searched 	  = 0;
	bool mate_found 			  = false;
	Move selected_move;
	void reset();
};


template <Color Us>
inline bool gives_check(Position& pos, Move m) {
    constexpr Color Them = ~Us;
    const Square their_king = bsf(pos.bitboard_of(Them, KING));
    Bitboard all = pos.all_pieces<WHITE>() | pos.all_pieces<BLACK>();

    Piece pc = pos.at(m.from());
    Bitboard from_bb = SQUARE_BB[m.from()];
    Bitboard to_bb = SQUARE_BB[m.to()];

    // Adjust occupancy for pseudo-move
    Bitboard occ = all;
    occ &= ~from_bb;
    if (m.flags() == EN_PASSANT)
        occ &= ~SQUARE_BB[m.to() + relative_dir<Us>(SOUTH)];
    else if (m.flags() == CAPTURE || m.flags() >= PC_KNIGHT)
        occ &= ~to_bb;
    occ |= to_bb;

    // 1. Direct check from the moving piece
    auto king_bb = pos.bitboard_of(Them, KING);
    switch (type_of(pc)) {
    case PAWN: {
        // Direct pawn check
        if (pawn_attacks<Us>(m.to()) & king_bb) return true;

        // Promotion giving check
        if (rank_of(m.to()) == relative_rank<Us>(RANK7)) {
            // Assume promotion to any piece that can attack the king
            if ((attacks<KNIGHT>(m.to(), occ) |
                attacks<BISHOP>(m.to(), occ) |
                attacks<ROOK>(m.to(), occ) |
                attacks<QUEEN>(m.to(), occ)) & king_bb)
                return true;
        }
        break;
    }
    case KNIGHT:
        if (attacks<KNIGHT>(m.to(), occ) & king_bb) return true;
        break;
    case BISHOP:
        if (attacks<BISHOP>(m.to(), occ) & king_bb) return true;
        break;
    case ROOK:
        if (attacks<ROOK>(m.to(), occ) & king_bb) return true;
        break;
    case QUEEN:
        if (attacks<QUEEN>(m.to(), occ) & king_bb) return true;
        break;
    case KING:
        return false; // king cannot give check
    default:
        return false;
    }

    // 2. Discovered checks: enemy king on a line with any slider, moving piece vacates that line
    Bitboard sliders = pos.diagonal_sliders<Us>() | pos.orthogonal_sliders<Us>();
    while (sliders) {
        Square sq = pop_lsb(&sliders);
        // If king lies on a ray from slider through 'from' square
        Bitboard between = SQUARES_BETWEEN_BB[sq][their_king];
        if ((between & ~from_bb & occ) == 0) {
            return true;
        }
    }

    // 3. En passant discovered check (special case)
    if (m.flags() == EN_PASSANT) {
        Square cap_sq = m.to() + relative_dir<Us>(SOUTH);
        Bitboard file_bb = SQUARE_BB[cap_sq] | SQUARE_BB[m.from()];
        Bitboard rooks_queens = pos.bitboard_of(Us, ROOK) | pos.bitboard_of(Us, QUEEN);
        if (rooks_queens & attacks<ROOK>(their_king, all & ~file_bb)) return true;

        Bitboard bishops_queens = pos.bitboard_of(Us, BISHOP) | pos.bitboard_of(Us, QUEEN);
        if (bishops_queens & attacks<BISHOP>(their_king, all & ~file_bb)) return true;
    }

    return false;
}


constexpr int 		checkmate_score = 100000;
constexpr int 		stalemate_score = 0;
File 		 		col_to_file(int row, bool reversed);
Rank 		 		row_to_rank(int col, bool reversed);
GameProgress 		get_progress(Position& pos, int mv1, int mv2);
std::string  		get_notation(Position& p, Move& move);
int 		 		detect_checkmate(Position& p);
bool		 		detect_draw(const std::vector<std::string>& fens);
void 		 		log_search_statistics(std::string st,search_statistics search_stats);