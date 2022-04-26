//
// Created by d-qql on 28.04.2021.
//

#ifndef FLOWMESHBUILDER_CELL_H
#define FLOWMESHBUILDER_CELL_H

#include <vector>
#include "Triangle.h"
#include <algorithm>
#include <unordered_map>
class Cell {
public:
    /**
     * @param faces - faces that cell contains
     * @param neighbors - array of neighbour cells to current
     * if i want to obtain cell, connected with current by faces[i], i should take neighbor[i]
     */
     int index;
    std::vector<unsigned> faces;
    std::vector<unsigned> neighbors;
    std::array<int, 4> nodes;
    Node center_pos;
    double volume;
    double temperature;
    std::vector<double> temperature_gradient;
    Cell(int idx, unsigned tri1, unsigned tri2, unsigned tri3, unsigned tri4, const Node& center, double vol);
    bool is_neighbour(const Cell& other);
};

#endif //FLOWMESHBUILDER_CELL_H
