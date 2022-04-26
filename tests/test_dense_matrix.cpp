//
// Created by evgen on 18.04.2022.
//

#include "gtest/gtest.h"
#include "iostream"
#include "../sources/Mesh/Cell.h"
#include "../sources/Mesh/Mesh.h"
#include "../sources/Mesh/MeshReader.h"
#include "../sources/GradientSchemes/LeastSquareGradient.h"
#include "SLAE/vectors/Vectors.hpp"
#include "SLAE/vectors/Norm.hpp"

double linear(double x, double y, double z) {
    return x + y + z;
}

double linear_gradient_error(const Cell &cell) {
    return norm(std::vector<double>({1, 1, 1}) -
                cell.temperature_gradient, NormType::SecondNorm);
}

double paraboloid(double x, double y, double z) {
    return x * x + y * y + z * z;
}

double paraboloid_gradient_error(const Cell &cell) {
    double x = cell.center_pos.x;
    double y = cell.center_pos.y;
    double z = cell.center_pos.z;
    return norm(std::vector<double>({2 * x, 2 * y, 2 * z}) -
                cell.temperature_gradient, NormType::SecondNorm);
}

double sinus(double x, double y, double z) {
    return std::sin(x + y + z);
}

double sinus_gradient_error(const Cell &cell) {
    double x = cell.center_pos.x;
    double y = cell.center_pos.y;
    double z = cell.center_pos.z;
    return norm(std::vector<double>({std::cos(x + y + z), std::cos(x + y + z), std::cos(x + y + z)}) -
                cell.temperature_gradient, NormType::SecondNorm);
}

void is_working(const std::string &name, const std::function<double(double, double, double)> &function,
                const std::function<double (const Cell&)>& error_function, Mesh &mesh,
                const LeastSquareGradient<LeastSquareType::Ordinary> &ordinary_gradient,
                const LeastSquareGradient<LeastSquareType::Weighted> &weighted_gradient,
                double gradient_norm_tolerance) {

    double max_error = 0;
    std::cout << "//---------------------" + name + "---------------------//" << '\n';

    mesh.setTemperatureFieldByFunction(function);

    for (int i = 0; i < mesh._cells.size(); i++) {
        mesh._cells[i].temperature_gradient = (-1) * ordinary_gradient(i);
        if (norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) > gradient_norm_tolerance) {
            double value = error_function(mesh._cells[i]) /
                           norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm);
            if (value > max_error)
                max_error = value;
        }
    }
    std::cout << "Ordinary gradient scheme gets " << max_error << '\n';

    max_error = 0;
    for (int i = 0; i < mesh._cells.size(); i++) {
        mesh._cells[i].temperature_gradient = (-1) * weighted_gradient(i);
        if (norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) > gradient_norm_tolerance) {
            double value = error_function(mesh._cells[i]) /
                           norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm);
            if (value > max_error)
                max_error = value;
        }
    }
    std::cout << "Weighted gradient scheme gets " << max_error << '\n';
}

TEST(GradientSchames, LeastSqueare) {
    auto res = readMeshDataFromFile(
            "/media/evgen/Big_disc/MIPT/2nd level/Chapter 4/3D_Temperature_Distribution/sources/Mesh/PrepearedMeshes/cube.csv");
    Mesh mesh("cube4", std::get<0>(res), std::get<1>(res));
    LeastSquareGradient<LeastSquareType::Weighted> weighted_gradient(mesh);
    LeastSquareGradient<LeastSquareType::Ordinary> ordinary_gradient(mesh);

    double gradient_norm_tolerance = 1e-3;

    is_working("Linear", linear, linear_gradient_error, mesh, ordinary_gradient, weighted_gradient, gradient_norm_tolerance);

#if 0
    std::cout << "//--------------Paraboloid distribution---------------//" << '\n';
    mesh.setTemperatureFieldByFunction(paraboloid);

    max_error = 0;
    for (int i = 0; i < mesh._cells.size(); i++) {
        mesh._cells[i].temperature_gradient = (-1) * ordinary_gradient(i);
        if (norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) > gradient_norm_tolerance) {
            double value = paraboloid_gradient_error(mesh._cells[i]) /
                           norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm);
            if (value > max_error)
                max_error = value;
        }
    }
    std::cout << "Ordinary gradient scheme gets " << max_error << '\n';

    max_error = 0;
    for (int i = 0; i < mesh._cells.size(); i++) {
        mesh._cells[i].temperature_gradient = (-1) * weighted_gradient(i);
        if (norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) > gradient_norm_tolerance) {
            double value = paraboloid_gradient_error(mesh._cells[i]) /
                           norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm);
            if (value > max_error)
                max_error = value;
        }
    }
    std::cout << "Weighted gradient scheme gets " << max_error << '\n';

    std::cout << "//--------------Sin(x + y + z) distribution---------------//" << '\n';
    mesh.setTemperatureFieldByFunction(sinus);

    max_error = 0;
    for (int i = 0; i < mesh._cells.size(); i++) {
        mesh._cells[i].temperature_gradient = (-1) * ordinary_gradient(i);
        if (norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) > gradient_norm_tolerance)
            if (sinus_gradient_error(mesh._cells[i]) / norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) >
                max_error)
                max_error = sinus_gradient_error(mesh._cells[i]) /
                            norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm);
    }
    std::cout << "Ordinary gradient scheme gets " << max_error << '\n';

    max_error = 0;
    for (int i = 0; i < mesh._cells.size(); i++) {
        mesh._cells[i].temperature_gradient = (-1) * weighted_gradient(i);
        if (norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) > gradient_norm_tolerance)
            if (sinus_gradient_error(mesh._cells[i]) / norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm) >
                max_error)
                max_error = sinus_gradient_error(mesh._cells[i]) /
                            norm(mesh._cells[i].temperature_gradient, NormType::SecondNorm);
    }
    std::cout << "Weighted gradient scheme gets " << max_error << '\n';
#endif
}