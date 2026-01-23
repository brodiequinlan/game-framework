#include "chess_game.hpp"

chess_game::chess_game(int sq_size, bool* flipped) :
    m_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
    m_square_size(sq_size),
    m_flipped(flipped),
    m_move_sound(bq::resource_holder::get().sounds.get("move.wav")),
    m_ai_white(true, search_type::V2),
    m_ai_black(true, search_type::V2)
{
    load_config();
    m_ai_future = std::async(std::launch::async, [this] {});
}

void chess_game::restart_pos(std::string fen)
{
    m_ai_future.wait();
    m_ai_white.reset();
    m_ai_black.reset();
    m_checkmate = false;
    m_draw = false;
    m_fens.clear();
    m_last_move = Move();
    m_selected_square = { -1,-1 };
    m_pgn = "";
    m_position = Position(fen);
}

void chess_game::restart()
{
    m_ai_future.wait();
    m_ai_white.reset();
    m_ai_black.reset();
    m_checkmate = false;
    m_draw = false;
    m_fens.clear();
    m_last_move = Move();
    m_selected_square = { -1,-1 };
    m_pgn = "";
    m_position = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
void chess_game::restart_p1()
{
    m_player_one = true;
    m_player_two = false;
    restart();
}
void chess_game::restart_p2()
{
    m_player_one = false;
    m_player_two = true;
    restart();
}
void chess_game::undo()
{
    m_ai_white.signal_stop();
    m_ai_black.signal_stop();
    m_ai_future.wait();
    if (m_position.turn() == BLACK) {
        m_position.undo<WHITE>(m_last_move);
        m_position.undo<BLACK>(m_second_last_move);
    }
    else if (m_position.turn() == WHITE) {
        m_position.undo<BLACK>(m_last_move);
        m_position.undo<WHITE>(m_second_last_move);
    }
}
void chess_game::update()
{
    if (m_checkmate || m_draw) { 
        //if(!m_auto_restart) return;
        restart();
    }

    if (!m_player_one && m_position.turn() == WHITE) 
    {
        handle_ai_move<WHITE>(m_ai_future);
    }
    if (!m_player_two && m_position.turn() == BLACK)
    {
        handle_ai_move<BLACK>(m_ai_future);
    }
    if (m_player_clicks.size() == 2) {
        if (m_position.turn() == WHITE) {
            handle_player_move<WHITE>();
        }
        else {
            handle_player_move<BLACK>();
        }
        m_selected_square = { -1,-1 };
        m_player_clicks.clear();
    }
}
void chess_game::handle_event(bq::event& evt)
{
    if (evt.type == bq::event_type::MOUSE) {
        handle_player_clicks(evt.mouse_clicked_pos);
    }
}

std::string chess_game::fen()
{
    return m_position.fen();
}
std::string chess_game::pgn()
{
    return m_pgn;
}
Position chess_game::pos()
{
    return m_position;
}
win_statistics chess_game::stats()
{
    return m_win_stats;
}
bq::v2i chess_game::selected_square()
{
    return m_selected_square;
}
Move chess_game::last_move()
{
    return m_last_move;
}
bool chess_game::player_one()
{
    return m_player_one;
}
bool chess_game::player_two()
{
    return m_player_two;
}

void chess_game::handle_player_clicks(bq::v2f click_pos)
{
    
    if (!m_player_one && m_position.turn() == WHITE) return;
    if (!m_player_two && m_position.turn() == BLACK) return;

    int col = int(click_pos.x / m_square_size);
    int row = int(click_pos.y / m_square_size);

    if (col >= 8) return;

    if (m_selected_square == bq::v2i(row, col)) {
        m_selected_square = { -1,-1 };
        m_player_clicks.clear();
    }
    else {
        m_selected_square = { row, col };
        m_player_clicks.push_back(m_selected_square);
    }
}
void chess_game::check_game_status()
{
    int checkmate_flag = detect_checkmate(m_position);
    if(checkmate_flag != 0){
        m_checkmate = true;
    }
    switch(checkmate_flag){
        case 1:  m_win_stats.white_wins++; bq::logger::info("Checkmate, white wins!"); break;
        case 2:  m_win_stats.stalemates++; bq::logger::info("Stalemate"); break;
        case -1: m_win_stats.black_wins++; bq::logger::info("Checkmate, black wins!");break;
        case -2: m_win_stats.stalemates++; bq::logger::info("Stalemate"); break;
    }
    m_draw = detect_draw(m_fens);
    if(m_draw) {
        bq::logger::info("Draw");
        m_win_stats.draws++;
    }
}
void chess_game::load_config()
{
    std::ifstream t("res/cfg.json");
    std::string str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
    bq::json js = bq::json::parse(str);
    int turn_time = js.get_i("time_per_turn");
    m_ai_white.set_turntime(turn_time);
    m_ai_black.set_turntime(turn_time);
    m_auto_restart = js.get_b("auto_restart");
    m_player_one = js.get_b("player_one");
    m_player_two = js.get_b("player_two");
}