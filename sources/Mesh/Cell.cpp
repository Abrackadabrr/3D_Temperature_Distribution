//
// Created by d-qql on 28.04.2021.
//

#include "Cell.h"

Cell::Cell(int idx, unsigned tri1, unsigned tri2, unsigned tri3, unsigned tri4, const Node &center, double vol) : index(
        idx), volume(vol), center_pos(center) {
    faces.resize(4);
    faces[0] = tri1;
    faces[1] = tri2;
    faces[2] = tri3;
    faces[3] = tri4;
}

bool Cell::is_neighbour(const Cell &other) {
    int matches = 0;
    for (int i = 0; i < 4; i++) {
        int curNode = nodes[i];
        if (std::any_of(other.nodes.begin(), other.nodes.end(), [curNode](int i) { return i == curNode; }))
            matches++;
    }
    if (matches == 3) return true;
    return false;
}
