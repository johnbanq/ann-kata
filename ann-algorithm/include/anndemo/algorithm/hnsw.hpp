/**
 * Hierarchical Navigable Small World(HNSW) Algorithm for the ANN problem
 * based on paper: https://arxiv.org/ftp/arxiv/papers/1603/1603.09320.pdf
 */
#ifndef ANNDEMO_ALGORITHM_HNSW_H_
#define ANNDEMO_ALGORITHM_HNSW_H_

#include <vector>
#include <queue>
#include <list>

#include "point.hpp"
#include "undirected_graph.hpp"

namespace ann {

    // parameters //

    struct hnsw;

    /**
     * policy for selecting neighbor,
     * exists to implement the simple-heuristic option in the algorithm
     */
    struct select_neighbor_policy {

        // polymorphic base class 

        select_neighbor_policy() = default;
        virtual ~select_neighbor_policy() = default;

        // value semantics, copy & move ctor & operator= permitted

        virtual std::vector<int> select_neighbors(
            const hnsw& graph,
            const point& q,
            const std::vector<int>& C,
            int M
        ) = 0;

    };

    struct hnsw_parameter {
        int M;
        int Mmax, Mmax0;
        int efCtor;
        double mL;
        std::unique_ptr<select_neighbor_policy> select_neighbor;
    };

    /*
     * a default heuristic that work out the paramters for you
     */
    hnsw_parameter default_parameter(const std::vector<point>& points);


    struct simple_select_neighbor_policy: public select_neighbor_policy {

        std::vector<int> select_neighbors(
            const hnsw& graph,
            const point& q,
            const std::vector<int>& C,
            int M
        );

    };

    struct heuristic_select_neighbor_policy: public select_neighbor_policy {

        bool extendCandidates;

        bool keepPrunedConnections;

        std::vector<int> select_neighbors(
            const hnsw& graph,
            const point& q,
            const std::vector<int>& C,
            int M
        );

    };


    struct hnsw {

        hnsw_parameter param;

        std::vector<point> points;

        //layered adjlist of the graph, bidirectional-ness done by duplicating links
        //prefer adjlist over adjmatrix because I guess the graph should be sparse & ease of adding vertex
        std::vector<undirected_graph> layers;

    };

    /*
     * build the HNSW graph 
     * 
     * @param points the points to be built
     * @param param the params for the hnsw instance
     * 
     * @returns the HNSW graph
     */
    hnsw build_hnsw(std::vector<point> points, const hnsw_parameter& param);

    /**
     * HNSW algorithm for ANN problem
     * 
     * @param graph the preprocessed HNSW graph for searching
     * @param query the point whose K-nearest neighbor to be found
     * @param neighborAmount the K value, amount of neighbor to find
     * 
     * @returns the indexes of the neighbors in the vector that orginally created the graph, 
     *          ordered from nearest to farest
     * 
     * @throws std::invalid_argument on argument error
     */
    std::vector<int> hnsw_knn(const hnsw& graph, const point& query, int neighborAmount);

    // internal functions, unrecommended for direct use //

    std::vector<vertex> hsnw_search_layer(
        const hnsw& hnsw,
        const point& q, 
        std::unordered_set<vertex> ep, int ef, 
        int layer
    );

}

#endif