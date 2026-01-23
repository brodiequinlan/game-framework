#pragma once
#include <bq.h>
#include "surge/surge.h"
#include "search.hpp"
#include "basic_search.hpp"
#include "mid_search.hpp"
#include "search_v2.hpp"
#include "book.hpp"


enum class search_type {
	BASIC,
	MID,
	FULL,
	V2
};


class pvs_ai {

	static int constexpr DEFAULT_TURN_TIME = 5000;
	static int constexpr DEFAULT_DEPTH = 30;
	
	int m_depth;
	int m_turntime;
	bool waiting = true;

	book m_book;
	pvs_search m_search;
	basic_search m_basic_search;
	mid_search m_mid_search;
	pvs_search_v2 m_v2_search;
	search_type m_search_type;

	bool m_use_book = true;

public:

	pvs_ai(bool use_book, search_type type);
	void signal_stop();
	void notify_book(Move& move);
	void reset();
	void set_turntime(int ms);
	Move get_best_move(Position& p);
	void disable_book();
};