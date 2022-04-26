//
// Created by evgen on 18.04.2022.
//

#ifndef MY_PROJECT_LEASTSQUAREGRADIENT_H
#define MY_PROJECT_LEASTSQUAREGRADIENT_H


#include "../Mesh/Cell.h"
#include "../Mesh/Mesh.h"
#include "SLAE/matrix/dense/Densematrix.hpp"

enum LeastSquareType {
    Ordinary,
    Weighted,
};

template<LeastSquareType Type>
class LeastSquareGradient {
private:
    /**
     * @param mesh - reference to mesh where we calculate gradient of scalar fieal
     * @param gradient_matrix - vector of gradient matrix to 'mesh'
     */
    const Mesh& mesh;
    std::vector<Slae::Matrix::DenseMatrix<double>> gradient_matrix;
public:
    LeastSquareGradient(const Mesh & _mesh);
    std::vector<double> operator()(int cell_index) const;
    Slae::Matrix::DenseMatrix<double> getGradientMatrix(const Cell& cell);
    Slae::Matrix::DenseMatrix<double> getWeightedGradientMatrix(const Cell& cell);
    Slae::Matrix::DenseMatrix<double> inverse(const Slae::Matrix::DenseMatrix<double>& a);
};

template<LeastSquareType Type>
LeastSquareGradient<Type>::LeastSquareGradient(const Mesh &_mesh) : mesh(_mesh) {
    gradient_matrix.reserve(mesh._cells.size());
    if (Type == LeastSquareType::Ordinary)
        for (auto &cell: mesh._cells) {
            gradient_matrix.push_back(getGradientMatrix(cell));
        }
    else {
        for (auto &cell: mesh._cells) {
            gradient_matrix.push_back(getWeightedGradientMatrix(cell));
        }
    }
}

template<LeastSquareType Type>
std::vector<double> LeastSquareGradient<Type>::operator()(int cell_index) const{
    // temperature difference vector calculation
    std::vector<double> temperature_difference{};
    temperature_difference.reserve(4);
    double current_temperature = mesh._cells[cell_index].temperature;
    for (auto &n: mesh._cells[cell_index].neighbors) {
        temperature_difference.push_back(mesh._cells[n].temperature - current_temperature);
    }
    if (temperature_difference.size() == 4) {
        Slae::Matrix::DenseMatrix<double> A = gradient_matrix[cell_index];
        return A * temperature_difference;
    }
    return {0, 0, 0};
}

template<LeastSquareType Type>
Slae::Matrix::DenseMatrix<double> LeastSquareGradient<Type>::getGradientMatrix(const Cell &cell) {
    std::vector<double> data{};
    if (cell.neighbors.size() == 4) {
        data.reserve(12);
        Node p = cell.center_pos;
        for (auto &neibour: cell.neighbors) {
            std::array<double, 3> distance_vector = mesh._cells[neibour].center_pos - p;
            data.push_back(distance_vector[0]);
            data.push_back(distance_vector[1]);
            data.push_back(distance_vector[2]);
        }
        auto distance_matrix = Slae::Matrix::DenseMatrix<double>(4, 3, data);

        return inverse(distance_matrix.transpose() * distance_matrix) * distance_matrix.transpose();
    } else {
        return Slae::Matrix::DenseMatrix<double>(3, 3, 0);
    }
}

template<LeastSquareType Type>
Slae::Matrix::DenseMatrix<double> LeastSquareGradient<Type>::inverse(const Slae::Matrix::DenseMatrix<double> &a) {
    Slae::Matrix::DenseMatrix<double> inverse(3, 3, 0);
    if (a.get_row_size() == 3 && a.get_col_size() == 3) {
        double det = a(0, 0) * a(1, 1) * a(2, 2) + a(1, 0) * a(2, 1) * a(0, 2) + a(0, 1) * a(1, 2) * a(2, 0) -
                     a(0, 2) * a(1, 1) * a(2, 0) - a(0, 0) * a(1, 2) * a(2, 1) - a(2, 2) * a(0, 1) * a(1, 0);
        if (std::abs(det - 0) > 1e-20) {
            inverse(0, 0) = (1 / det) * (a(1, 1) * a(2, 2) - a(2, 1) * a(1, 2));
            inverse(1, 1) = (1 / det) * (a(0, 0) * a(2, 2) - a(2, 0) * a(0, 2));
            inverse(2, 2) = (1 / det) * (a(0, 0) * a(1, 1) - a(1, 0) * a(0, 1));
            inverse(0, 1) = (-1 / det) * (a(1, 0) * a(2, 2) - a(2, 0) * a(1, 2));
            inverse(0, 2) = (1 / det) * (a(1, 0) * a(2, 1) - a(2, 0) * a(1, 1));
            inverse(1, 2) = (-1 / det) * (a(0, 0) * a(2, 1) - a(2, 0) * a(0, 1));
            inverse(1, 0) = inverse(0, 1);
            inverse(2, 0) = inverse(0, 2);
            inverse(2, 1) = inverse(1, 2);
        }
    }
    return inverse;
}

template<LeastSquareType Type>
Slae::Matrix::DenseMatrix<double> LeastSquareGradient<Type>::getWeightedGradientMatrix(const Cell &cell) {
    std::vector<double> data{};
    std::vector<double> weights;
    if (cell.neighbors.size() == 4) {
        data.reserve(12);
        weights.resize(16);
        Node p = cell.center_pos;
        int counter = 0;
        for (auto &neibour: cell.neighbors) {
            std::array<double, 3> distance_vector = mesh._cells[neibour].center_pos - p;
            double x = distance_vector[0];
            double y = distance_vector[1];
            double z = distance_vector[2];
            data.push_back(x);
            data.push_back(y);
            data.push_back(z);
            weights[4 * counter + counter] = 1 / std::sqrt(x * x + y * y + z * z);
            counter++;
        }
        auto distance_matrix = Slae::Matrix::DenseMatrix<double>(4, 3, data);
        auto weights_matrix = Slae::Matrix::DenseMatrix<double>(4, 4, weights);
        auto newD = weights_matrix * distance_matrix;

//        std::cout << newD << '\n';
//        std::cout << newD.transpose() * newD << '\n';
//        std::cout << inverse(newD.transpose() * newD) << '\n';
//        std::cout << inverse(newD.transpose() * newD) *
//                     newD.transpose() * weights_matrix << '\n';
//        std::cout << inverse(newD.transpose() * newD) * newD.transpose() * newD << '\n';

        return inverse(newD.transpose() * newD) *
               newD.transpose() * weights_matrix;
    } else {
        return Slae::Matrix::DenseMatrix<double>(3, 3, 0);
    }
}

#endif //MY_PROJECT_LEASTSQUAREGRADIENT_H
