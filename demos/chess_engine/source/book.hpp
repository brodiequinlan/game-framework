#pragma once

#include "surge/surge.h"
#include "chess_utils.hpp"
#include "utils.hpp"
class book {

    std::vector<Move> m_move_history;

    struct GameEntry {
        std::vector<std::string> moves; // your original move strings
        int result; // 1 = white win, -1 = black win, 0 = draw
    };

    std::vector<GameEntry> m_games;

public:

    book()
    {
        std::ifstream t("res/books/book2.txt");
        if (!t.is_open()) {
            bq::logger::warn("Could not open book file");
            return;
        }

        std::stringstream buffer;
        buffer << t.rdbuf();
        std::string file_contents = buffer.str();
        auto games = split(file_contents, "\n");

        for (auto& game : games) {
            if (game.empty()) continue;

            Position p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            auto tokens = split(game, " ");

            if (tokens.empty()) continue;

            // Parse game result from last token
            std::string last_token = tokens.back();
            int result = 0;
            if (last_token == "1-0") result = 1;
            else if (last_token == "0-1") result = -1;
            else if (last_token == "1/2-1/2") result = 0;

            if (last_token == "1-0" || last_token == "0-1" || last_token == "1/2-1/2")
                tokens.pop_back(); // remove result

            // ======= KEEP ORIGINAL MOVE-LOADING LOGIC =======
            std::vector<std::string> updated_tokens;
            for (auto& token : tokens) {
                if (p.turn() == WHITE) {
                    MoveList<WHITE> moves(p);
                    for (Move move : moves) {
                        if (get_notation(p, move) == token) {
                            updated_tokens.push_back(move.str());
                            p.play<WHITE>(move);
                        }
                    }
                }
                else {
                    MoveList<BLACK> moves(p);
                    for (Move move : moves) {
                        if (get_notation(p, move) == token) {
                            updated_tokens.push_back(move.str());
                            p.play<BLACK>(move);
                        }
                    }
                }
            }
            // ==================================================

            if (!updated_tokens.empty()) {
                m_games.push_back({ updated_tokens, result });
            }
        }

        // Shuffle games
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(m_games.begin(), m_games.end(), g);

        bq::logger::info("games loaded from book: " + std::to_string(m_games.size()));
    }

    void reset()
    {
        m_move_history.clear();
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(m_games.begin(), m_games.end(), g);
    }

    void add_move(Move move)
    {
        m_move_history.push_back(move);
    }

    template <Color Us>
    Move get_book_move(Position& p)
    {
        std::vector<Move> candidates;

        for (auto& game : m_games) {
            // ====== Only filter by winning games here ======
            if (Us == WHITE) {
                if (game.result != 1) continue;
            }
            else {
                if (game.result != -1) continue;
            }

            if (m_move_history.size() >= game.moves.size()) continue;

            bool match = true;
            for (size_t i = 0; i < m_move_history.size(); ++i) {
                if (m_move_history[i].str() != game.moves[i]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                auto next_move_str = game.moves[m_move_history.size()];
                MoveList<Us> moves(p);
                for (auto& move : moves) {
                    if (move.str() == next_move_str) {
                        candidates.push_back(move);
                    }
                }
            }
        }

        if (candidates.empty()) return Move();

        static thread_local std::mt19937 g(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
        return candidates[dist(g)];
    }
};