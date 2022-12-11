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

template<class T>
class Vector{
public:
    Vector() = default;
    explicit Vector(int size);
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



template<class T, class IsLess>
void Merge(T *buffer_for_sort, const T *array1, size_t array1_size, const T *array2, size_t array2_size, const IsLess& is_less) {
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
void MergeSort(T *buffer_for_sort, T *array, size_t array_size, const IsLess& is_less) {
    if (array_size <= 1) {
        return;
    }


    size_t left_array_size = array_size / 2;
    size_t right_array_size = array_size - left_array_size;

    // глубина рекурсии меньше 1000 при длине массива < 2^1000 (~10^300)
    MergeSort(buffer_for_sort, array, left_array_size, is_less);
    MergeSort(buffer_for_sort, array + left_array_size, right_array_size, is_less);

    Merge(buffer_for_sort, array, left_array_size, array + left_array_size, right_array_size, is_less);

    for (size_t i = 0; i < array_size; ++i) {
        array[i] = buffer_for_sort[i];
    }
}

template<class T, class IsLess>
Vector<T> CorrectVector(const Vector<T>& vector, const IsLess& is_less) {
    Vector<T> corrected_vector;

    for (int i = 0; i < vector.Size(); i += 2) {
        T coming_of_age = {vector[i].year + 18, vector[i].month, vector[i].day, true};
        T coming_of_old = {vector[i].year + 80, vector[i].month, vector[i].day, false};
        T coming_of_death = {vector[i + 1].year, vector[i + 1].month, vector[i + 1].day, false};

        if (!is_less(coming_of_death, coming_of_age)) {  //  добавляем только доживших до 18
            corrected_vector.Add(coming_of_age);

            if (is_less(coming_of_death, coming_of_old)) {    //  если человеку было больше восьмидесяти,
                // то добавляем день восьмидесятилетия
                corrected_vector.Add(coming_of_death);
            } else {
                corrected_vector.Add(coming_of_old);
            }
        }
    }
    return corrected_vector;
}

template<class T, class IsLess = IsLessDefault<T>>
size_t GetMaxContemporaries(const Vector<T>& vector, const IsLess& is_less = IsLessDefault<T>()) {
    Vector<T> corrected_vector = CorrectVector(vector, is_less);
    Vector<T> vector_for_sort(corrected_vector.Size());

    MergeSort(&vector_for_sort[0], &corrected_vector[0], corrected_vector.Size(), is_less);

    int64_t result = 0;
    int64_t current_value = 0;
    for (int i = 0; i < corrected_vector.Size(); ++i) {
        current_value += corrected_vector[i].factor ? 1 : -1;
        if (current_value > result) {
            result = current_value;
        }
    }
    return result;
}

int run(std::istream& input, std::ostream& output) {
    IsDateLess<Event<size_t>> is_date_less;

    size_t number_of_persons = 0;
    input >> number_of_persons;

    Event<size_t> birth;
    Event<size_t> death;

    Vector<Event<size_t>> events;

    for (size_t i = 0; i < number_of_persons; ++i) {
        input >> birth.day;
        input >> birth.month;
        input >> birth.year;

        input >> death.day;
        input >> death.month;
        input >> death.year;

        events.Add(birth);
        events.Add(death);
    }
    output << GetMaxContemporaries(events, is_date_less);
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

    //Tests();
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

template<class T>
Vector<T>::Vector(int size) {
    buffers_size = size;
    buffer = new T[size];
}
