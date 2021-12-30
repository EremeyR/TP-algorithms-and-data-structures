/*
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей
между какими-то вершинами. Найдите количество различных кратчайших путей между заданными вершинами.
Требования: сложность O(V+E).
Формат ввода.
v: кол-во вершин (макс. 50000),
n: кол-во ребер (макс. 200000),
n пар реберных вершин,
пара вершин u, w для запроса.
Формат вывода.
Количество кратчайших путей от u к w.
*/

#include <vector>
#include <queue>
#include <cassert>
#include <functional>
#include <iostream>

#define MAX_VERTEX_COUNT  200000 // максимальное количество ребер

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};



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
    lists[to].push_back(from);
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

void BFS (const IGraph& graph, int vertex, const std::function<void(int)> &visit) {
    std::vector<bool> visited (graph.VerticesCount(), false);
    std::queue<int> bfs_queue;
    bfs_queue.push(vertex);
    visited[vertex] = true;

    while (!bfs_queue.empty()) {
        int current = bfs_queue.front();
        bfs_queue.pop();
        visit(current);

        for (auto& node: graph.GetNextVertices(current)) {
            if(!visited[node]) {
                bfs_queue.push(node);
                visited[node] = true;
            }
        }
    }
}



size_t ShortestPathsCounter(const IGraph &graph, int start_vertex, int end_vertex) {
    std::vector<int> path_length(graph.VerticesCount(), MAX_VERTEX_COUNT);
    std::vector<int> shortest_paths_counts(graph.VerticesCount(), 0);

    auto on_visited = [&path_length, &shortest_paths_counts, &graph](int vertex){
        for (int next_vertex : graph.GetNextVertices(vertex)) {
            if (path_length[next_vertex] - 1 > path_length[vertex]) {
                path_length[next_vertex] = path_length[vertex] + 1;
                shortest_paths_counts[next_vertex] = shortest_paths_counts[vertex];
            } else if (path_length[next_vertex] - 1 == path_length[vertex]) {
                shortest_paths_counts[next_vertex] += shortest_paths_counts[vertex];
            }
        }
    };

    path_length[start_vertex] = 0;
    shortest_paths_counts[start_vertex] = 1;
    BFS(graph, start_vertex, on_visited);

    return shortest_paths_counts[end_vertex];
}

int main() {
    int vertex_count = 0;
    std::cin >> vertex_count;
    int edge_count = 0;
    std::cin >> edge_count;

    ListGraph graph(vertex_count);
    int from = 0, to = 0;
    for (int i = 0; i < edge_count; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cin >> from >> to; //  Для определения количества путей

    std::cout << ShortestPathsCounter(graph, from, to);

}