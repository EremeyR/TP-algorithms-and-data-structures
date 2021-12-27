#pragma once
#include "IGraph.h"
#include "set"

class SetGraph : public IGraph {
public:
    explicit SetGraph(int vertexCount);
    explicit SetGraph(const IGraph& graph);

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    std::vector<std::set<int>> sets;
};