//
// Created by evgen on 17.04.2022.
//

#include "gtest/gtest.h"
#include "../sources/Mesh/MeshReader.h"

TEST(MESH, READING) {
    auto res = readMeshDataFromFile("/media/evgen/Big_disc/MIPT/2nd level/Chapter 4/Miniscience/Project/sources/Mesh/PrepearedMeshes/test_mesh_cube.csv");
    ASSERT_TRUE(std::get<1>(res).size() == 24);
}

std::ostream &operator<<(std::ostream &os, std::array<int, 4> a) {
    os <<'(' << a[0] << ' ';
    os << a[1] << ' ';
    os << a[2] << ' ';
    os << a[3] << ')';
    return os;
}