#include <random>
#include "anndemo\algorithm\hnsw-internal.hpp"

using namespace ann;

std::vector<int> ann::hnsw_knn(const hnsw& graph, const point& query, int neighborAmount, int ef) {
    vertex ep = graph.enter_point;
    unsigned int L = graph.pointMaxLayers[graph.enter_point.id];
    for(unsigned int lc = L; lc>= 1; lc--) {
        auto W = hnsw_search_layer(graph, query, {ep}, 1, lc);
        //TODO: optimize for min
        std::pair<vertex, double> nearest{ {0}, std::numeric_limits<double>::max() };
        while (!W.empty()) {
            auto now = W.top();
            W.pop();
            if(now.second<nearest.second) {
                nearest = now;
            }
        }
        ep = nearest.first;
    }

    auto W = hnsw_search_layer(graph, query, {ep}, ef, 0);
    std::vector<int> result(W.size());
    for(unsigned int i=(unsigned int)result.size();i>0;i--) {
        unsigned int idx = i-1;
        result[idx] = W.top().first.id;
        W.pop();
    }
    result.resize(neighborAmount);

    return result;
}


//helper function that turns list of vertex into the heap for nearest_neighbor consumption
static max_vertex_dist_queue convert(hnsw& hnsw, vertex q, const std::list<vertex>& l) {
    max_vertex_dist_queue heap;
    for(const auto& v: l) {
        heap.emplace(v, distance_of(hnsw.points[q.id], hnsw.points[v.id]));
    }
    return heap;
}

static void insert(
    hnsw& hnsw,
    point q,
    std::default_random_engine& engine
) {
    unsigned int l = (unsigned int)floor(-log(std::uniform_real_distribution<double>(0.0, 1.0)(engine))*hnsw.param.mL);
    
    //update related DS
    vertex q_vertex{ (unsigned int)hnsw.points.size() }; //vertex representing q
    hnsw.points.push_back(q);
    hnsw.pointMaxLayers.push_back(l);
    //prepare the required layers
    while(hnsw.layers.size()<=l) {
        undirected_graph g;
        while(g.vertex_size()<hnsw.points.size()) {
            g.add_vertex();
        }
        hnsw.layers.push_back(std::move(g));
    }
    //calibrate all existing layer's to include the new point
    //HACK: design flaw, should use more compact way to represent graph
    for(auto& g: hnsw.layers) {
        while(g.vertex_size()<hnsw.points.size()) {
            g.add_vertex();
        }
    }

    //special case: the first point
    //we have no other point to scan, so just add it
    if(hnsw.points.size() == 1) {
        hnsw.enter_point = {0};
        return;
    }

    unsigned int L = hnsw.pointMaxLayers[hnsw.enter_point.id];
    std::unordered_set<vertex> ep {hnsw.enter_point};

    for(unsigned int i=L;i>=l+1;i--) {
        auto W = hnsw_search_layer(hnsw, q, ep, 1, i);
        //TODO: optimizable for min
        std::pair<vertex, double> nearest{ {0}, std::numeric_limits<double>::max() };
         while (!W.empty()) {
            auto now = W.top();
            W.pop();
            if(now.second<nearest.second) {
                nearest = now;
            }
        }
        ep = std::unordered_set<vertex>{ nearest.first };
    }

    for(unsigned int i=std::min(l, L)+1;i>0;i--) {
        auto lc = i-1; //i is greater than actual val by 1 to prevent wrapping
        auto W = hnsw_search_layer(hnsw, q, ep, hnsw.param.efCtor, lc);
        auto neighbors = hnsw.param.select_neighbor->select_neighbors(hnsw, q, lc, W, hnsw.param.M);
        for(const auto& v : neighbors ) {
            hnsw.layers[lc].add_edge(edge{ q_vertex, v });
        }
        for(const auto& e: neighbors) {
            const auto& eConn = hnsw.layers[lc].adjcent_to(e);
            auto mmax = (lc==0?hnsw.param.Mmax0:hnsw.param.Mmax);
            if(eConn.size() > mmax) {
                auto eNewConn = hnsw.param.select_neighbor->select_neighbors(hnsw, hnsw.points[e.id], lc, convert(hnsw, e, eConn), mmax);
                hnsw.layers[lc].adjlist[e.id] = std::list<vertex>(eNewConn.begin(), eNewConn.end()); 
            }
        }
        //ep = W
        ep.clear();
        while(!W.empty()) {
            ep.insert(W.top().first);
            W.pop();
        }
    }

    if(l>L) {
        hnsw.enter_point = q_vertex;
    }
}

hnsw ann::build_hnsw(std::vector<point> points, hnsw_parameter param) {
    if(points.empty()) {
        throw std::invalid_argument("error.empty_points");
    }

    hnsw graph{
        std::move(param),
        {},
        {}
    };

    std::random_device r;
    std::default_random_engine engine(r());
    for(auto& p: points) {
        insert(graph, std::move(p), engine);
    }

    return graph;
}

max_vertex_dist_queue ann::hnsw_search_layer(
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