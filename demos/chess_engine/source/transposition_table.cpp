#include "transposition_table.hpp"


transposition_table::transposition_table()
{
    m_invalid_entry.valid = false;
}

void transposition_table::clear()
{
    m_table.clear();
}
void transposition_table::insert(uint64_t hash, tt_entry entry)
{
    auto it = m_table.find(hash);
    if (it == m_table.end() || entry.depth >= it->second.depth)
    {
        m_table[hash] = entry;
    }
}
tt_entry transposition_table::lookup(uint64_t hash) 
{
    auto it = m_table.find(hash);
    if (it != m_table.end())
        return it->second;
    return m_invalid_entry;
}
void transposition_table::set_top_move(Move m)
{
    m_top_move = m;
}
Move transposition_table::selected_move() 
{
    return m_top_move;
}