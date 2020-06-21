#include <cstdlib>
#include <stdexcept>
#include "catch.hpp"
#include "anndemo/algorithm/brute-force.hpp"


TEST_CASE( "brute-force works", "[algorithm]" ) {
    const std::vector<ann::point> points {
        {10, 10},
        {20, 20},
        {30, 30}
    };
    const std::vector<int> expected {
        0, 1
    };
    auto result = ann::brute_force_knn(points, {5,5}, 2);
    
    REQUIRE(result == expected);
}

TEST_CASE( "brute-force rejects invalid parameter", "[algorithm]" ) {
    const std::vector<ann::point> points {
        {10, 10},
        {20, 20},
        {30, 30}
    };
    const std::vector<int> expected {
        0, 1
    };

    CHECK_THROWS_AS(ann::brute_force_knn(points, {5,5}, -1), std::invalid_argument);
    CHECK_THROWS_AS(ann::brute_force_knn(points, {5,5}, 10), std::invalid_argument);
}