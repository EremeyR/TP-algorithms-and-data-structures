#pragma once

#include "IGraph.h"

class ArcGraph : public IGraph {
public:
    explicit ArcGraph(int vertexCount);
    explicit ArcGraph(const IGraph& graph);

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    struct Cell {
        int from;
        int to;
        Cell(int from, int to) : from(from), to (to) {}
    };
    std::vector<Cell> pairs;
    int vertices_count;
};
