#include <cmath>
#include <limits>
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
    bool keepPrunedConnections    
) {
    return ann::heuristic_policy(keepPrunedConnections, false);
}

std::unique_ptr<select_neighbor_policy> ann::heuristic_policy(
    bool keepPrunedConnections,
    bool extendCandidates       
) {
    return std::make_unique<heuristic_select_neighbor_policy>(extendCandidates, keepPrunedConnections);
}



std::vector<vertex> ann::simple_select_neighbor_policy::select_neighbors(
    const hnsw& graph,
    const point& q,
    unsigned int layer,
    max_vertex_dist_queue C,
    unsigned int M
) {
    if(layer>graph.layers.size()) {
        throw std::invalid_argument("error.invalid_layer");
    }
    if(M>C.size()) {
        throw std::invalid_argument("error.invalid_M");
    }

    //we had to convert to the min heap for extraction
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
    unsigned int layer,
    max_vertex_dist_queue C,
    unsigned int M
) {
    if(layer>graph.layers.size()) {
        throw std::invalid_argument("error.invalid_layer");
    }
    if(M>C.size()) {
        throw std::invalid_argument("error.invalid_M");
    }

    //we had to convert to the min heap for later extraction
    //TODO: this is optimizable by directly make_heap on underlying vector
    std::priority_queue<
        std::pair<vertex, double>, 
        std::vector<std::pair<vertex, double>>,
        dist_pair_greater
    > W;
    std::unordered_set<vertex> visitedW;
    //merged step 2 & 3-7 in Alg.4
    while(!C.empty()) {
        auto [v, dist] = C.top();
        C.pop();
        if(visitedW.count(v)==0) {
            visitedW.insert(v);
            W.emplace(v, dist);
        }
        if(this->extendCandidates) {
            for(const auto& e: graph.layers[layer].adjcent_to(v)) {
                if(visitedW.count(e)==0) {
                    visitedW.insert(e);
                    W.emplace(e, dist);
                }       
            }
        }
    }

    //min distance heap for line 17 in Alg.4
    std::priority_queue<
        std::pair<vertex, double>, 
        std::vector<std::pair<vertex, double>>,
        dist_pair_greater
    > Wd;

    std::vector<vertex> result;
    double result_min_dist = std::numeric_limits<double>::max();

    while(!W.empty() && (result.size())<M) {
        auto [e, eq_dist] = W.top();
        W.pop();
        if(eq_dist < result_min_dist) {
            result_min_dist = eq_dist;
            result.push_back(e);
        } else {
            Wd.emplace(e, eq_dist);
        }
    }

    if(this-> keepPrunedConnections) {
        while(!Wd.empty() && (result.size())<M) {
            auto [e, eq_dist] = Wd.top();
            Wd.pop();
            result.push_back(e);
        }        
    }

    return result;
}