//Требование для всех вариантов Задачи 5
//Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции.
//Решение должно поддерживать передачу функции сравнения снаружи.
//Общее время работы алгоритма O(n log n).
//
//5_2. Современники.
//Группа людей называется современниками если был такой момент, когда они могли собраться вместе. Для этого в этот
// момент каждому из них должно было уже исполниться 18 лет, но ещё не исполниться 80 лет.
//Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников. В день 18летия человек
// уже может принимать участие в собраниях, а в день 80летия и в день смерти уже не может.
//Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. В этих случаях принимать участие
// в собраниях он не мог.


#include <iostream>
#include <cassert>
#include <sstream>

template <class T>
struct Event {
    T year = 0;
    T month = 0;
    T day = 0;

    bool factor = false;  // true - достижение совершенолетия,
    // false - достижение 80-тилетия / смерть.
};

template <class T>
struct Person {             // очень мрачная структура
    T birth_year = 0;
    T birth_month = 0;
    T birth_day = 0;

    T death_year = 0;
    T death_month = 0;
    T death_day = 0;
};

template <class T>
class IsLessDefault{
public:
    IsLessDefault() = default;
    bool operator() (const T& l, const T& r) const { return  l < r; };
};

template <class T>
class IsDateLess{
public:
    IsDateLess() = default;
    bool operator() (const T& l, const T& r) const;
};

template<class T>
bool IsDateLess<T>::operator()(const T &l, const T &r) const {

    if (l.year < r.year) {
        return true;
    } else if (l.year > r.year) {
        return false;
    }

    if (l.month < r.month) {
        return true;
    } else if (l.month > r.month) {
        return false;
    }

    if (l.day < r.day) {
        return true;
    } else if (l.day > r.day) {
        return false;
    }

    return l.factor < r.factor;   // считаем, что смерть наступает раньше рождения

}


template <class T, class IsLess = IsLessDefault<T>>
class MergeSort {
public :
    explicit MergeSort(const IsLess &is_less = IsLessDefault<T>());
    MergeSort(const MergeSort &) = delete;
    ~MergeSort();

    MergeSort &operator=(const MergeSort &) = delete;

    void Add(const T& value);

    const T* GetSortedArray(size_t& size);
private:
    IsLess is_less = IsLessDefault<T>();

    T* buffer = nullptr;
    T* buffer_for_sort = nullptr;
    size_t buffers_size = 0;
    size_t real_size = 0;

    void GrowBuffer();

    void Sort(T* array, size_t array_size);

    void Merge(const T* array1, size_t array1_size, const T* array2, size_t array2_size);
};

template<class T, class IsLess>
void MergeSort<T, IsLess>::Merge(const T *array1, size_t array1_size, const T *array2, size_t array2_size) {
    if(array1_size == 0) { // если первый массив пустой
        for (int i = 0; i < array2_size; ++i) {
            buffer_for_sort[i] = array2[i];
        }
    } else if (array2_size == 0) { // если второй массив пустой
        for (int i = 0; i < array1_size; ++i) {
            buffer_for_sort[i] = array1[i];
        }
    }

    size_t i = 0;
    size_t j = 0;
    for (int k = 0; k < array1_size + array2_size; ++k) {
        if (is_less(array1[i], array2[j])) {
            buffer_for_sort[k] = array1[i];
            ++i;
        } else {
            buffer_for_sort[k] = array2[j];
            j++;
        }

        if (i == array1_size) {  // если первый массив кончился
            while (j != array2_size) {
                buffer_for_sort[++k] = array2[j++];
            }
            return;
        } else if (j == array2_size) { // если второй массив кончился
            while (i != array2_size) {
                ++k;
                buffer_for_sort[k] = array1[i];
                ++i;
            }
            return;
        }
    }
}

template<class T, class IsLess>
void MergeSort<T, IsLess>::GrowBuffer() {
    size_t new_size = (buffers_size == 0) ? 1 : buffers_size * 2;
    T* new_buffer = new T[new_size];
    T* new_buffer_for_sort = new T[new_size];

    for (size_t i = 0; i < buffers_size; ++i) {
        new_buffer[i] = buffer[i];
    }

    delete[] buffer;
    delete[] buffer_for_sort;

    buffer = new_buffer;
    buffer_for_sort = new_buffer_for_sort;
    buffers_size = new_size;
}

template<class T, class IsLess>
void MergeSort<T, IsLess>::Add(const T &value) {
    if (real_size == buffers_size) {
        GrowBuffer();
    }
    buffer[real_size] = value;
    ++real_size;
}

