#include "search_v2.hpp"

void pvs_search_v2::signal_stop()
{
    m_stopping = true;
}

pvs_search_v2::pvs_search_v2(int max_sel_depth)
    : m_max_sel_depth(max_sel_depth)
{

}