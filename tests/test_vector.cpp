//
// Created by evgen on 13.04.2022.
//

#include "gtest/gtest.h"
#include "../sources/Mesh/Node.h"
#include "../sources/Mesh/CellCalculation.h"

class VectorTESTS: public ::testing::Test {
protected:
    Node a, b, c, d;
    double mixprod;
    double tolerance;
    void SetUp() override {
        tolerance = 1e-10;
        mixprod = 5./6;
        a = {1, 2,3};
        b = {6, 9,11};
        c = {6, 7,8};
        d = {4, 8,10};
    }
};

TEST_F(VectorTESTS, MIXED_PRODUCTION){
    ASSERT_NEAR(CellCalculation::VolumeCalc(a, b, c, d), mixprod,tolerance);
}

TEST_F(VectorTESTS, CROSS_PRODUCTION){
    auto res = CellCalculation::CrossProduct(b-a, c-a);
    std::array<double, 3> real_res{-5, 15, -10};
    double sum = 0;
    for (int i = 0; i < 3; i++) sum += (res[i]-real_res[i]);
    ASSERT_NEAR(sum, 0, tolerance);
}