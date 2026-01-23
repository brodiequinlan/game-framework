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

constexpr int 		checkmate_score = 100000;
constexpr int 		stalemate_score = 0;
constexpr uint32_t  max_tt_size = 10000000;
File 		 		col_to_file(int row, bool reversed);
Rank 		 		row_to_rank(int col, bool reversed);
GameProgress 		get_progress(Position& pos, int mv1, int mv2);
std::string  		get_notation(Position& p, Move& move);
int 		 		detect_checkmate(Position& p);
bool		 		detect_draw(const std::vector<std::string>& fens);
void 		 		log_search_statistics(std::string st,search_statistics search_stats);