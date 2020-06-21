#include <cstdlib>
#include <stdexcept>
#include "catch.hpp"
#include "anndemo/algorithm/hnsw.hpp"

using namespace ann;

TEST_CASE( "search_layer works on naive case", "[algorithm][hnsw]" ) {
    std::vector<point> points {
        {10,10},
        {20,20},
        {30,30},
        {40,40}
    };

    hnsw world {
        ann::default_parameter(points),
        points,
        {
            //layer 0
            {
                {0, 1},
                {1, 2},
                {2, 3}
            }
        }
    };
    
    auto result = hsnw_search_layer(
        world,
        {15, 15},
        {{3}}, 2,
        0
    );

    std::vector<vertex> expected = {{0}, {1}};

    CHECK(result == expected);
}
