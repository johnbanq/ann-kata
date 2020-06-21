/**
 * brute force algorithm for the ANN problem
 */
#ifndef ANNDEMO_ALGORITHM_BRUTEFORCE_H_
#define ANNDEMO_ALGORITHM_BRUTEFORCE_H_

#include<vector>

#include "point.hpp"

namespace ann {

    /**
     * brute force algorithm for ANN problem
     * 
     * @param points the points to be searched
     * @param query the point whose K-nearest neighbor to be found
     * @param neighborAmount the K value, amount of neighbor to find
     * 
     * @returns the indexes of the neighbors, ordered from nearest to farest
     * 
     * @throws std::invalid_argument on argument error
     */
    std::vector<int> brute_force_knn(const std::vector<point>& points, const point& query, int neighborAmount);

}

#endif