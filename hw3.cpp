/*
Требуется отыскать самый выгодный маршрут между городами.
Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута.
*/

#include <vector>
#include <set>
#include <cassert>
#include <functional>
#include <iostream>

#define MAX_PATH_TIME 320000 // максимальное время в пути

struct RoadGraph {
    struct Road {
        int time;
        int to;
        Road(int time, int to) : time(time), to(to) {}
    };

    virtual ~RoadGraph() {}
    virtual void AddRoad(int from, int to, int time) = 0;

    virtual int CitesCount() const  = 0;

    virtual std::vector<Road> GetNextRoads(int vertex) const = 0;


};



class ListRoadGraph : public RoadGraph {
public:
    explicit ListRoadGraph(int roads_count) : roads(roads_count) {};

    void AddRoad(int from, int to, int time) override;
    int CitesCount() const override;
    std::vector<Road> GetNextRoads(int vertex) const override{
        return roads[vertex];
    }

private:

    std::vector<std::vector<Road>> roads;
};

void ListRoadGraph::AddRoad(int from, int to, int time) {
    assert(from >= 0 && from <= roads.size());
    assert(to >= 0 && to <= roads.size());

    roads[from].push_back(Road(time, to));
    roads[to].push_back(Road(time, from));
}

int ListRoadGraph::CitesCount() const{
    return roads.size();
}
//
//
int GetShortestPath(const RoadGraph &graph, int start_city, int end_city) {
    std::vector<int> path_length(graph.CitesCount(), MAX_PATH_TIME);

    struct Road {
        int time;
        int to;
        Road(int time, int to) : time(time), to(to) {}
        Road(std::pair<int, int> road) : time(road.first), to(road.second) {}
    };
    std::set<std::pair<int, int>> roads_queue;
    roads_queue.emplace(0, start_city);

    path_length[start_city] = 0;
    while (!roads_queue.empty()) {
        Road current_road = *(roads_queue.begin());
        roads_queue.erase(std::pair<int, int>(current_road.time, current_road.to));
        for (auto &next_road : graph.GetNextRoads((current_road.to))) {
            if (path_length[next_road.to] - next_road.time > path_length[current_road.to]) {
                roads_queue.erase({path_length[next_road.to], next_road.to});
                path_length[next_road.to] = path_length[current_road.to] + next_road.time;
                roads_queue.emplace(path_length[next_road.to], next_road.to);
            }
        }
    }

    return path_length[end_city];
}

int main() {
    int vertex_count = 0;
    std::cin >> vertex_count;
    int edge_count = 0;
    std::cin >> edge_count;

    ListRoadGraph graph(vertex_count);
    int from = 0, to = 0, time;
    for (int i = 0; i < edge_count; ++i) {
        std::cin >> from >> to >> time;
        graph.AddRoad(from, to, time);
    }

    std::cin >> from >> to; //  Для определения времени в пути

    std::cout << GetShortestPath(graph, from, to);
}
