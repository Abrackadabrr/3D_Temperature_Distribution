//
// Created by evgen on 14.04.2022.
//

#ifndef MY_PROJECT_MESH_H
#define MY_PROJECT_MESH_H
#define WITH_VTK true

#include <cmath>
#include <vector>

#if WITH_VTK
#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkTetra.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#endif

#include "../Mesh/Cell.h"
#include "../Mesh/CellCalculation.h"
#include "../Mesh/Node.h"
#include "../Mesh/Triangle.h"

class Mesh {
public:
    /**
     * @param nodes - all nodes in mesh
     * @param cells - all cells in mesh
     * @param faces - all faces in mesh
     */
    std::vector<Node> _nodes;
    std::vector<Cell> _cells;
    std::string _name;

public:
    Mesh(std::string  name, std::vector<Node> nodes, const std::vector<std::array<int, 4>>& tetrahedrons, const std::vector<std::vector<unsigned>>& neighbours);
    Mesh(std::string  name, std::vector<Node> nodes, const std::vector<std::array<int, 4>>& tetrahedrons);
    void setTemperatureFieldByFunction(const std::function<double (double, double, double)>& f);
#if WITH_VTK
    void test_snapshot(unsigned int snap_number);
#endif
};

#endif //MY_PROJECT_MESH_H
