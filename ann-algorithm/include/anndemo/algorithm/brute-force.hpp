#ifndef ANNDEMO_ALGORITHM_BRUTEFORCE_H_
#define ANNDEMO_ALGORITHM_BRUTEFORCE_H_

#include<vector>

#include "point.hpp"

namespace ann {

    std::vector<int> brute_force_knn(const std::vector<point>& points, const point& query, int neighborAmount);

}

#endif