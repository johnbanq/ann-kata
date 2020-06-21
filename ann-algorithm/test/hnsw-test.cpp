#include <cstdlib>
#include <stdexcept>
#include "catch.hpp"
#include "anndemo/algorithm/hnsw-internal.hpp"

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
    
    auto found = hsnw_search_layer(
        world,
        {15, 15},
        {{3}}, 2,
        0
    );

    std::vector<vertex> expected = {{0}, {1}};

    std::vector<vertex> result(found.size());
    for(int i=(int)found.size()-1; i >= 0 ; i--) {
        result[i] = found.top().first;
        found.pop();
    }
    
    CHECK(result == expected);
}