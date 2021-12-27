#include "ListGraph.h"
#include <cassert>

ListGraph::ListGraph(int vertexCount) : lists(vertexCount) {}

ListGraph::ListGraph(const IGraph &graph) : lists(graph.VerticesCount()){
    for (int i = 0; i < lists.size(); ++i) {
        lists[i] = graph.GetNextVertices(i);
    }
}

void ListGraph::AddEdge(int from, int to){
    assert(from >= 0 && from <= lists.size());
    assert(to >= 0 && to <= lists.size());

    lists[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return lists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex <= lists.size());
    return lists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex <= lists.size());

    std::vector<int> result;
    for (int from = 0; from < lists.size(); ++from) {
        for (int i = 0; i < lists[from].size(); ++i) {
            if (lists[from][i] == vertex) {
                result.push_back(from);
                break;
            }
        }
    }

    return result;
}