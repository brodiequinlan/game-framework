#include "mid_search.hpp"

void mid_search::signal_stop()
{
    m_stopping = true;
}

mid_search::mid_search(int max_sel_depth)
    : m_max_sel_depth(max_sel_depth)
{

}