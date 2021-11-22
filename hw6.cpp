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
#include <cassert>

template <class T>
class IsLessDefault{
public:
    IsLessDefault() = default;
    bool operator() (const T& l, const T& r) const { return  l < r; };
};

template <class T, class IsLess = IsLessDefault<T>>
class KStat {
public :
    explicit KStat(const IsLess& is_less = IsLessDefault<T>());
    KStat(const KStat&) = delete;
    ~KStat();

    KStat& operator = (const KStat&) = delete;

    const T& GetValue(int64_t position);
    const T& GetPercentile(size_t percent);
    const T& GetMedian();
    void Add(const T& value, size_t id = 0);
    size_t Size();

private:
    IsLess is_less = IsLessDefault<T>();
    T* buffer = nullptr;
    size_t buffers_size = 0;
    size_t real_size = 0;

    void GrowBuffer();

    size_t GetPivotPos(int64_t left, int64_t right);
    size_t Partition(int64_t l, int64_t r);
};

template<class T, class IsLess>
KStat<T, IsLess>::KStat(const IsLess &is_less)
        : is_less(is_less) {}

template<class T, class IsLess>
KStat<T, IsLess>::~KStat() {
    delete[] buffer;
}

template<class T, class IsLess>
void KStat<T, IsLess>::GrowBuffer() {
    size_t new_size = (buffers_size == 0) ? 1 : buffers_size * 2;
    T* new_buffer = new T[new_size];

    for (size_t i = 0; i < buffers_size; ++i) {
        new_buffer[i] = buffer[i];
    }

    delete[] buffer;
    buffer = new_buffer;
    buffers_size = new_size;
}

template<class T, class IsLess>
void KStat<T, IsLess>::Add(const T &value, size_t id) {
    if (real_size == buffers_size) {
        GrowBuffer();
    }
    buffer[real_size] = value;
    ++real_size;
}

template<class T, class IsLess>
size_t KStat<T, IsLess>::GetPivotPos(int64_t left, int64_t right) {
    int64_t temp_arr[3] = {left, ((right - 1) - left) / 2, right - 1};
    for (size_t i = 1; i < 3; ++i) {                    // сортировка вставкой для трех элементов
        for (size_t j = i; j > 0; --j) {
            if (is_less(buffer[temp_arr[j]], buffer[temp_arr[j - 1]])) {
                std::swap(temp_arr[j], temp_arr[j - 1]);
            }
        }
    }
    return temp_arr[1]; //  возвращаем средний элемет
}

template<class T, class IsLess>
size_t KStat<T, IsLess>::Partition(int64_t left, int64_t right) {
    size_t pivot = GetPivotPos(left, right);
    std::swap(buffer[left], buffer[pivot]);
    for (int64_t i = right, j = right - 1; j >= left; --j) {
        if (is_less(buffer[left], buffer[j])) {
            --i;
            if (i != j) {
                std::swap(buffer[i], buffer[j]);
            }
        }

        if (j == left) {
            std::swap(buffer[i - 1], buffer[j]);
            pivot = i - 1;
        }
    }
    return pivot;
}

template<class T, class IsLess>
const T &KStat<T, IsLess>::GetValue(int64_t position) {
    int64_t left = 0;
    int64_t right = real_size;
    while (true) {
        int64_t pivot_pos = Partition(left, right);

        if (pivot_pos == position) {
            return buffer[pivot_pos];
        } else {
            pivot_pos < position ? left = pivot_pos + 1 : right = pivot_pos;
        }
    }
}

template<class T, class IsLess>
const T &KStat<T, IsLess>::GetPercentile(size_t percent) {
    assert(percent <= 100);
    return GetValue(real_size * percent / 100);
}

template<class T, class IsLess>
const T &KStat<T, IsLess>::GetMedian() {
    return GetValue(real_size / 2);
}

template<class T, class IsLess>
size_t KStat<T, IsLess>::Size() {
    return real_size;
}

int main() {
    KStat<size_t> k_stat;
    size_t number_of_values = 0;
    std::cin >>  number_of_values;
    size_t value = 0;
    for (size_t i = 0; i < number_of_values; ++i) {
        std::cin >>  value;
        k_stat.Add(value);
    }

    std::cout << k_stat.GetPercentile(10) << std::endl << k_stat.GetMedian()
              << std::endl << k_stat.GetPercentile(90);
    return 0;
}