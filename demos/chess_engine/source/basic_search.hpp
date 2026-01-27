#pragma once
#pragma once
#include <bq.h>
#include "surge/surge.h"
#include "chess_utils.hpp"
#include "evaluator.hpp"
#include "transposition_table.hpp"
#include "move_ordering.hpp"
#include <future>

#define ANALYTICS_VERBOSE true
#define ANALYTICS_MINIMAL true



class basic_search {

	transposition_table m_tt;
	search_statistics m_search_statistics;
	bool m_stopping = false;
	int m_max_sel_depth;

public:


	void signal_stop()
	{
		m_stopping = true;
	}


	template <Color us>
	Move initiate_iterative_search(Position& p, int depth)
	{
		m_search_statistics.reset();
		m_stopping = false;
		Move sel_move;
		for (int i = 1; i <= depth; ++i)
		{
			sel_move = m_search_statistics.selected_move;
			initiate_search<us>(p, i);
			m_search_statistics.depth = i;
			if (m_stopping || m_search_statistics.mate_found) break;
		}
		log_search_statistics("Basic Search",m_search_statistics);
		return sel_move;
	}

private:

	template <Color us>
	void initiate_search(Position& p, int depth)
	{
		m_search_statistics.depth = depth;
		auto start = std::chrono::high_resolution_clock::now();
		int score = pvs(p, depth, -checkmate_score, checkmate_score);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<std::chrono::microseconds>(stop - start);
		m_search_statistics.ellapsed_ms += duration.count();
	}

	


	int pvs(Position p, int depth, int alpha, int beta) {

		if (depth == 0) {
			return score_board<WHITE>(p);
		}

		if (m_stopping) return 0;


		if (p.turn() == WHITE) {

			int max_score = -checkmate_score;
			MoveList<WHITE> moves(p);
			for (Move move : moves) {
				p.play<WHITE>(move);
				m_search_statistics.nodes_searched++;
				int score = pvs(p, depth - 1, alpha, beta);
				p.undo<WHITE>(move);
				if (m_stopping) return 0;
				if (score > max_score) {
					max_score = score;
					if (depth == m_search_statistics.depth) {
						m_search_statistics.selected_move = move;
						m_search_statistics.score = score;
					}

				}
				if (max_score > beta)
					break;
				alpha = std::max(alpha, max_score);

			}
			return max_score;
		}
		else if (p.turn() == BLACK) {
			int min_score = checkmate_score;
			MoveList<BLACK> moves(p);
			for (Move move : moves) {
				p.play<BLACK>(move);
				m_search_statistics.nodes_searched++;
				int score = pvs(p, depth - 1, alpha, beta);
				p.undo<BLACK>(move);

				if (score < min_score) {
					min_score = score;
					if (depth == m_search_statistics.depth) {
						m_search_statistics.selected_move = move;
						m_search_statistics.score = score;
					}
				}
				if (min_score < alpha)
					break;
				beta = std::min(beta, min_score);


			}
			return min_score;
		}
		return -9999999;
	}
};