template<class T, class IsLess>
MergeSort<T, IsLess>::MergeSort(const IsLess &is_less)
        : is_less(is_less) {}

template<class T, class IsLess>
const T *MergeSort<T, IsLess>::GetSortedArray(size_t& size) {
    Sort(buffer, real_size);
    size = real_size;
    return buffer;
}

template<class T, class IsLess>
void MergeSort<T, IsLess>::Sort(T *array, size_t array_size) {
    if (array_size <= 1) {
        return;
    }

    size_t left_array_size = array_size / 2;
    size_t right_array_size = array_size - left_array_size;

    // глубина рекурсии меньше 1000 при длине массива < 2^1000 (~10^300)
    Sort(array, left_array_size);
    Sort(array + left_array_size, right_array_size);

    Merge(array, left_array_size, array + left_array_size, right_array_size);

    for (size_t i = 0; i < array_size; ++i) {
        array[i] = buffer_for_sort[i];
    }
}

template<class T, class IsLess>
MergeSort<T, IsLess>::~MergeSort() {
    delete[] buffer;
    delete[] buffer_for_sort;
}

template <class T, class IsLess = IsLessDefault<T>>
class MaxContemporaries {
public:
    explicit MaxContemporaries(const IsDateLess<Event<T>> &is_less = IsDateLess<Event<size_t>>());
    MaxContemporaries(const MaxContemporaries &) = delete;
    ~MaxContemporaries() = default;

    MaxContemporaries &operator=(const MaxContemporaries &) = delete;

    void Add(const Person<T>& person);
    size_t GetResult();


private:
    MergeSort<Event<T>, IsDateLess<Event<T>>> merge_sort;
    IsLess is_less = IsDateLess<Event<size_t>>();
};

template<class T, class IsLess>
MaxContemporaries<T, IsLess>::MaxContemporaries(const IsDateLess<Event<T>> &is_less)
        : merge_sort(is_less){}

template<class T, class IsLess>
void MaxContemporaries<T, IsLess>::Add(const Person<T>& person) {
    //  добавляем только доживших до 18
    if (!is_less({person.death_year,person.death_month,person.death_day},
                {person.birth_year + 18, person.birth_month, person.birth_day})) {

        Event<T> coming_of_age;
        coming_of_age.factor = true; //  "положительное" влияние
        coming_of_age.day = person.birth_day;
        coming_of_age.month = person.birth_month;
        coming_of_age.year = person.birth_year + 18;
        merge_sort.Add(coming_of_age);

        Event<T> coming_of_old_age_or_death;
        coming_of_old_age_or_death.factor = false; //  "отрицательное" влияние

        // добавляем либо дату смерти, либо дату восьмидесятилетия
        if (is_less({person.death_year, person.death_month, person.death_day},
                    {person.birth_year + 80, person.birth_month, person.birth_day})) {
            coming_of_old_age_or_death.day = person.death_day;
            coming_of_old_age_or_death.month = person.death_month;
            coming_of_old_age_or_death.year = person.death_year;
        } else {
            coming_of_old_age_or_death.day = person.birth_day;
            coming_of_old_age_or_death.month = person.birth_month;
            coming_of_old_age_or_death.year = person.birth_year + 80;
        }
        merge_sort.Add(coming_of_old_age_or_death);
    }
}

template<class T, class IsLess>
size_t MaxContemporaries<T, IsLess>::GetResult() {
    const Event<size_t>* arr;
    size_t size;
    arr = merge_sort.GetSortedArray(size);

    int64_t result = 0;
    int64_t current_value = 0;
    for (int i = 0; i < size; ++i) {
        current_value += arr[i].factor ? 1 : -1;
        if (current_value > result) {
            result = current_value;
        }
    }
    return result;
}

int run(std::istream& input, std::ostream& output) {
    IsDateLess<Event<size_t>> is_date_less;
    MaxContemporaries<size_t, IsDateLess<Event<size_t>>> max_contemporaries(is_date_less);

    size_t number_of_persons = 0;
    input >> number_of_persons;

    Person<size_t> great_person;

    for (size_t i = 0; i < number_of_persons; ++i) {
        input >> great_person.birth_day;
        input >> great_person.birth_month;
        input >> great_person.birth_year;

        input >> great_person.death_day;
        input >> great_person.death_month;
        input >> great_person.death_year;

        max_contemporaries.Add(great_person);
    }

    output << max_contemporaries.GetResult();
    return 0;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}