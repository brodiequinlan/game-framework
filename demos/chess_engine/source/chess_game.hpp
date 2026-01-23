#include <bq.h>
#include "surge/surge.h"
#include "chess_ai.hpp"
#include "search.h"

struct win_statistics{
	int white_wins=0;
	int black_wins=0;
	int draws=0;
	int stalemates=0;
};


class chess_game {

    bool m_player_one = true;
	bool m_player_two = true;
	bool m_checkmate = false;
	bool m_draw = false;
	bool m_auto_restart = true;
	bool* m_flipped;
	int m_square_size;

	std::vector<std::string> m_fens;
	std::vector<bq::v2i> m_player_clicks;
	std::string m_pgn;
	std::future<void> m_ai_future;
	
    Position m_position;
    Move m_last_move;
	Move m_second_last_move;

	pvs_ai m_ai_white;
	pvs_ai m_ai_black;
    win_statistics m_win_stats;
	
	bq::v2i m_selected_square = { -1,-1 };
	sf::Sound m_move_sound;

public:
    
	chess_game(int sq_size, bool* flipped);

	void 			restart_pos(std::string fen);
    void 			restart();
    void 			restart_p1();
    void 			restart_p2();
    void 			undo();
    void 			update();
    void 			handle_event(bq::event& evt);
    std::string 	fen();
    std::string 	pgn();
    Position 		pos();
    win_statistics  stats();
    bq::v2i 		selected_square();
    Move 			last_move();
    bool 			player_one();
	bool 			player_two();

private:

	void 		    handle_player_clicks(bq::v2f click_pos);
    void 			check_game_status();
	void 			load_config();

	//TEMPLATED PRIVATE FUNCTIONS

	template <Color Us>
	void handle_player_move()
	{
		MoveList<Us> list(m_position);
		for (Move move : list) {
			if (move.to() == create_square(col_to_file(m_player_clicks[1].y,*m_flipped), row_to_rank(m_player_clicks[1].x,*m_flipped)) && move.from() == create_square(col_to_file(m_player_clicks[0].y,*m_flipped), row_to_rank(m_player_clicks[0].x,*m_flipped))) {
				if (move.flags() == MoveFlags::PR_KNIGHT) continue;
				if (move.flags() == MoveFlags::PR_BISHOP) continue;
				if (move.flags() == MoveFlags::PR_ROOK) continue;
				play_move<Us>(move);
				break;
			}
		}
	}
    template<Color Us>
	void play_move(Move& move)
	{
		m_position.play<Us>(move);
		m_fens.push_back(m_position.fen());
		m_move_sound.play();
		m_ai_white.notify_book(move);
		m_ai_black.notify_book(move);
		m_second_last_move = m_last_move;
		m_last_move = move;
		if (Us == BLACK) {
			m_pgn +=  get_notation(m_position, move) + " ";
		}
		else {
			m_pgn += std::to_string(std::max(1,m_position.ply()+1 / 2)) + ". " + get_notation(m_position, move) + " ";
		}
		check_game_status();
	}
	template <Color Us>
	void handle_ai_move(std::future<void>& future)
	{
		using namespace std::chrono_literals;
		auto status = future.wait_for(0ms);

		if (status == std::future_status::ready)
		{
			future = std::async(std::launch::async, [this]
			{
				Position p(m_position.fen());
				if(Us == BLACK)
				{
					auto m = m_ai_black.get_best_move(p);
					play_move<Us>(m);
				}
				else
				{
					auto m = m_ai_white.get_best_move(p);
					play_move<Us>(m);
				}
					
			});
		}
	}
};