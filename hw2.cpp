// 2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
// Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента
// массива A, равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет,
//выведите n. n, m ≤ 10000.
// Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)). Внимание!
// В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска
// размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать
// бинарный поиск.

#include <iostream>
#include "string"
#include <cstring>
#include <cassert>

template<class T>
class MinimumIndexes {
public:
    MinimumIndexes() = default;
    MinimumIndexes& operator = (const MinimumIndexes&) = delete;
    MinimumIndexes(const MinimumIndexes&) = delete;
    ~MinimumIndexes();

    void AddToFirstArray(const T& value);
    void AddToSecondArray(const T& value);
    T* GetResultArray(size_t& size);

private:
    T* first_array = nullptr;
    size_t first_array_size = 0;
    size_t first_buf_size = 0;

    T* second_array = nullptr;
    size_t second_array_size = 0;
    size_t second_buf_size = 0;

    size_t ExponentialSearch(const T& value) const;
    size_t BinarySearch(size_t boundary, const T &value) const;

    size_t* result_array = nullptr;

    void Grow(T** array, size_t& size);

    void CalculateResult();
};

int main() {
    size_t first_array_size = 0;
    size_t second_array_size = 0;
    std::cin >> first_array_size >> second_array_size;

    MinimumIndexes<size_t> minimum_indexes;
    size_t value = 0;
    for(size_t i = 0; i < first_array_size; ++i) {
        std::cin >> value;
        minimum_indexes.AddToFirstArray(value);
    }

    for(size_t i = 0; i < second_array_size; ++i) {
        std::cin >> value;
        minimum_indexes.AddToSecondArray(value);
    }
    size_t array_size = 0;
    size_t* result_array = minimum_indexes.GetResultArray(array_size);

    for (size_t i = 0; i < array_size; ++i) {
        std::cout << result_array[i] << " ";
    }
    return 0;
}


template<class T>
void MinimumIndexes<T>::AddToFirstArray(const T &value) {
    if(first_array_size == first_buf_size) {
        Grow(&first_array, first_buf_size);
    }
    first_array[first_array_size++] = value;
}

template<class T>
void MinimumIndexes<T>::AddToSecondArray(const T &value) {
    if(second_array_size == second_buf_size) {
        Grow(&second_array, second_buf_size);
    }
    second_array[second_array_size++] = value;
}

template<class T>
void MinimumIndexes<T>::Grow(T** array, size_t &size) {
    size_t new_size = (size == 0) ? 1 : size * 2;
    T* new_array = new T[new_size];

    for (size_t i = 0; i < size; ++i) {
        new_array[i] = (*array)[i];
    }

    delete[] *array;

    *array = new_array;
    size = new_size;
}

template<class T>
T *MinimumIndexes<T>::GetResultArray(size_t& size) {
    CalculateResult();
    size = second_array_size;
    return result_array;
}

template<class T>
MinimumIndexes<T>::~MinimumIndexes() {
    delete[] first_array;
    delete[] second_array;
    delete[] result_array;
}

template<class T>
void MinimumIndexes<T>::CalculateResult() {
    result_array = new T[second_array_size];
    for (size_t i = 0; i < second_array_size; ++i) {
        size_t boundary = ExponentialSearch(second_array[i]);
        result_array[i] = BinarySearch(boundary, second_array[i]);
        assert(result_array[i] != first_array_size);
    }
}

template<class T>
size_t MinimumIndexes<T>::ExponentialSearch(const T &value) const {
    size_t boundary = 1;
    while(boundary < first_array_size && first_array[boundary - 1] < value) {
        boundary *= 2;
    }
    return  boundary;
}

template<class T>
size_t MinimumIndexes<T>::BinarySearch(size_t boundary, const T &value) const {
    size_t first = boundary / 2;
    size_t last = (boundary < first_array_size) ? boundary : first_array_size;
    while (first < last) {
        size_t middle = (first + last) / 2;
        assert(middle < first_array_size);
        first_array[middle] < value ? first = middle + 1 : last = middle;
    }

    return first;
}