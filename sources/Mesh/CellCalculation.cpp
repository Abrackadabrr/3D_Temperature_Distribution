//
// Created by evgen on 13.04.2022.
//
#include "CellCalculation.h"

namespace CellCalculation {
    double
    MixedProduct(const std::array<double, 3> &f, const std::array<double, 3> &s, const std::array<double, 3> &th) {
        return f[0] * s[1] * th[2] + f[3] * s[0] * th[1] + f[1] * s[2] * th[0] - f[2] * s[1] * th[0] -
               f[1] * s[0] * th[2] - f[0] * s[2] * th[1];
    };

    double VolumeCalc(const Node &fst, const Node &snd, const Node &th, const Node &fth) {
        return std::abs(MixedProduct(fst - snd, snd - th, th - fth)) / 6;
    }

    std::array<double, 3> CrossProduct(const std::array<double, 3> &f, const std::array<double, 3> &s) {
        return {f[1] * s[2] - f[2] * s[1], f[2] * s[0] - f[0] * s[2], f[0] * s[1] - f[1] * s[0]};
    }

    Node Centroid(Node a, Node b, Node c, Node d) {
        return {(a.x + b.x + c.x + d.x) / 4, (a.y + b.y + c.y + d.y) / 4, (a.z + b.z + c.z + d.z) / 4};
    }
}  // CellCalculation