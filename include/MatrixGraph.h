#pragma once
#include "IGraph.h"


class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(int vertexCount);
    explicit MatrixGraph(const IGraph& graph);

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    struct Cell {
        bool is_connected;
        int directed_to;
        Cell(bool is_connected = false, int directed_to = -1)
                : is_connected(is_connected), directed_to(directed_to) {
        }
    };
    std::vector<std::vector<Cell>> matrix;
};
