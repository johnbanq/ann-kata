#ifndef ANNDEMO_ALGORITHM_HNSW_INTERNAL_H_
#define ANNDEMO_ALGORITHM_HNSW_INTERNAL_H_

#include "hnsw.hpp"

namespace ann {

    max_vertex_dist_queue hnsw_search_layer(
        const hnsw& hnsw,
        const point& q, 
        std::unordered_set<vertex> ep, int ef, 
        int layer
    );

}

#endif