//
// Created by d-qql on 28.04.2021.
//

#ifndef FLOWMESHBUILDER_NODE_H
#define FLOWMESHBUILDER_NODE_H

#include <iostream>
#include <array>

struct Node {
    double x;
    double y;
    double z;
};

bool operator==(const Node& fst, const Node& snd);
std::ostream& operator<<(std::ostream& os, const Node& n);
std::array<double, 3> operator-(Node start, Node end);

#endif //FLOWMESHBUILDER_NODE_H
