#include "anndemo\algorithm\hnsw-internal.hpp"

using namespace ann;

max_vertex_dist_queue ann::hsnw_search_layer(
    const hnsw& hnsw,
    const point& q, 
    std::unordered_set<vertex> ep, int ef, 
    int layer
) {

    //min heap for candidate
    std::priority_queue<
        std::pair<vertex, double>, 
        std::vector<std::pair<vertex, double>>,
        dist_pair_greater
    > candidates;

    //max heap for found
    std::priority_queue<
        std::pair<vertex, double>, 
        std::vector<std::pair<vertex, double>>,
        dist_pair_less
    > found;

    //initialize candidate
    for(const auto& p: ep) {
        auto pq_dist = distance_of(hnsw.points[p.id], q);
        candidates.emplace(p, pq_dist);
        found.emplace(p, pq_dist);
    }

    //move to initialize the visited
    std::unordered_set<vertex> visited(std::move(ep));

    while(!candidates.empty()) {
        auto [c, cq_dist] = candidates.top();
        candidates.pop();
        auto [f, fq_dist] = found.top();
        
        if(fq_dist < cq_dist) {
            break;
        }

        for(const vertex& v: hnsw.layers[layer].adjcent_to(c)) {
            if(visited.count(v)==0) {
                visited.insert(v);
                auto eq_dist = distance_of(hnsw.points[v.id], q);
                if(eq_dist < fq_dist || found.size() < ef) {
                    candidates.emplace(v, eq_dist);
                    found.emplace(v, eq_dist);
                    if(found.size()>ef) {
                        found.pop();
                    }
                }
            }
        }
    }

    return found;
}