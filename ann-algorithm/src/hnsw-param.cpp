#include<cmath>
#include "anndemo\algorithm\hnsw.hpp"


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
        std::make_unique<simple_select_neighbor_policy>()
    };
}


std::vector<int> ann::simple_select_neighbor_policy::select_neighbors(
    const hnsw& graph,
    const point& q,
    const std::vector<int>& C,
    int M
) {
    return {};
}