#pragma once
#include <bq.h>

#include "surge/surge.h"
#include "chess_utils.hpp"

enum class tt_flag {
	EXACT,
	UPPERBOUND,
	LOWERBOUND
};

struct tt_entry {
	int depth;
	int score;
	tt_flag flag;
	bool valid;
};

class transposition_table {

	Move m_top_move;
	std::unordered_map<uint64_t,tt_entry> m_table;

	tt_entry m_invalid_entry;

	std::mutex m_insertion_mutex;

public:
	std::unordered_map<std::string, int> move_scores;

	static constexpr size_t MAX_TT_SIZE = 4'000'000; // ~4M entries

	transposition_table();

	Move 		selected_move();
	tt_entry    lookup(uint64_t hash);
	void 	 	set_top_move(Move m);
	void 		insert(uint64_t hash, tt_entry entry);
	void 		clear();
};