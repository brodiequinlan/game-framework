/*
#include <boost/test/included/unit_test.hpp>
#include <bq.h>

BOOST_AUTO_TEST_CASE(test_no_colission_find) {
	bq::astar::pathfinder pf;
    pf.set_size({100,100});
    auto path = pf.pathfind({0,0},{0,10});
    BOOST_CHECK(path->size() == 11);
}

BOOST_AUTO_TEST_CASE(test_basic_colisions_find) {
	bq::astar::pathfinder pf;
    pf.set_size({100,100});

    std::vector<bq::v2i> colisions;

    for(int i = 0; i < 51; ++i)
    {
        colisions.push_back({6, i});
    }
    for(int i = 6; i < 51; ++i)
    {
        colisions.push_back({i, 50});
    }
    for(auto& col : colisions)
    {
        pf.add_collision(col);
    }
    
    auto path = pf.pathfind({5,5},{20,5});

    BOOST_CHECK(path->size() == 170);
}

BOOST_AUTO_TEST_CASE(test_complex_colisions_find) {
	bq::astar::pathfinder pf;
    pf.set_size({100,100});

    std::vector<bq::v2i> colisions;

    for(int i = 0; i <= 90; ++i)
    {
        colisions.push_back({1, i});
    }
    for(int i = 1; i <= 90; ++i)
    {
        colisions.push_back({i, 90});
    }
    for(int i = 90; i >= 60; --i)
    {
        colisions.push_back({90, i});
    }
    for(int i = 90; i >= 20; --i)
    {
        colisions.push_back({i, 60});
    }
    for(int i = 60; i >= 20; --i)
    {
        colisions.push_back({20, i});
    }
    for(int i = 20; i <= 90; ++i)
    {
        colisions.push_back({i, 20});
    }
    for(auto& col : colisions)
    {
        pf.add_collision(col);
    }
    
    auto path = pf.pathfind({0,0},{50,70});
    BOOST_CHECK(path->size() == 409);
}

BOOST_AUTO_TEST_CASE(test_impossible_find) {
	bq::astar::pathfinder pf;
    pf.set_size({100,100});

    std::vector<bq::v2i> colisions{
        {5,6},{5,4}, {4,5},{6,5}
    };
    for(auto& col : colisions)
    {
        pf.add_collision(col);
    }
    

    auto path = pf.pathfind({5,5},{10,10});

    bq::v2i first = path->at(0);
    BOOST_CHECK((first == bq::v2i{5,5})); 
    BOOST_CHECK(path->size() == 1);
}
*/