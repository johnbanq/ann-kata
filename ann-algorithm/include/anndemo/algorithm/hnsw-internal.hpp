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

    //max heap that keeps vertex-distance to q pair
    using max_vertex_dist_queue = std::priority_queue<
        std::pair<vertex, double>, 
        std::vector<std::pair<vertex, double>>,
        dist_pair_less
    >;

    max_vertex_dist_queue hsnw_search_layer(
        const hnsw& hnsw,
        const point& q, 
        std::unordered_set<vertex> ep, int ef, 
        int layer
    );

    // select neighbor //

        /**
     * policy for selecting neighbor,
     * exists to implement the simple-heuristic option in the algorithm
     */
    struct select_neighbor_policy {

        // polymorphic base class 

        select_neighbor_policy() = default;
        virtual ~select_neighbor_policy() = default;

        // value semantics, copy & move ctor & operator= permitted

        virtual std::vector<vertex> select_neighbors(
            const hnsw& graph,
            const point& q,
            max_vertex_dist_queue C,
            unsigned int M
        ) = 0;

    };

    struct simple_select_neighbor_policy: public select_neighbor_policy {

        virtual std::vector<vertex> select_neighbors(
            const hnsw& graph,
            const point& q,
            max_vertex_dist_queue C,
            unsigned int M
        );

    };

    struct heuristic_select_neighbor_policy: public select_neighbor_policy {

        bool extendCandidates;

        bool keepPrunedConnections;

        heuristic_select_neighbor_policy(bool extendCandidates, bool keepPrunedConnections)
            :extendCandidates(extendCandidates), keepPrunedConnections(keepPrunedConnections) {}

        virtual std::vector<vertex> select_neighbors(
            const hnsw& graph,
            const point& q,
            max_vertex_dist_queue C,
            unsigned int M
        );

    };

}

#endif