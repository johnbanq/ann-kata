#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include "catch.hpp"
#include "anndemo/algorithm/hnsw.hpp"
#include "anndemo/algorithm/brute-force.hpp"
#include "anndemo/format/undirected_graph.hpp"
#include "anndemo/format/common.hpp"

using namespace ann;


std::random_device r;

std::default_random_engine engine(r());

std::uniform_int_distribution<int> dist(-10000, 10000);


std::vector<point> generate_points(int amount) {
    std::vector<point> result;
    for(int i = 0; i < amount; i++) {
        result.emplace_back(dist(engine), dist(engine));
    }
    return result;
}

TEST_CASE( "hnsw/brute-force cross-ref validation", "[algorithm][hnsw]" ) {
    for(int i=0;i<100;i++) {
        auto points = generate_points(1000);
        point query {dist(engine), dist(engine)};

        auto p = ann::default_parameter(points);
        auto world = ann::build_hnsw(points, std::move(p));
        //ef=16 is arbitrary
        auto result = ann::hnsw_knn(world, query, 10, 16);
        
        auto brute = ann::brute_force_knn(points, query, 10);

        std::vector<int> diff; 
        std::sort(result.begin(), result.end());
        std::sort(brute.begin(), brute.end());
        std::set_intersection(result.begin(), result.end(), brute.begin(), brute.end(), std::back_inserter(diff));
        if(diff.size()<8) {
            INFO("unacceptable difference detected");
            CHECK(result == brute);   
        } else {
            //so we can have the total assertion count as test count
            CHECK(true);
        }
    }
}
