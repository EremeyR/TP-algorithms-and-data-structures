#include "MatrixGraph.h"
#include <cassert>

MatrixGraph::MatrixGraph(int vertexCount) :  matrix(vertexCount) {
    for (auto& line : matrix) {
        line.resize(vertexCount);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph) : matrix(graph.VerticesCount()) {
    for (auto& line : matrix) {
        line.resize(graph.VerticesCount());
    }

    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (auto& to : graph.GetNextVertices(from)) {
            matrix[from][to] = Cell(true, to);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    matrix[from][to] = Cell(true, to);
}

int MatrixGraph::VerticesCount() const {
    return matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (auto& cell : matrix[vertex]) {
        if (cell.is_connected && cell.directed_to != vertex) {
            result.push_back(cell.directed_to);
        }
    }
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < matrix.size(); ++i) {
        if (matrix[i][vertex].is_connected && matrix[i][vertex].directed_to == vertex) {
            result.push_back(i);
        }
    }
    return result;
}
