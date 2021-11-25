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

    const T& GetValue(int32_t position);
    const T& GetPercentile(int32_t percent);
    const T& GetMedian();
    void Add(const T& value, int32_t id = 0);
    int32_t Size();

private:
    IsLess is_less = IsLessDefault<T>();
    T* buffer = nullptr;
    int32_t buffers_size = 0;
    int32_t real_size = 0;
    int32_t pivot = 0;

    void GrowBuffer();

    int32_t GetPivotPos(int32_t left, int32_t right);
    int32_t Partition(int32_t l, int32_t r);
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
    int32_t new_size = (buffers_size == 0) ? 1 : buffers_size * 2;
    T* new_buffer = new T[new_size];

    for (int32_t i = 0; i < buffers_size; ++i) {
        new_buffer[i] = buffer[i];
    }

    delete[] buffer;
    buffer = new_buffer;
    buffers_size = new_size;
}

template<class T, class IsLess>
void KStat<T, IsLess>::Add(const T &value, int32_t id) {
    if (real_size == buffers_size) {
        GrowBuffer();
    }
    buffer[real_size] = value;
    ++real_size;
}

template<class T, class IsLess>
int32_t KStat<T, IsLess>::GetPivotPos(int32_t left, int32_t right) {
    int middle = left + ((right - 1) - left) / 2;
    // находим медиану трех
    if (buffer[left] > buffer[right - 1]) {
        if (buffer[middle] > buffer[left]) {
            return left;
        } else {
            return (buffer[right - 1] > buffer[middle]) ?
                   right - 1 : middle;
        }
    }
    if (buffer[middle] > buffer[right - 1]) {
        return right - 1;
    } else {
        return (buffer[left] > buffer[middle]) ?
               left : middle;
    }
}

template<class T, class IsLess>
int32_t KStat<T, IsLess>::Partition(int32_t left, int32_t right) {
    pivot = GetPivotPos(left, right);
    std::swap(buffer[left], buffer[pivot]);

    int32_t i = right;
    // Идем с конца массива, справа от pivot оставляем значения >  buffer[pivot]
    for (int32_t j = right - 1; j >= left; --j) {
        if (is_less(buffer[left], buffer[j])) {
            --i;
            if (i != j) {
                std::swap(buffer[i], buffer[j]);
            }
        }
    }
    std::swap(buffer[i - 1], buffer[left]);

    return i - 1; // pivot
}

template<class T, class IsLess>
const T &KStat<T, IsLess>::GetValue(int32_t position) {
    int32_t left = 0;
    int32_t right = real_size;
    while (true) {
        int32_t pivot_pos = Partition(left, right);

        if (pivot_pos == position) {
            return buffer[pivot_pos];
        } else {
            pivot_pos < position ? left = pivot_pos + 1 : right = pivot_pos;
        }
    }
}

template<class T, class IsLess>
const T &KStat<T, IsLess>::GetPercentile(int32_t percent) {
    return GetValue(real_size * percent / 100);
}

template<class T, class IsLess>
const T &KStat<T, IsLess>::GetMedian() {
    return GetValue(real_size / 2);
}

template<class T, class IsLess>
int32_t KStat<T, IsLess>::Size() {
    return real_size;
}

int run(std::istream& input, std::ostream& output) {
    KStat<int32_t> k_stat;
    int32_t number_of_values = 0;
    input >>  number_of_values;
    int32_t value = 0;
    for (int32_t i = 0; i < number_of_values; ++i) {
        input >>  value;
        k_stat.Add(value);
    }
    output << k_stat.GetPercentile(10) << "\n" << k_stat.GetMedian()
              << "\n" << k_stat.GetPercentile(90);
    return 0;

}

int main() {
    run(std::cin, std::cout);
    return 0;
}