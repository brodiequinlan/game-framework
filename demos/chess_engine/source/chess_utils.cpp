#include "chess_utils.hpp"


void search_statistics::reset()
{
	depth 					  = 0;
	score 					  = 0;
	ellapsed_ms 			  = 0;
	nodes_searched 			  = 0;
	quinscience_depth_reached = 0;
	mate_found = false;
}

File col_to_file(int row, bool reversed)
{
	if (reversed) {
		File f = AFILE;
		switch (row) {
		case 7: f = File::AFILE; break;
		case 6: f = File::BFILE; break;
		case 5: f = File::CFILE; break;
		case 4: f = File::DFILE; break;
		case 3: f = File::EFILE; break;
		case 2: f = File::FFILE; break;
		case 1: f = File::GFILE; break;
		case 0: f = File::H_FILE; break;
		}
		return f;
	}
	File f = AFILE;
	switch (row) {
		case 0: f = File::AFILE; break;
		case 1: f = File::BFILE; break;
		case 2: f = File::CFILE; break;
		case 3: f = File::DFILE; break;
		case 4: f = File::EFILE; break;
		case 5: f = File::FFILE; break;
		case 6: f = File::GFILE; break;
		case 7: f = File::H_FILE; break;
	}
	return f;
}
Rank row_to_rank(int col, bool reversed)
{
	if (reversed) {
		Rank r = RANK8;
		switch (col) {
		case 7: r = Rank::RANK8; break;
		case 6: r = Rank::RANK7; break;
		case 5: r = Rank::RANK6; break;
		case 4: r = Rank::RANK5; break;
		case 3: r = Rank::RANK4; break;
		case 2: r = Rank::RANK3; break;
		case 1: r = Rank::RANK2; break;
		case 0: r = Rank::RANK1; break;
		}
		return r;
	}
	Rank r = RANK8;
	switch (col) {
	case 0: r = Rank::RANK8; break;
	case 1: r = Rank::RANK7; break;
	case 2: r = Rank::RANK6; break;
	case 3: r = Rank::RANK5; break;
	case 4: r = Rank::RANK4; break;
	case 5: r = Rank::RANK3; break;
	case 6: r = Rank::RANK2; break;
	case 7: r = Rank::RANK1; break;
	}
	return r;
}
GameProgress get_progress(Position& pos, int mv1, int mv2)
{
	bool no_queens =
		!pos.bitboard_of(WHITE, QUEEN) &&
		!pos.bitboard_of(BLACK, QUEEN);

	return (no_queens && mv1 <= 1800 && mv2 <= 1800)
		? ENDGAME
		: MIDGAME;
}
std::string get_notation(Position& p, Move& move)
{
	if (move.is_promotion()) {
		return  move.str().substr(2,3) + "Q";
	}
	
	if (move.is_castling())
		if (move.to() == h1 || move.to() == h8)
			return "0-0";
		else
			return "0-0-0";
	if (move.flags() == MoveFlags::EN_PASSANT) {
		return move.str().substr(0,1) + "x" + move.str().substr(2, 3) + " e.p.";
	}
	if (move.is_capture())
		if (p.at(move.from()) == Piece::BLACK_PAWN || p.at(move.from()) == Piece::WHITE_PAWN) {
			return  move.str().substr(0, 1) + "x" + move.str().substr(2, 3);
		}
		else {
			auto piece = p.at(move.from());
			std::string piece_letter = "";
			switch (piece) {
				case WHITE_BISHOP: piece_letter = "B"; break;
				case BLACK_BISHOP: piece_letter = "B"; break;
				case WHITE_ROOK: piece_letter = "R"; break;
				case BLACK_ROOK: piece_letter = "R"; break;
				case WHITE_KNIGHT: piece_letter = "N"; break;
				case BLACK_KNIGHT: piece_letter = "N"; break;
				case WHITE_QUEEN: piece_letter = "Q"; break;
				case BLACK_QUEEN: piece_letter = "Q"; break;
				case WHITE_KING: piece_letter = "K"; break;
				case BLACK_KING: piece_letter = "K"; break;
			}
			return piece_letter + "x" + move.str().substr(2, 3);
		}
	else {
		if (p.at(move.from()) == Piece::BLACK_PAWN || p.at(move.from()) == Piece::WHITE_PAWN) {
			return move.str().substr(2,3);
		}
		
		else {
			auto piece = p.at(move.from());
			std::string piece_letter = "";
			switch (piece) {
				case WHITE_BISHOP: piece_letter = "B"; break;
				case BLACK_BISHOP: piece_letter = "B"; break;
				case WHITE_ROOK: piece_letter = "R"; break;
				case BLACK_ROOK: piece_letter = "R"; break;
				case WHITE_KNIGHT: piece_letter = "N"; break;
				case BLACK_KNIGHT: piece_letter = "N"; break;
				case WHITE_QUEEN: piece_letter = "Q"; break;
				case BLACK_QUEEN: piece_letter = "Q"; break;
				case WHITE_KING: piece_letter = "K"; break;
				case BLACK_KING: piece_letter = "K"; break;
			}
			return piece_letter + move.str().substr(2, 3);
		}
		
	}
}
int detect_checkmate(Position& p)
{

	if (p.turn() == WHITE) {
		MoveList<WHITE> moves(p);
		if (moves.size() == 0) {
			if (p.in_check<WHITE>())
			{
				return -1;
			}
			else
			{
				return -2;
			}
		}
		return 0;
	}
	else {
		MoveList<BLACK> moves(p);
		if (moves.size() == 0) {
			if (p.in_check<BLACK>())
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
		return 0;
	}
}
bool detect_draw(const std::vector<std::string>& fens)
{
	if(fens.size() == 0) return false;

	std::string recent_position = fens[fens.size() -1];

	int found_times = 0;

	for(int i = 0; i < fens.size()-1; ++i)
	{
		if(fens[i] == recent_position) found_times++;
	}

	return found_times>2;
}
void log_search_statistics(std::string st, search_statistics search_stats)
{
	float millions_of_nodes = roundf((float(search_stats.nodes_searched) / 1000000.0f) * 100) / 100;
	float mnps = float(search_stats.nodes_searched) / float(search_stats.ellapsed_ms);
	bq::logger::info("----------------------------------------------------------------------------------------------------------------------------");
	bq::logger::info(st);
	bq::logger::info("Depth Searched: " + std::to_string(search_stats.depth) + " - Nodes Searched : " + format_decimal(millions_of_nodes) + "M(" + std::to_string(search_stats.ellapsed_ms / 1000000) + "S)(" + format_decimal(mnps) + "MN / S) - Selective Depth reached : " + std::to_string(search_stats.quinscience_depth_reached) + " - Move Selection : " + search_stats.selected_move.str_d() + " - " + std::to_string(search_stats.score));
	bq::logger::info("----------------------------------------------------------------------------------------------------------------------------");
}