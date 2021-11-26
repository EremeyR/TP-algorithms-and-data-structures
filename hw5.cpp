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
class MaxContemporaries {
public:
    explicit MaxContemporaries(const IsDateLess<T> &is_less = IsDateLess<T>());
    MaxContemporaries(const MaxContemporaries &) = delete;
    ~MaxContemporaries();

    MaxContemporaries &operator=(const MaxContemporaries &) = delete;

    void Add(const T& birth, const T& death);
    size_t GetResult();

private:
    IsLess is_less = IsDateLess<Event<size_t>>();

    T* buffer = nullptr;            //  буфер для событий
    T* buffer_for_sort = nullptr;   // буфер для сортировки
    size_t buffers_size = 0;        // размер буфера
    size_t real_size = 0;           // количество событий

    void GrowBuffer();

    void MergeSort(T* array, size_t array_size);
    void Merge(const T* array1, size_t array1_size, const T* array2, size_t array2_size);
};

template<class T, class IsLess>
void MaxContemporaries<T, IsLess>::Add(const T& birth, const T& death) {
    
    T coming_of_age = {birth.year + 18, birth.month, birth.day, true};
    T coming_of_old = {birth.year + 80, birth.month, birth.day, false};
    T coming_of_death = {death.year, death.month, death.day, false};
    
    if (!is_less(coming_of_death, coming_of_age)) {  //  добавляем только доживших до 18
        if(real_size == buffers_size) {
            GrowBuffer();
        }
        buffer[real_size] = coming_of_age;
        ++real_size;

        if (is_less(coming_of_death, coming_of_old)) {
            buffer[real_size] = coming_of_death;
        } else {
            buffer[real_size] = coming_of_old;
        }
        ++real_size;
    }
}

template<class T, class IsLess>
size_t MaxContemporaries<T, IsLess>::GetResult() {
    MergeSort(buffer, real_size);

    int64_t result = 0;
    int64_t current_value = 0;
    for (int i = 0; i < real_size; ++i) {
        current_value += buffer[i].factor ? 1 : -1;
        if (current_value > result) {
            result = current_value;
        }
    }
    return result;
}

template<class T, class IsLess>
void MaxContemporaries<T, IsLess>::Merge(const T *array1, size_t array1_size, const T *array2, size_t array2_size) {
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
void MaxContemporaries<T, IsLess>::GrowBuffer() {
    size_t new_size = (buffers_size == 0) ? 2 : buffers_size * 2;
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
void MaxContemporaries<T, IsLess>::MergeSort(T *array, size_t array_size) {
    if (array_size <= 1) {
        return;
    }

    size_t left_array_size = array_size / 2;
    size_t right_array_size = array_size - left_array_size;

    // глубина рекурсии меньше 1000 при длине массива < 2^1000 (~10^300)
    MergeSort(array, left_array_size);
    MergeSort(array + left_array_size, right_array_size);

    Merge(array, left_array_size, array + left_array_size, right_array_size);

    for (size_t i = 0; i < array_size; ++i) {
        array[i] = buffer_for_sort[i];
    }
}

template<class T, class IsLess>
MaxContemporaries<T, IsLess>::~MaxContemporaries() {
    delete[] buffer;
    delete[] buffer_for_sort;
}

template<class T, class IsLess>
MaxContemporaries<T, IsLess>::MaxContemporaries(const IsDateLess<T> &is_less) :
    is_less(is_less){}

int run(std::istream& input, std::ostream& output) {
    IsDateLess<Event<size_t>> is_date_less;
    MaxContemporaries<Event<size_t>, IsDateLess<Event<size_t>>> max_contemporaries(is_date_less);

    size_t number_of_persons = 0;
    input >> number_of_persons;

    Event<size_t> birth;
    Event<size_t> death;

    for (size_t i = 0; i < number_of_persons; ++i) {
        input >> birth.day;
        input >> birth.month;
        input >> birth.year;

        input >> death.day;
        input >> death.month;
        input >> death.year;

        max_contemporaries.Add(birth, death);
    }

    output << max_contemporaries.GetResult();
    return 0;
}
void Tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 "
              << "2 5 1980 13 11 2055 "
              << "1 1 1982 1 1 2030 "
              << "2 1 1920 2 1 2000";
        run(input, output);
        assert(output.str() == "3");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "1 "
              << "2 5 1980 13 11 2055 ";
        run(input, output);
        assert(output.str() == "1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "2 "
              << "1 1 1982 1 1 2030 "
              << "2 1 2020 2 1 2050";
        run(input, output);
        assert(output.str() == "1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "2 "
              << "1 1 1922 1 1 2030 "
              << "1 1 2012 2 1 2050";
        run(input, output);
        assert(output.str() == "1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "3 "
              << "1 1 1922 1 1 2031 "
              << "1 1 2012 2 1 2050 "
              << "1 1 2040 2 1 2080";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 "
              << "1 1 1922 1 1 2031 "
              << "1 1 2012 2 1 2050 "
              << "1 1 2040 2 1 2080 "
              << "1 1 1922 1 1 2031 "
              << "1 1 2012 2 1 2050 "
              << "1 1 2040 2 1 2080";
        run(input, output);
        assert(output.str() == "2");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 "
              << "1 3 1992 3 1 2031 "
              << "1 2 1992 1 1 2031 "
              << "1 1 1992 2 1 2031 "
              << "1 1 1992 1 1 2031 ";
        run(input, output);
        assert(output.str() == "4");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "2 "
              << "1 3 1992 3 1 2030 "
              << "3 1 2012 1 1 2031 ";
        run(input, output);
        assert(output.str() == "1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "2 "
              << "4 3 1900 3 2 2000 "
              << "4 3 1962 1 1 2031 ";
        run(input, output);
        assert(output.str() == "1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "2 "
              << "4 3 1900 2 3 1980 "
              << "2 3 1962 1 1 2031 ";
        run(input, output);
        assert(output.str() == "1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "10 "
              << "4 3 1000 2 3 1070 "
              << "1 3 1062 1 1 1099 "
              << "4 3 1000 2 3 1070 "
              << "1 3 1062 1 1 1099 "
              << "4 3 1000 2 3 1070 "
              << "1 3 1062 1 1 1099 "
              << "4 3 1000 2 3 1070 "
              << "1 3 1062 1 1 1099 "
              << "4 3 1000 2 3 1070 "
              << "1 3 1062 1 1 1099 ";
        run(input, output);
        assert(output.str() == "5");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "10 "
              << "4 3 1000 2 3 1070 "
              << "1 3 1062 1 1 1099 "
              << "4 3 1100 2 3 1170 "
              << "1 3 1162 1 1 1199 "
              << "4 3 1200 2 3 1270 "
              << "1 3 1262 1 1 1299 "
              << "4 3 1300 2 3 1370 "
              << "1 3 1362 1 1 1399 "
              << "4 3 1400 2 3 1470 "
              << "1 3 1462 1 1 1499 ";
        run(input, output);
        assert(output.str() == "1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 "
              << "4 3 1030 4 3 1040 "
              << "3 3 1082 1 1 1099 ";
        run(input, output);
        assert(output.str() == "0");
    }
}

int main() {
    Tests();
    //run(std::cin, std::cout);
    return 0;
}