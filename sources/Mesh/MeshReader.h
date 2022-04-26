//
// Created by evgen on 14.04.2022.
//

#ifndef MY_PROJECT_MESHREADER_H
#define MY_PROJECT_MESHREADER_H

#include "../Mesh/Mesh.h"
#include "../Mesh/Node.h"
#include "string"
#include "fstream"
#include "sstream"

std::tuple<std::vector<Node>, std::vector<std::array<int, 4>>, std::vector<std::vector<unsigned>>>
readMeshDataFromFileWithNeighbours(const std::string &filename) {
    std::ifstream inputFile;
    inputFile.open(filename);

    // reading nodes
    std::string line_nodes{};
    getline(inputFile, line_nodes);
    std::stringstream inputLine(line_nodes);

    std::vector<double> node_coords{};

    std::string tmp;
    while (getline(inputLine, tmp, ',')) {
        node_coords.push_back(std::stod(tmp));
    }

    unsigned nodes_size = node_coords.size() / 3;
    std::vector<Node> nodes{};
    nodes.reserve(nodes_size);
    for (int i = 0; i < nodes_size; i++)
        nodes.push_back({node_coords[3 * i], node_coords[3 * i + 1], node_coords[3 * i + 2]});

    // reading tetrahedrons
    std::string line_t{};
    getline(inputFile, line_t);
    std::stringstream inputLine_t(line_t);

    std::vector<int> elem_coords{};

    while (getline(inputLine_t, tmp, ',')) {
        elem_coords.push_back(std::stoi(tmp));
    }
    unsigned elem_size = elem_coords.size() / 4;
    std::vector<std::array<int, 4>> tetras{};
    tetras.reserve(elem_size);
    for (int i = 0; i < elem_size; i++)
        tetras.push_back({elem_coords[4 * i] - 1, elem_coords[4 * i + 1] - 1, elem_coords[4 * i + 2] - 1,
                          elem_coords[4 * i + 3] - 1});


    // reading neighbours
    std::vector<std::vector<unsigned>> neighbours(elem_size);
    std::string line;
    int counter = 0;
    while (getline(inputFile, line)) {
        std::stringstream inputline(line);
        while (getline(inputline, tmp, ',')) {
            neighbours[counter].push_back(std::stoi(tmp));
        }
        counter++;
    }

    return {nodes, tetras, neighbours};
}


std::tuple<std::vector<Node>, std::vector<std::array<int, 4>>>
readMeshDataFromFile(const std::string &filename) {
    std::ifstream inputFile;
    inputFile.open(filename);

    // reading nodes
    std::string line_nodes{};
    getline(inputFile, line_nodes);
    std::stringstream inputLine(line_nodes);

    std::vector<double> node_coords{};

    std::string tmp;
    while (getline(inputLine, tmp, ',')) {
        node_coords.push_back(std::stod(tmp));
    }

    unsigned nodes_size = node_coords.size() / 3;
    std::vector<Node> nodes{};
    nodes.reserve(nodes_size);
    for (int i = 0; i < nodes_size; i++)
        nodes.push_back({node_coords[3 * i], node_coords[3 * i + 1], node_coords[3 * i + 2]});

    // reading tetrahedrons
    std::string line_t{};
    getline(inputFile, line_t);
    std::stringstream inputLine_t(line_t);

    std::vector<int> elem_coords{};

    while (getline(inputLine_t, tmp, ',')) {
        elem_coords.push_back(std::stoi(tmp));
    }
    unsigned elem_size = elem_coords.size() / 4;
    std::vector<std::array<int, 4>> tetras{};
    tetras.reserve(elem_size);
    for (int i = 0; i < elem_size; i++)
        tetras.push_back({elem_coords[4 * i] - 1, elem_coords[4 * i + 1] - 1, elem_coords[4 * i + 2] - 1,
                          elem_coords[4 * i + 3] - 1});

    return {nodes, tetras};
}

#endif //MY_PROJECT_MESHREADER_H
