#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include "catch.hpp"
#include "anndemo/algorithm/hnsw-internal.hpp"
#include "anndemo/format/undirected_graph.hpp"
#include "anndemo/format/common.hpp"

using namespace ann;

TEST_CASE( "simple_select_neighbor_policy works", "[algorithm][hnsw]" ) {
    std::vector<point> points {
        {10,10}, {20,20}, {30,30}, {40,40}
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
    
    auto found = hnsw_search_layer(
        world,
        {15, 15},
        {{1}}, 3,
        0
    );

    auto result = simple_policy()->select_neighbors(
        world,
        {15, 15},
        0,
        found,
        2
    );

    std::vector<vertex> expected = {{0}, {1}};

    CHECK(result == expected);
}

TEST_CASE( "heuristic_select_neighbor_policy works", "[algorithm][hnsw]" ) {
    std::vector<point> points {
        {10,10}, {20,20}, {30,30}, {40,40}
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
    
    auto found = hnsw_search_layer(
        world,
        {15, 15},
        {{1}}, 3,
        0
    );

    auto result = heuristic_policy(true)->select_neighbors(
        world,
        {15, 15},
        0,
        found,
        2
    );

    std::vector<vertex> expected = {{0}, {1}};
    CHECK(result == expected);
}