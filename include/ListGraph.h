#pragma once
#include "IGraph.h"



class ListGraph : public IGraph {
public:
    explicit ListGraph(int vertexCount);
    explicit ListGraph(const IGraph& graph);

    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    std::vector<std::vector<int>> lists;
};

