/*
    Created by Alejandro Daniel Noel on 20/05/2017.
*/

#ifndef SIMULATOR_LINEARALGEBRA_H
#define SIMULATOR_LINEARALGEBRA_H

#include <vector>
#include <cmath>

template <typename T>
inline T vector_distance(std::vector<T> point1, std::vector<T> point2) {
    return sqrt(pow(point1.at(0) - point2.at(0), 2) + pow(point1.at(1) - point2.at(1), 2));
}

template <typename T>
int lowestInt(T value) {
    if (value < 0) return int(value) - 1;
    else return int(value);
}


#endif //SIMULATOR_LINEARALGEBRA_H
