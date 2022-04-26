//
// Created by evgen on 13.04.2022.
//

#ifndef GRADIENT_SCHEMES_CELLCALCULATION_H
#define GRADIENT_SCHEMES_CELLCALCULATION_H

#include "Cell.h"
#include "Node.h"
#include <array>
namespace CellCalculation{
    double
    MixedProduct(const std::array<double, 3> &f, const std::array<double, 3> &s, const std::array<double, 3> &th);

    double VolumeCalc(const Node &fst, const Node &snd, const Node &th, const Node &fth);

    std::array<double, 3> CrossProduct(const std::array<double, 3> &f, const std::array<double, 3> &s);

    Node Centroid(Node a, Node b, Node c, Node d);
}  // CellCalculation

#endif //GRADIENT_SCHEMES_CELLCALCULATION_H
