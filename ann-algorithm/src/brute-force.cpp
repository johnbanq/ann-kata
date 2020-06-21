#include<cstdlib>
#include<queue>
#include<stdexcept>
#include "anndemo\algorithm\brute-force.hpp"

using namespace ann;

struct distance_less {

    bool operator()(
        const std::pair<int, double>& a, 
        const std::pair<int, double>& b
    ) const {
        return a.second < b.second;
    }

};

std::vector<int> ann::brute_force_knn(const std::vector<point>& points, const point& query, int neighborAmount) {
    if(neighborAmount <= 0) {
        throw std::invalid_argument("error.neighborAmount_too_small");
    }
    if(neighborAmount > points.size()) {
        throw std::invalid_argument("error.neighborAmount_too_large");
    }
    
    //max heap for index-distance pair
    //we use max and pop extra elements on the go instead of heap every point and pop min approach for space saving
    std::priority_queue<
        std::pair<int, double>, 
        std::vector<std::pair<int, double>>, 
        distance_less
    > maxIndexDistanceQueue;

    for(int i=0;i < (int)points.size();i++) {
        maxIndexDistanceQueue.push(std::make_pair(i, distance_of(query, points[i])));
        if(maxIndexDistanceQueue.size() > neighborAmount) {
            maxIndexDistanceQueue.pop();
        }
    }

    //we are moving from a maxDistance heap, so reversed order
    std::vector<int> result(neighborAmount);
    for(int i = neighborAmount - 1 ; i >= 0 ; i--) {
        result[i] = maxIndexDistanceQueue.top().first;
        maxIndexDistanceQueue.pop();
    }

    return result;
}