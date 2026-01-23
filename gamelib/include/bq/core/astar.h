#pragma once
#include <memory>
namespace bq
{
    namespace astar {

        struct node
        {
           
            bq::v2i m_coordinates;
            std::shared_ptr<node> m_parent;

            unsigned m_G, m_H;

            node(bq::v2i coord_, std::shared_ptr<node> parent_ = nullptr);

            unsigned F() const;
        };

        class pathfinder
        {
            std::vector<bq::v2i> m_direction, m_walls;
            bq::v2i m_size;
            unsigned m_directions;

            bool detect_collision(bq::v2i coordinates_);
            unsigned get_heuristic(bq::v2i source_, bq::v2i target_);
            static bq::v2i get_delta(bq::v2i source_, bq::v2i target_);

            std::shared_ptr<node> find_node(std::set<std::shared_ptr<node>>& nodes_, bq::v2i coordinates_);
        public:
            pathfinder();
            void set_size(bq::v2i worldSize_);
            void set_diagonal(bool enable_);
            void add_collision(bq::v2i coordinates_);

            std::unique_ptr<std::vector<bq::v2i>> pathfind(bq::v2i source_, bq::v2i target_);
        };
    }
}