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



class pvs_search {

	transposition_table m_tt;
	search_statistics m_search_statistics;
	bool m_stopping = false;
	int m_max_sel_depth;

public:

	pvs_search(int max_sel_depth);

	void signal_stop();

	template <Color us>
	Move initiate_iterative_search(Position& p, int depth)
	{
		m_search_statistics.reset();
		m_stopping = false;
		m_tt.clear();
		Move sel_move;
		for (int i = 1; i <= depth; ++i)
		{
			initiate_search<us>(p, i);
			m_search_statistics.depth = i;
			if (m_stopping || m_search_statistics.mate_found) break;
		}
		log_search_statistics("Full search",m_search_statistics);
		return m_search_statistics.selected_move;
	}

private:
	
	template <Color us>
	void initiate_search(Position& p, int depth)
	{
		m_search_statistics.depth = depth;
		auto start = std::chrono::high_resolution_clock::now();
		pvs<us>(p, depth, -checkmate_score, checkmate_score,false);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<std::chrono::microseconds>(stop - start);
		m_search_statistics.ellapsed_ms += duration.count();
	}

	template<Color us>
	int quiescence(Position& p, int depth, int alpha, int beta)
	{

		if (m_stopping) return 0;

		m_search_statistics.quinscience_depth_reached = std::max(depth, m_search_statistics.quinscience_depth_reached);
		
		int score = score_board<us>(p);

		if (score >= beta || depth == m_max_sel_depth) return score;
		if (alpha < score) alpha = score;

		MoveList<us> moves(p);
		
		order_moves<us>(p, moves, m_tt,false);
		for (Move& move : moves)
		{
			if (!move.is_capture() || ((score + piece_values[type_of(p.at(move.to()))] + 200 < alpha))) continue;

			p.play<us>(move);
			m_search_statistics.nodes_searched++;
			int score = -quiescence<~us>(p, depth + 1, -beta, -alpha);
			p.undo<us>(move);

			
			if (score >= beta) return beta;
			if (score > alpha) alpha = score;
		}
		return alpha;
	}

	
	//chad 1990s search
	template <Color us>
	int pvs(Position& p, int depth, int alpha, int beta, bool reduced)
	{
		if (m_stopping) return 0;

		int orig_alpha = alpha;

		auto tt_lookup = m_tt.lookup(p.get_hash());

		if (tt_lookup.valid && tt_lookup.score != checkmate_score &&
			tt_lookup.score != -checkmate_score &&
			depth != m_search_statistics.depth)
		{
			if (tt_lookup.depth >= depth) 
			{
				if (tt_lookup.flag == tt_flag::EXACT) return tt_lookup.score;
				else if (tt_lookup.flag == tt_flag::LOWERBOUND) alpha = std::max(alpha, tt_lookup.score);
				else if (tt_lookup.flag == tt_flag::UPPERBOUND) beta = std::min(beta, tt_lookup.score);
			}
		}
		if (depth <= 0) return quiescence<us>(p, 0, alpha, beta);

		bool is_pv = alpha != beta - 1;

		if (depth <= 3 && m_search_statistics.depth >= 7)
		{
			int s_eval = score_board<us>(p);
			//razering
			if (s_eval + 220 * depth < beta)
			{
				int score = quiescence<us>(p, 0, alpha, beta);
				if (score < beta) return score;
			}
			// Reverse futility pruning
			if (!is_pv && !p.in_check<us>())
			{
				if (s_eval - (150 * depth) >= beta) return s_eval;
			}
			
		}


		MoveList<us> moves(p);
		order_moves<us>(p, moves, m_tt, depth==m_search_statistics.depth);

		if (moves.size() == 0) 
		{
			if (p.in_check<us>()) return -checkmate_score;
			else 				  return 0;
		}

		int move_num = 0;

		for (Move& move : moves) 
		{
			p.play<us>(move);
			m_search_statistics.nodes_searched++;
			int move_reduct = 0;
			if (!is_pv && move_num > 3 && moves.size() >= 4 && depth >= 3 && !reduced && !move.is_capture()) move_reduct = 1;
			int score;
			if (move_num == 0 || p.in_check<us>())
			{
				score = -pvs<~us>(p, (depth - 1), -beta, -alpha,reduced);
			}
			else 
			{
				score = -pvs<~us>(p, (depth - 1) - move_reduct, -alpha - 1, -alpha, move_reduct>0);
				if (score > alpha && move_reduct > 0) score = -pvs<~us>(p, (depth - 1), -alpha - 1, -alpha, reduced);
				if (score > alpha && score < beta) 	  score = -pvs<~us>(p, (depth - 1), -beta, -alpha,reduced);
			}
			p.undo<us>(move);

			if(m_stopping) return 0;

			if (score > alpha)
			{
				alpha = score;
				if (depth == m_search_statistics.depth) 
				{
					m_search_statistics.selected_move = move;
					m_search_statistics.score = score;
					m_tt.move_scores[move.str_d()] = score;
					m_tt.set_top_move(move);
					if (score == checkmate_score) m_search_statistics.mate_found = true;
				}
			}
			else {
				if (depth == m_search_statistics.depth) m_tt.move_scores[move.str_d()] = score;
			}
			if (score >= beta) return beta;
			++move_num;
		}
		tt_entry entry;
		entry.valid = true;
		entry.score = alpha;
		entry.depth = depth;
		if (alpha <= orig_alpha) entry.flag = tt_flag::UPPERBOUND;
		else if (alpha >= beta)  entry.flag = tt_flag::LOWERBOUND;
		else 					 entry.flag = tt_flag::EXACT;
		m_tt.insert(p.get_hash(), entry);
		return alpha;
	}
};