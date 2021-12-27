#include "ArcGraph.h"
#include <cassert>

ArcGraph::ArcGraph(int vertexCount) : vertices_count(vertexCount) {}

ArcGraph::ArcGraph(const IGraph &graph) : vertices_count(graph.VerticesCount()){
    for (int from = 0; from < vertices_count; ++from) {
        for (auto& to : graph.GetNextVertices(from)) {
            pairs.emplace_back(Cell(from,to));
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from <= vertices_count);
    assert(to >= 0 && to <= vertices_count);
    pairs.emplace_back(Cell(from,to));
}

int ArcGraph::VerticesCount() const {
    return vertices_count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex <= vertices_count);
    std::vector<int> result;

    for (auto& cell: pairs) {
        if (cell.from == vertex) {
            result.push_back(cell.to);
        }
    }
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex <= vertices_count);
    std::vector<int> result;

    for (auto& cell: pairs) {
        if (cell.to == vertex) {
            result.push_back(cell.from);
        }
    }
    return result;
}