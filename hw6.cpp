//Дано множество целых чисел из [0..10^9] размера n.
//Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
//10%  перцентиль
//        медиана
//90%  перцентиль
//
//        Требования: к дополнительной памяти: O(n).
//Среднее время работы: O(n)
//Должна быть отдельно выделенная функция partition.
//Рекурсия запрещена.
//Решение должно поддерживать передачу функции сравнения снаружи.
//
//6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition
//реализуйте методом прохода двумя итераторами от конца массива к началу.


#include <iostream>
#include <sstream>
#include <cassert>

template<class T>
class Vector{
public:
    Vector() = default;
    ~Vector() {delete[] buffer;}

    Vector(const Vector&);
    Vector& operator = (const Vector&) = delete;


    void Add(const T& value, int32_t id = 0);
    T& operator[] (int i) const;
    int32_t Size() const;

private:
    T* buffer = nullptr;
    int32_t buffers_size = 0;
    int32_t real_size = 0;

private:
    void GrowBuffer();
};

template <class T>
class IsLessDefault{
public:
    IsLessDefault() = default;
    bool operator() (const T& l, const T& r) const { return  l < r; };
};

template<class T, class IsLess>
int32_t GetPivotPos(const Vector<T>& vector, int32_t left, int32_t right, IsLess isLess) {
    int middle = left + ((right - 1) - left) / 2;
    // находим медиану трех isLess(vector[middle], vector[left])
    if (isLess(vector[right - 1], vector[left])) {
        if (isLess(vector[left], vector[middle])) {
            return left;
        } else {
            return (isLess(vector[middle], vector[right - 1])) ?
                   right - 1 : middle;
        }
    }
    if (isLess(vector[right - 1], vector[middle])) {
        return right - 1;
    } else {
        return (isLess(vector[middle], vector[left])) ?
               left : middle;
    }
}

template<class T, class IsLess>
int32_t Partition(Vector<T>& vector, int32_t left, int32_t right, IsLess isLess) {
    int32_t pivot = GetPivotPos(vector, left, right, isLess);
    std::swap(vector[left], vector[pivot]);

    int32_t i = right;
    // Идем с конца массива, справа от pivot оставляем значения >  buffer[pivot]
    for (int32_t j = right - 1; j >= left; --j) {
        if (isLess(vector[left], vector[j])) {
            --i;
            if (i != j) {
                std::swap(vector[i], vector[j]);
            }
        }
    }
    std::swap(vector[i - 1], vector[left]);

    return i - 1; // pivot
}

template<class T, class IsLess>
T GetValue(const Vector<T>& vector, int32_t position, IsLess isLess) {
    Vector<T> local_vector(vector);
    int32_t left = 0;
    int32_t right = local_vector.Size();
    while (true) {
        int32_t pivot_pos = Partition(local_vector, left, right, isLess);

        if (pivot_pos == position) {
            return local_vector[pivot_pos];
        } else {
            pivot_pos < position ? left = pivot_pos + 1 : right = pivot_pos;
        }
    }
}

template<class T, class IsLess = IsLessDefault<T>>
T GetPercentile(const Vector<T>& vector, int32_t percent, const IsLess& is_less = IsLessDefault<T>()) {
    return GetValue(vector, vector.Size() * percent / 100, is_less);
}

int run(std::istream& input, std::ostream& output) {
    Vector<int32_t> vector;
    int32_t number_of_values = 0;
    input >>  number_of_values;
    int32_t value = 0;
    for (int32_t i = 0; i < number_of_values; ++i) {
        input >>  value;
        vector.Add(value);
    }

    output << GetPercentile<int32_t, IsLessDefault<int32_t>>(vector, 10) << std::endl;
    output << GetPercentile<int32_t, IsLessDefault<int32_t>>(vector, 50) << std::endl;
    output << GetPercentile<int32_t, IsLessDefault<int32_t>>(vector, 90) << std::endl;

    return 0;

}



int main() {
    run(std::cin, std::cout);
    return 0;
}


template<class T>
void Vector<T>::Add(const T &value, int32_t id) {
    if (real_size == buffers_size) {
        GrowBuffer();
    }
    buffer[real_size] = value;
    ++real_size;
}

template<class T>
void Vector<T>::GrowBuffer() {
    int32_t new_size = (buffers_size == 0) ? 1 : buffers_size * 2;
    T* new_buffer = new T[new_size];

    for (int32_t i = 0; i < buffers_size; ++i) {
        new_buffer[i] = buffer[i];
    }

    delete[] buffer;
    buffer = new_buffer;
    buffers_size = new_size;
}

template<class T>
int32_t Vector<T>::Size() const{
    return real_size;
}

template<class T>
T &Vector<T>::operator[](int i) const{
    assert(i < (int)Size());
    return buffer[i];
}

template<class T>
Vector<T>::Vector(const Vector &vector) {
    this->buffer = new T[vector.buffers_size];
    this->buffers_size = vector.buffers_size;
    this->real_size = vector.real_size;

    for (int i = 0; i < vector.real_size; ++i) {
        this->buffer[i] = vector.buffer[i];
    }
}