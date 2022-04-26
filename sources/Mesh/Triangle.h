//
// Created by d-qql on 28.04.2021.
//

#ifndef FLOWMESHBUILDER_TRIANGLE_H
#define FLOWMESHBUILDER_TRIANGLE_H

#include "Node.h"
#include <array>

class Triangle {
    using idx_t = unsigned int;
public:
    std::array<double, 3> n;  // normal vector to triangle
    std::array<idx_t , 3> nodes;
    Triangle(idx_t id, idx_t node1, idx_t node2, idx_t node3);
    Triangle() = default;
    bool operator<(const Triangle& T2) const;
};

std::ostream& operator<<(std::ostream& os, const Triangle& Tri);

#endif //FLOWMESHBUILDER_TRIANGLE_H
