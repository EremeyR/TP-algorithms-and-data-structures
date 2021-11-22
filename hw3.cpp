//Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
//Формат входных данных.
//В первой строке количество команд n. n ≤ 1000000.
//Каждая команда задаётся как 2 целых числа: a b.
//a = 1 - push front
//        a = 2 - pop front
//        a = 3 - push back
//        a = 4 - pop back
//        Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
//Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
//Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
//Формат выходных данных.
//Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
//
//3_2. Реализовать дек с динамическим зацикленным буфером.
//Требования: Дек должен быть реализован в виде класса.

#include <iostream>
#include <cassert>

template<class T>
class Deque {
public:
    Deque() = default;
    ~Deque();
    Deque& operator = (const Deque&) = delete;
    Deque(const Deque&) = delete;

    void PushFront(const T& value);
    void PushBack(const T& value);
    T PopBack();
    T PopFront();

    size_t Size();
    bool IsEmpty();

private:
    T* buffer = nullptr;
    size_t buffer_size = 0;
    size_t front = 0;
    size_t back = 0;

    void Grow();
    void Reduce();
};

int main() {
    Deque<size_t> deque;

    size_t number_of_commands = 0;
    std::cin >> number_of_commands;

    bool is_success = true;
    for (size_t i = 0; i < number_of_commands; ++i) {
        size_t command = 0;
        size_t value = 0;
        std::cin >> command >> value;
        switch (command) {
            case 1:
                deque.PushFront(value);
                break;

            case 2:
                is_success = is_success && (value == deque.PopFront());
                break;

            case 3:
                deque.PushBack(value);
                break;

            case 4:
                is_success = is_success && (value == deque.PopBack());
                break;

            default:
                assert(false);
        }
    }

    std::cout << (is_success ? "YES" : "NO");
    return 0;
}

template<class T>
Deque<T>::~Deque() {
    delete[] buffer;
}

template<class T>
void Deque<T>::Grow() {
    size_t new_size = (buffer_size == 0) ? 1 : buffer_size * 2;
    T* new_buffer = new T[new_size];

    if(buffer_size != 0) {
        for (size_t new_i = 0, old_i = front;  new_i < buffer_size; ++new_i, ++old_i) {
            if (old_i == buffer_size) {
                old_i = 0;
            }
            new_buffer[new_i] = buffer[old_i];
        }
    }

    delete[] buffer;
    buffer = new_buffer;
    front = 0;
    back = buffer_size;
    buffer_size = new_size;
}

template<class T>
void Deque<T>::PushBack(const T &value) {
    if (back == front || back == buffer_size && front == 0) {
        Grow();
    }

    back == buffer_size ? buffer[0] = value : buffer[back] = value;
    back = (back == buffer_size) ? 1 : back + 1;
}

template<class T>
size_t Deque<T>::Size() {
    if (front == 0 && back == 0) {
        return  0;
    } else {
        return (front < back) ? back - front : back + buffer_size - front;
    }
}

template<class T>
bool Deque<T>::IsEmpty() {
    return !(bool)Size();
}

template<class T>
T Deque<T>::PopFront() {
    if (IsEmpty()) {
        return static_cast<T>(-1);
    } else {
        size_t old_front = front;
        if (Size() == 1) {
            front = 0;
            back = 0;
        } else {
            front = (front == buffer_size - 1) ? 0 : front + 1;
        }

        if (buffer_size / 4 >= Size()) {
            Reduce();
        }
        return buffer[old_front];
    }
}

template<class T>
T Deque<T>::PopBack() {
    if (IsEmpty()) {
        return static_cast<T>(-1);
    } else {
        size_t old_back = back;
        if (Size() == 1) {
            front = 0;
            back = 0;
        } else {
            back = (back == 1) ? buffer_size : back - 1;
        }

        if (buffer_size / 4 >= Size()) {
            Reduce();
        }
        return buffer[old_back - 1];
    }
}

template<class T>
void Deque<T>::PushFront(const T &value) {
    if (back == front || back == buffer_size && front == 0) {
        Grow();
    }

    front = (front == 0) ? buffer_size - 1 : front - 1;
    buffer[front] = value;
}

template<class T>
void Deque<T>::Reduce() {
    size_t new_size = (buffer_size == 2) ? 0 : buffer_size / 2;  //  Если вызов функции произошел при длине массива 2 =>
    //  массив пустой
    T* new_buffer = new T[new_size];

    if(buffer_size != 1) {
        for (size_t new_i = 0, old_i = front;  new_i < new_size / 2; ++new_i, ++old_i) {
            if (old_i == buffer_size) {
                old_i = 0;
            }
            new_buffer[new_i] = buffer[old_i];
        }
    }

    delete[] buffer;
    buffer = new_buffer;
    buffer_size = new_size;

    front = 0;
    back = buffer_size / 2;
}