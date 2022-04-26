import numpy as np
import argparse
import sys
import csv


def create_parser():
    p = argparse.ArgumentParser()
    p.add_argument('-f', type=str, default='./GMSHMeshes/cube_small.msh')
    return p


class CalcNode:
    def __init__(self, a, c: np.ndarray):
        self.index = a
        self.coords = c

    def __str__(self):
        return 'Node_' + str(self.index) + ': ' + str(self.coords)


class Tetrahedron:
    def __init__(self, a, c: np.ndarray):
        self.index = a
        self.nodes = c

    def __str__(self):
        return 'Tetrahedron_' + str(self.index) + ': ' + str(self.nodes)

    def is_neighbour(self, tetra):
        num_of_matches = 0
        for ind in range(4):
            if self.nodes[ind] in tetra.nodes:
                num_of_matches += 1

        if num_of_matches == 3:
            return True
        return False


parser = create_parser()
namespace = parser.parse_args(sys.argv[1:])
m_file = namespace.f
print('FILE:', m_file)

data_nodes = []
data_elements = []

with open(m_file) as mesh_file:
    file = mesh_file.readlines()
    size = len(file)
    nodes_founded = False
    elements_founded = False
    line_counter = 0
    start_nodes = 0
    end_nodes = 0
    start_elements = 0
    end_elements = 0

    while not nodes_founded:
        if file[line_counter] == '$Nodes\n':
            start_nodes = line_counter
        if file[line_counter] == '$EndNodes\n':
            end_nodes = line_counter
        line_counter += 1
        if start_nodes * end_nodes != 0:
            nodes_founded = True
    data_nodes = list(map(lambda x: x.split(' '), list(map(lambda x: x[0:-1:1], file[start_nodes + 1:end_nodes]))))

    print('NODES: start with', start_nodes, 'and end with', end_nodes)

    while not elements_founded:
        if file[line_counter] == '$Elements\n':
            start_elements = line_counter
        if file[line_counter] == '$EndElements\n':
            end_elements = line_counter
        line_counter += 1
        if start_elements * end_elements != 0:
            elements_founded = True
    data_elements = list(
        map(lambda x: x.split(' '), list(map(lambda x: x[0:-1:1], file[start_elements + 1:end_elements]))))

    print('ELEMENTS: start with', start_elements, 'and end with', end_elements)

# NODES PARSING
# getting an amount of nodes and their min and max tags. this information will be used in parsing verification
numNodes, nodeTags = int(data_nodes[0][1]), (int(data_nodes[0][2]), int(data_nodes[0][3]))
print('Amount of nodes:', numNodes)

# getting nodes
nodes = []
counter = 1
while counter < len(data_nodes):
    assert (len(data_nodes[counter]) == 4)
    number_of_nodes_below = int(data_nodes[counter][3])
    counter += 1

    nodes_indexes_below = []
    for i in range(number_of_nodes_below):
        assert (len(data_nodes[counter]) == 1)
        nodes_indexes_below.append(int(data_nodes[counter][0]))
        counter += 1

    for i in range(number_of_nodes_below):
        nodes.append(CalcNode(nodes_indexes_below[i], np.array(data_nodes[counter], dtype=np.float64)))
        counter += 1

# verification
assert len(nodes) == numNodes


# TETRAHEDRONS PARSING
numOfTetrahedrons = 0
# getting tetrahedrons
elements = []
counter = 1
while counter < len(data_elements):
    assert (len(data_elements[counter]) == 4)

    number_of_elements_below = int(data_elements[counter][3])
    type_of_elements_below = int(data_elements[counter][2])
    counter += 1

    if type_of_elements_below != 4:
        counter += number_of_elements_below
    else:
        numOfTetrahedrons += number_of_elements_below
        for i in range(number_of_elements_below):
            assert len(data_elements[counter]) == 6
            elements.append(
                Tetrahedron(int(data_elements[counter][0]), np.array(data_elements[counter][1:-1], dtype=np.int32)))
            counter += 1


# for elem in elements:
#     print(elem)
print('Amount of tetrahedrons:', numOfTetrahedrons)

# verification
assert numOfTetrahedrons == len(elements)
if numOfTetrahedrons == 0:
    print('There is nothing to parse')

# indexes formation
norm_index = elements[0].index
for tetra in elements:
    tetra.index = tetra.index - norm_index


# WRITING TO FILE I CAN EASY PARSE IN C++
nodes_indexes = list(map(lambda x: x.index, nodes))
nodes_coordinates = []
elements_indexes = list(map(lambda x: x.index, elements))
elements_nodes = []
for node in nodes:
    nodes_coordinates.append(node.coords[0])
    nodes_coordinates.append(node.coords[1])
    nodes_coordinates.append(node.coords[2])

for element in elements:
    elements_nodes.append(element.nodes[0])
    elements_nodes.append(element.nodes[1])
    elements_nodes.append(element.nodes[2])
    elements_nodes.append(element.nodes[3])

with open('./PrepearedMeshes/' + m_file.split('/')[-1].split('.')[0] + '.csv', 'w') as csvfile:
    mesh_writer = csv.writer(csvfile, csv.excel)
    # mesh_writer.writerow(nodes_indexes)
    mesh_writer.writerow(nodes_coordinates)
    # mesh_writer.writerow(elements_indexes)
    mesh_writer.writerow(elements_nodes)

    '''
    # FOUNDING NEIGHBORS (confusing)
    neighbours = []
    for i, tetra_base in enumerate(elements):
        # if i % 100 == 0:
        #     print(i)
        neighbours_base = []
        for tetra_tmp in elements:
            if tetra_base.is_neighbour(tetra_tmp):
                neighbours_base.append(tetra_tmp.index)
        neighbours.append(neighbours_base)
    
    for row in neighbours:
        mesh_writer.writerow(row)
    '''

print("WRITE TO:", './PrepearedMeshes/' + m_file.split('/')[-1].split('.')[0] + '.csv')
