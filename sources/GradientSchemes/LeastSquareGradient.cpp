//
// Created by evgen on 18.04.2022.
//

LeastSquareGradient::LeastSquareGradient(const Mesh &_mesh) : mesh(_mesh) {
    gradient_matrix.reserve(mesh._cells.size());
    if (Type == LeastSquareType::Weighted)
        for (auto &cell: mesh._cells) {
            gradient_matrix.push_back(getGradientMatrix(cell));
        }
    else {
        for (auto &cell: mesh._cells) {
            gradient_matrix.push_back(getWeightedGradientMatrix(cell));
        }
        }
}

std::vector<double> LeastSquareGradient::operator()(int cell_index) {
    // temperature difference vector calculation
    std::vector<double> temperature_difference{}; temperature_difference.reserve(4);
    double current_temperature = mesh._cells[cell_index].temperature;
    for (auto & n: mesh._cells[cell_index].neighbors) {
        temperature_difference.push_back(mesh._cells[n].temperature - current_temperature);
    }
    if (temperature_difference.size() == 4)
        return gradient_matrix[cell_index] * temperature_difference;
    return {0, 0, 0};
}


Slae::Matrix::DenseMatrix<double> LeastSquareGradient::getGradientMatrix(const Cell &cell) {
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

Slae::Matrix::DenseMatrix<double> LeastSquareGradient::inverse(const Slae::Matrix::DenseMatrix<double> &a) {
    Slae::Matrix::DenseMatrix<double> inverse(3, 3, 0);
    if (a.get_row_size() == 3 && a.get_col_size() == 3) {
        double det = a(0, 0) * a(1, 1) * a(2, 2) + a(1, 0) * a(2, 1) * a(0, 2) + a(0, 1) * a(1, 2) * a(2, 0) -
                     a(0, 2) * a(1, 1) * a(2, 0) - a(0, 0) * a(1, 2) * a(2, 1) - a(2, 2) * a(0, 1) * a(1, 0);
        if (std::abs(det - 0) > 1e-10) {
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

