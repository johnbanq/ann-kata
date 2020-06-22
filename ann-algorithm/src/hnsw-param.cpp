#include<cmath>
#include "anndemo\algorithm\hnsw-internal.hpp"


using namespace ann;

hnsw_parameter ann::default_parameter(const std::vector<point>& points) {
    int M = 12;
    int Mmax = M;
    int Mmax0 = 2*M;
    int efCtor = 100;
    double mL = 1/log(M);
    return hnsw_parameter {
        M,
        Mmax,
        Mmax0,
        efCtor,
        mL,
        simple_policy()
    };
}

std::unique_ptr<select_neighbor_policy> ann::simple_policy() {
    return std::make_unique<simple_select_neighbor_policy>();
}

std::unique_ptr<select_neighbor_policy> ann::heuristic_policy(
    bool extendCandidates,
    bool keepPrunedConnections        
) {
    return std::make_unique<heuristic_select_neighbor_policy>(extendCandidates, keepPrunedConnections);
}



std::vector<vertex> ann::simple_select_neighbor_policy::select_neighbors(
    const hnsw& graph,
    const point& q,
    max_vertex_dist_queue C,
    unsigned int M
) {
    if(M>C.size()) {
        throw std::invalid_argument("error.invalid_M");
    }

    //we had to construct the min heap for extraction
    //TODO: this is optimizable by directly make_heap on underlying vector
    std::priority_queue<
        std::pair<vertex, double>, 
        std::vector<std::pair<vertex, double>>,
        dist_pair_greater
    > min_heap;
    while(!C.empty()) {
        min_heap.push(std::move(C.top()));
        C.pop();
    }
    
    std::vector<vertex> result(M);
    for(unsigned int i=0;i<M;i++) {
        result[i] = min_heap.top().first;
        min_heap.pop();
    }

    return result;
}

std::vector<vertex> ann::heuristic_select_neighbor_policy::select_neighbors(
    const hnsw& graph,
    const point& q,
    max_vertex_dist_queue C,
    unsigned int M
) {
    return {};
}