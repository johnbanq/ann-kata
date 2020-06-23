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
//we put the policy related definition to somewhere else for brevity
#include "hnsw-policy.hpp"

namespace ann {

    // parameters //

    struct hnsw;

    // select neighbor policies //

    std::unique_ptr<select_neighbor_policy> simple_policy();

    //disabled extendCandidates by default, you only need it for extremely clustered data
    std::unique_ptr<select_neighbor_policy> heuristic_policy(
        bool keepPrunedConnections
    );

    std::unique_ptr<select_neighbor_policy> heuristic_policy(
        bool keepPrunedConnections,
        bool extendCandidates
    );

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

    // algorithm //

    struct hnsw {

        hnsw_parameter param;

        std::vector<point> points;

        //layered adjlist of the graph, bidirectional-ness done by duplicating links
        //prefer adjlist over adjmatrix because I guess the graph should be sparse & ease of adding vertex
        //@note each layer graph must store all vertexes instead of the realvent subset, this is a design flaw
        std::vector<undirected_graph> layers = {};

        vertex enter_point = {0};

        //the max layer of a point
        std::vector<unsigned int> pointMaxLayers = {};

    };

    /*
     * build the HNSW graph 
     * 
     * @param points the points to be built
     * @param param the params for the hnsw instance
     * 
     * @returns the HNSW graph
     */
    hnsw build_hnsw(std::vector<point> points, hnsw_parameter param);

    /**
     * HNSW algorithm for ANN problem
     * 
     * @param graph the preprocessed HNSW graph for searching
     * @param query the point whose K-nearest neighbor to be found
     * @param neighborAmount the K value, amount of neighbor to find
     * @param ef the algorithm's parameter
     * 
     * @returns the indexes of the neighbors in the vector that orginally created the graph, 
     *          ordered from nearest to farest
     * 
     * @throws std::invalid_argument on argument error
     */
    std::vector<int> hnsw_knn(const hnsw& graph, const point& query, int neighborAmount, int ef);

}

#endif