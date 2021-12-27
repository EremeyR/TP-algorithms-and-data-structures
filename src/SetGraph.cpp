#include "SetGraph.h"

#include <cassert>

SetGraph::SetGraph(int vertexCount) : sets(vertexCount) {}

SetGraph::SetGraph(const IGraph &graph) : sets(graph.VerticesCount()){
    for (int i = 0; i < sets.size(); ++i) {
        for (auto& next_vertex : graph.GetNextVertices(i)) {
            sets[i].insert(next_vertex);
        }
    }
}

void SetGraph::AddEdge(int from, int to){
    assert(from >= 0 && from <= sets.size());
    assert(to >= 0 && to <= sets.size());

    sets[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return sets.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex <= sets.size());
    std::vector<int> result;

    for (auto& next_vertex: sets[vertex]) {
        result.push_back(next_vertex);
    }
    return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex <= sets.size());

    std::vector<int> result;
    for (int from = 0; from < sets.size(); ++from) {
        if (sets[from].contains(vertex)) {
            result.push_back(from);
        }
    }

    return result;
}