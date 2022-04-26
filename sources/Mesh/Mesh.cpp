//
// Created by evgen on 14.04.2022.
//

#include "Mesh.h"
#include "chrono"
//#include "SLAE/vectors/Vectors.hpp"

#include <utility>

Mesh::Mesh(std::string name, std::vector<Node> nodes, const std::vector<std::array<int, 4>> &tetrahedrons,
           const std::vector<std::vector<unsigned>> &neighbours) : _name(std::move(name)) {
    _nodes = std::move(nodes);
    _cells.reserve(tetrahedrons.size());
    for (int i = 0; i < tetrahedrons.size(); i++) {
        Node fst = _nodes[tetrahedrons[i][0]];
        Node snd = _nodes[tetrahedrons[i][1]];
        Node thd = _nodes[tetrahedrons[i][2]];
        Node fth = _nodes[tetrahedrons[i][3]];
        _cells.emplace_back(i, 0, 0, 0, 0, CellCalculation::Centroid(fst, snd, thd, fth),
                            CellCalculation::VolumeCalc(fst, snd, thd, fth));
        _cells[i].neighbors = neighbours[i];
        _cells[i].temperature = 0;
        _cells[i].temperature_gradient = {0, 0, 0};
        _cells[i].nodes = tetrahedrons[i];
    }
}

Mesh::Mesh(std::string name, std::vector<Node> nodes, const std::vector<std::array<int, 4>> &tetrahedrons) : _name(
        std::move(name)) {
    _nodes = std::move(nodes);
    _cells.reserve(tetrahedrons.size());
    for (int i = 0; i < tetrahedrons.size(); i++) {
        Node fst = _nodes[tetrahedrons[i][0]];
        Node snd = _nodes[tetrahedrons[i][1]];
        Node thd = _nodes[tetrahedrons[i][2]];
        Node fth = _nodes[tetrahedrons[i][3]];
        _cells.emplace_back(i, 0, 0, 0, 0, CellCalculation::Centroid(fst, snd, thd, fth),
                            CellCalculation::VolumeCalc(fst, snd, thd, fth));
        _cells[i].temperature = 0;
        _cells[i].temperature_gradient = {0, 0, 0};
        _cells[i].nodes = tetrahedrons[i];
    }

    // SEARCHING NEIGHBOURS

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < _cells.size(); i++) {
        // prepearing i cell to search neighbours
        auto &current_cell = _cells.at(i);
        current_cell.neighbors.reserve(4);
        // comparison with other cells
        // it's more expensive to do comparison k and i on every step to avoid self comparison
        for (int k = 0; k < _cells.size(); k++) {
            auto temporary_cell = _cells.at(k);
            if (current_cell.is_neighbour(temporary_cell)) {
                // is k neighbour to i
//                if (!std::any_of(current_cell.neighbors.begin(), current_cell.neighbors.end(),
//                                 [k](int i) { return i == k; }))
                    current_cell.neighbors.push_back(k);
/*
                // searching neighbours of i cell in k cell neighbours
                for (const auto &neighbour_neighbour_cell : temporary_cell.neighbors)
                    // neighbour_neighbour_cell is int shows index in global vector of nodes in mesh
                    if (k > _cells[neighbour_neighbour_cell].index)
                        if (current_cell.is_neighbour(_cells[neighbour_neighbour_cell]))
                            if (!std::any_of(current_cell.neighbors.begin(), current_cell.neighbors.end(),
                                             [neighbour_neighbour_cell](int i) {
                                                 return i == neighbour_neighbour_cell;
                                             }))
                                current_cell.neighbors.push_back(neighbour_neighbour_cell);
*/
//                if (!std::any_of(temporary_cell.neighbors.begin(), temporary_cell.neighbors.end(),
//                                 [i](int k) { return i == k; }))
//                    temporary_cell.neighbors.push_back(i);
            }
            if (current_cell.neighbors.size() == 4) {
//                std::cout << current_cell.neighbors << '\n';
                break;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "\t Searcing neidours takes " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count()<< "  miliseconds" << std::endl;
}

void Mesh::setTemperatureFieldByFunction(const std::function<double(double, double, double)> &f) {
    for (auto &cell: _cells)
        cell.temperature = f(cell.center_pos.x, cell.center_pos.y, cell.center_pos.z);
}

#if WITH_VTK

void Mesh::test_snapshot(unsigned int snap_number) {
    // VTK grid
    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    // VTK points
    vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();

    auto smth = vtkSmartPointer<vtkDoubleArray>::New();
    smth->SetName("smth");
    auto grad = vtkSmartPointer<vtkDoubleArray>::New();
    grad->SetNumberOfComponents(3);
    grad->SetName("gradient");

    for (auto &_cell : _cells) {
        dumpPoints->InsertNextPoint(_cell.center_pos.x, _cell.center_pos.y, _cell.center_pos.z);
        smth->InsertNextValue(_cell.temperature);
        double a[3] = {_cell.temperature_gradient[0], _cell.temperature_gradient[1], _cell.temperature_gradient[2]};
        grad->InsertNextTuple(a);
    }

//     Обходим все точки нашей расчётной сетки
    for (auto &_node : _nodes) {
        // Вставляем новую точку в сетку VTK-снапшота
        dumpPoints->InsertNextPoint(_node.x, _node.y, _node.z);
//        smth->InsertNextValue(0);
//        double a[3] = {0, 0, 0};
//        grad->InsertNextTuple(a);
    }


    // Грузим точки в сетку
    unstructuredGrid->SetPoints(dumpPoints);

    // А теперь пишем, как наши точки объединены в тетраэдры
    for (auto &_cell : _cells) {
        auto tetra = vtkSmartPointer<vtkTetra>::New();
        tetra->GetPointIds()->SetId(0, _cell.nodes[0] + _cells.size());
        tetra->GetPointIds()->SetId(1, _cell.nodes[1] + _cells.size());
        tetra->GetPointIds()->SetId(2, _cell.nodes[2]+ _cells.size());
        tetra->GetPointIds()->SetId(3, _cell.nodes[3]+ _cells.size());
//        tetra->GetPoints()->SetPoint(0, _nodes[_cell.nodes[0]].x, _nodes[_cell.nodes[0]].y, _nodes[_cell.nodes[0]].z);
//        tetra->GetPoints()->SetPoint(1, _nodes[_cell.nodes[1]].x, _nodes[_cell.nodes[1]].y, _nodes[_cell.nodes[1]].z);
//        tetra->GetPoints()->SetPoint(2, _nodes[_cell.nodes[2]].x, _nodes[_cell.nodes[2]].y, _nodes[_cell.nodes[2]].z);
//        tetra->GetPoints()->SetPoint(3, _nodes[_cell.nodes[3]].x, _nodes[_cell.nodes[3]].y, _nodes[_cell.nodes[3]].z);
        unstructuredGrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
    }

    unstructuredGrid->GetPointData()->AddArray(smth);
//    unstructuredGrid->GetCellData()->
    unstructuredGrid->GetPointData()->AddArray(grad);

    // Создаём снапшот в файле с заданным именем
    std::string fileName = _name + "-step-" + std::to_string(snap_number) + ".vtu";
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(unstructuredGrid);
    writer->Write();
}

#endif