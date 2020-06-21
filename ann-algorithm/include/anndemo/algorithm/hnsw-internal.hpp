/**
 * internal function definition for the HNSW algorithm
 */
#ifndef ANNDEMO_ALGORITHM_HNSW_INTERNAL_H_
#define ANNDEMO_ALGORITHM_HNSW_INTERNAL_H_

#include <vector>
#include <unordered_set>
#include <queue>
#include <list>

#include "point.hpp"
#include "undirected_graph.hpp"
#include "hnsw.hpp"

namespace ann {

    struct dist_pair_greater {

        bool operator()(const std::pair<vertex, double>& a, const std::pair<vertex, double>& b) const {
            return a.second > b.second;
        }

    };

    struct dist_pair_less {

        bool operator()(const std::pair<vertex, double>& a, const std::pair<vertex, double>& b) const {
            return a.second < b.second;
        }

    };

    //min heap that keeps vertex-distance to q pair
    using min_vertex_dist_queue = std::priority_queue<
        std::pair<vertex, double>, 
        std::vector<std::pair<vertex, double>>,
        dist_pair_less
    >;

    min_vertex_dist_queue hsnw_search_layer(
        const hnsw& hnsw,
        const point& q, 
        std::unordered_set<vertex> ep, int ef, 
        int layer
    );

}

#endif