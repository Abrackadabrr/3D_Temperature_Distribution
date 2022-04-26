//
// Created by d-qql on 29.04.2021.
//
#include "Node.h"

std::ostream& operator<<(std::ostream& os, const Node& n){
    os << "(" << n.x << ", " << n.y << ", " << n.z << ")";
    return os;
}

std::array<double, 3> operator-(Node start, Node end) {
    return {end.x - start.x, end.y - start.y, end.z - start.z};
}

bool operator==(const Node& fst, const Node& snd){
    if (std::abs(fst.x - snd.x) > 1e-10 ) return false;
    if (std::abs(fst.y - snd.y) > 1e-10 ) return false;
    if (std::abs(fst.z - snd.z) > 1e-10 ) return false;
    return true;
}
