/*
Необходимо написать несколько реализаций интерфейса:
ListGraph, хранящий граф в виде массива списков смежности,
MatrixGraph, хранящий граф в виде матрицы смежности,
SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
ArcGraph, хранящий граф в виде одного массива пар {from, to}.
Также необходимо реализовать конструктор, принимающий const IGraph&. Такой конструктор должен скопировать
переданный граф в создаваемый объект.
Для каждого класса создавайте отдельные h и cpp файлы.
Число вершин графа задается в конструкторе каждой реализации.
*/

#include<iostream>
#include<vector>
#include<string>
#include <sstream>
#include<cassert>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

#include <cassert>

void Tests();

int run(std::istream& input, std::ostream& output) {

    return 0;
}

int main() {
    Tests();
    //run(std::cin, std::cout);
    return 0;
}

void Tests() {
    std::vector<int> single_test1_answers = {1, 2, 3};
    std::vector<int> single_test2_answers = {};
    std::vector<int> single_test3_answers = {0, 1, 2};
    std::vector<int> single_test4_answers = {};
    std::vector<int> single_test5_answers = {4};
    std::vector<int> single_test6_answers = {2,3};
    int size_answer = 5;

    // single tests
    {
        ListGraph list_graph(5);
        list_graph.AddEdge(0, 1);
        list_graph.AddEdge(0, 2);
        list_graph.AddEdge(0, 3);
        list_graph.AddEdge(1, 3);
        list_graph.AddEdge(2, 3);
        list_graph.AddEdge(2, 4);

        assert(list_graph.GetNextVertices(0) == single_test1_answers);
        assert(list_graph.GetNextVertices(3) == single_test2_answers);
        assert(list_graph.GetPrevVertices(3) == single_test3_answers);
        assert(list_graph.GetPrevVertices(0) == single_test4_answers);
        assert(list_graph.VerticesCount() == size_answer);

        list_graph.AddEdge(3, 4);

        assert(list_graph.GetNextVertices(3) == single_test5_answers);
        assert(list_graph.GetPrevVertices(4) == single_test6_answers);
        assert(list_graph.VerticesCount() == size_answer);

        std::cout << "Single ListGraph test  passed\n";
    }

    {
        SetGraph set_graph(5);
        set_graph.AddEdge(0, 1);
        set_graph.AddEdge(0, 2);
        set_graph.AddEdge(0, 3);
        set_graph.AddEdge(1, 3);
        set_graph.AddEdge(2, 3);
        set_graph.AddEdge(2, 4);

        assert(set_graph.GetNextVertices(0) == single_test1_answers);
        assert(set_graph.GetNextVertices(3) == single_test2_answers);
        assert(set_graph.GetPrevVertices(3) == single_test3_answers);
        assert(set_graph.GetPrevVertices(0) == single_test4_answers);
        assert(set_graph.VerticesCount() == size_answer);

        set_graph.AddEdge(3, 4);

        assert(set_graph.GetNextVertices(3) == single_test5_answers);
        assert(set_graph.GetPrevVertices(4) == single_test6_answers);
        assert(set_graph.VerticesCount() == size_answer);
        std::cout << "Single SetGraph test  passed\n";
    }

    {
        ArcGraph arc_graph(5);
        arc_graph.AddEdge(0, 1);
        arc_graph.AddEdge(0, 2);
        arc_graph.AddEdge(0, 3);
        arc_graph.AddEdge(1, 3);
        arc_graph.AddEdge(2, 3);
        arc_graph.AddEdge(2, 4);

        assert(arc_graph.GetNextVertices(0) == single_test1_answers);
        assert(arc_graph.GetNextVertices(3) == single_test2_answers);
        assert(arc_graph.GetPrevVertices(3) == single_test3_answers);
        assert(arc_graph.GetPrevVertices(0) == single_test4_answers);
        assert(arc_graph.VerticesCount() == size_answer);

        arc_graph.AddEdge(3, 4);

        assert(arc_graph.GetNextVertices(3) == single_test5_answers);
        assert(arc_graph.GetPrevVertices(4) == single_test6_answers);
        assert(arc_graph.VerticesCount() == size_answer);
        std::cout << "Single ArcGraph test  passed\n";
    }

    {
        MatrixGraph matrix_graph(5);
        matrix_graph.AddEdge(0, 1);
        matrix_graph.AddEdge(0, 2);
        matrix_graph.AddEdge(0, 3);
        matrix_graph.AddEdge(1, 3);
        matrix_graph.AddEdge(2, 3);
        matrix_graph.AddEdge(2, 4);

        assert(matrix_graph.GetNextVertices(0) == single_test1_answers);
        assert(matrix_graph.GetNextVertices(3) == single_test2_answers);
        assert(matrix_graph.GetPrevVertices(3) == single_test3_answers);
        assert(matrix_graph.GetPrevVertices(0) == single_test4_answers);
        assert(matrix_graph.VerticesCount() == size_answer);

        matrix_graph.AddEdge(3, 4);

        assert(matrix_graph.GetNextVertices(3) == single_test5_answers);
        assert(matrix_graph.GetPrevVertices(4) == single_test6_answers);
        assert(matrix_graph.VerticesCount() == size_answer);
        std::cout << "Single MatrixGraph test  passed\n";
    }

    // convert tests
    {
        ListGraph list_graph(5);
        list_graph.AddEdge(0, 1);
        list_graph.AddEdge(0, 2);
        list_graph.AddEdge(0, 3);
        list_graph.AddEdge(1, 3);
        list_graph.AddEdge(2, 3);
        list_graph.AddEdge(2, 4);

        SetGraph set_graph(list_graph);
        MatrixGraph matrix_graph(list_graph);
        ArcGraph arc_graph(list_graph);
        ListGraph list_graph2(list_graph);

        assert(set_graph.GetNextVertices(0) == single_test1_answers);
        assert(set_graph.GetNextVertices(3) == single_test2_answers);
        assert(set_graph.GetPrevVertices(3) == single_test3_answers);
        assert(set_graph.GetPrevVertices(0) == single_test4_answers);
        assert(set_graph.VerticesCount() == size_answer);

        assert(matrix_graph.GetNextVertices(0) == single_test1_answers);
        assert(matrix_graph.GetNextVertices(3) == single_test2_answers);
        assert(matrix_graph.GetPrevVertices(3) == single_test3_answers);
        assert(matrix_graph.GetPrevVertices(0) == single_test4_answers);
        assert(matrix_graph.VerticesCount() == size_answer);

        assert(arc_graph.GetNextVertices(0) == single_test1_answers);
        assert(arc_graph.GetNextVertices(3) == single_test2_answers);
        assert(arc_graph.GetPrevVertices(3) == single_test3_answers);
        assert(arc_graph.GetPrevVertices(0) == single_test4_answers);
        assert(arc_graph.VerticesCount() == size_answer);

        assert(list_graph2.GetNextVertices(0) == single_test1_answers);
        assert(list_graph2.GetNextVertices(3) == single_test2_answers);
        assert(list_graph2.GetPrevVertices(3) == single_test3_answers);
        assert(list_graph2.GetPrevVertices(0) == single_test4_answers);
        assert(list_graph2.VerticesCount() == size_answer);
        std::cout << "Convert graphs test  passed\n";
    }
}