#include "chess_ai.hpp"

pvs_ai::pvs_ai(bool use_book, search_type type) :
	m_turntime(DEFAULT_TURN_TIME),
	m_depth(DEFAULT_DEPTH),
	m_search(DEFAULT_DEPTH),
	m_v2_search(DEFAULT_DEPTH),
	m_mid_search(DEFAULT_DEPTH),
	m_use_book(use_book),
	m_search_type(type)
{
	
}

void pvs_ai::disable_book()
{
	m_use_book = false;
}

void pvs_ai::signal_stop()
{
	m_basic_search.signal_stop();
    m_search.signal_stop();
	m_mid_search.signal_stop();
	m_v2_search.signal_stop();
    waiting=false;
}

void pvs_ai::notify_book(Move& move)
{
    m_book.add_move(move);
}

void pvs_ai::reset()
{
    signal_stop();
    waiting=false;
    m_book.reset();
}
void pvs_ai::set_turntime(int ms)
{
    m_turntime = ms;
}
Move pvs_ai::get_best_move(Position& p)
{
	if (p.turn() == WHITE && m_search_type != search_type::BASIC && m_use_book) {
		Move bookMove = m_book.get_book_move<WHITE>(p);
		if (bookMove.str() != "a1a1") {
#if ANALYTICS_VERBOSE
			bq::logger::info("------------------------------------------");
			bq::logger::info("Book selection - " + bookMove.str_d());
			bq::logger::info("------------------------------------------");
#endif
			return bookMove;
		}
	}
	else {
		if ((m_search_type != search_type::BASIC) && m_use_book)
		{
			Move bookMove = m_book.get_book_move<BLACK>(p);
			if (bookMove.str() != "a1a1") {
#if ANALYTICS_VERBOSE 
				bq::logger::info("------------------------------------------");
				bq::logger::info("Book selection - " + bookMove.str_d());
				bq::logger::info("------------------------------------------");
#endif
				return bookMove;
			}
		}
	}

	if (p.turn() == WHITE) {
		waiting = true;
		call_after( std::bind(&pvs_ai::signal_stop,this), m_turntime);
		Move move;
		if (m_search_type == search_type::FULL)
		{
			move = m_search.initiate_iterative_search<WHITE>(p, m_depth);
		}
		else if (m_search_type == search_type::BASIC)
		{
			move = m_basic_search.initiate_iterative_search<WHITE>(p, m_depth);
		}
		else if (m_search_type == search_type::MID)
		{
			move = m_mid_search.initiate_iterative_search<WHITE>(p, m_depth);
		}
		else if (m_search_type == search_type::V2)
		{
			move = m_v2_search.initiate_iterative_search<WHITE>(p, m_depth);
		}
		while(waiting) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);
		}

		return move;
	}
	else {
		waiting = true;
		call_after( std::bind(&pvs_ai::signal_stop,this), m_turntime);
		
		Move move;
		if (m_search_type == search_type::FULL)
		{
			move = m_search.initiate_iterative_search<BLACK>(p, m_depth);
		}
		else if (m_search_type == search_type::BASIC)
		{
			move = m_basic_search.initiate_iterative_search<BLACK>(p, m_depth);
		}
		else if (m_search_type == search_type::MID)
		{
			move = m_mid_search.initiate_iterative_search<BLACK>(p, m_depth);
		}
		else if (m_search_type == search_type::V2)
		{
			move = m_v2_search.initiate_iterative_search<BLACK>(p, m_depth);
		}

		while(waiting){
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);
		}
		return move;
	}
}