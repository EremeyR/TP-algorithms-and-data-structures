#include <iostream>
#include <sstream>
#include <cassert>

template <class T>
class IsLessDefault{
public:
    IsLessDefault() = default;
    bool operator() (const T& l, const T& r) const { return  l < r; };
};

template <class T, class IsLess = IsLessDefault<T>>
class Heap {
public :
    explicit Heap(const IsLess& is_less = IsLessDefault<T>());
    Heap(const Heap&) = delete;
    ~Heap();

    Heap& operator = (const Heap&) = delete;

    void BuildHeap(T* array, size_t size);
    void Add(const T& value, size_t id = 0);
    const T& PeekMax() const;
    T ExtractMax(size_t* id = nullptr);
    int Size();

private:
    IsLess is_less = IsLessDefault<T>();
    T* buffer = nullptr;
    size_t* id_buffer = nullptr;
    size_t buffers_size = 0;
    size_t heap_size = 0;


    void GrowBuffers();
    void SiftDown(size_t position);
    void SiftUp(size_t position);
};

template <class T, class IsLess = IsLessDefault<T>>
class Sort {
public:
    explicit Sort(const IsLess& is_less = IsLessDefault<T>());
    Sort(const Sort&) = delete;
    ~Sort();

    Sort& operator = (const Sort&) = delete;

    void SetNumberOfArrays(size_t number);
    void SetArraySize(size_t number_of_array, size_t size);
    void AddValue(size_t number_of_array, size_t position, const T& value);

    const T* GetResult(size_t& size);

private:
    Heap<T, IsLessDefault<T>> heap;

    size_t number_of_arrays = 0;
    size_t number_of_values = 0;

    IsLess is_less = IsLessDefault<T>();
    T** arrays_buffer = nullptr;
    T* result_buffer = nullptr;
    size_t* arrays_length = nullptr;
};

int run(std::istream& input, std::ostream& output) {
    Sort<int64_t , IsLessDefault<int64_t>> sort;

    size_t number_of_arrays = 0;
    input >> number_of_arrays;
    sort.SetNumberOfArrays(number_of_arrays);

    size_t array_length = 0;
    int64_t value = 0;
    for (size_t i = 0; i < number_of_arrays; ++i) {
        input >> array_length;
        sort.SetArraySize(i, array_length);
        for (size_t j = 0; j < array_length; ++j) {
            input >> value;
            sort.AddValue(i, j, value);
        }
    }
    const int64_t* result;
    size_t result_size = 0;
    result = sort.GetResult(result_size);
    for (int i = 0; i < result_size; ++i) {
        output << result[i] << " ";
    }

    return 0;
}

int main() {
    //Tests();
    run(std::cin, std::cout);
    return 0;
}

template<class T, class IsLess>
void Heap<T, IsLess>::GrowBuffers() {
    size_t new_size = (buffers_size == 0) ? 1 : buffers_size * 2;
    T* new_buffer = new T[new_size];
    auto* new_id_buffer = new size_t[new_size];

    for (size_t i = 0; i < buffers_size; ++i) {
        new_buffer[i] = buffer[i];
    }

    for (size_t i = 0; i < buffers_size; ++i) {
        new_id_buffer[i] = id_buffer[i];
    }

    for (size_t i = 0; i < buffers_size; ++i) {
        new_id_buffer[i] = id_buffer[i];
    }
    delete[] buffer;
    delete[] id_buffer;
    id_buffer = new_id_buffer;
    buffer = new_buffer;
    buffers_size = new_size;
}

template<class T, class IsLess>
void Heap<T, IsLess>::Add(const T &value, size_t id) {
    if (heap_size == buffers_size) {
        GrowBuffers();
    }
    buffer[heap_size] = value;
    id_buffer[heap_size] = id;
    SiftUp(heap_size);
    heap_size ++;
}

template<class T, class IsLess>
Heap<T, IsLess>::~Heap() {
    delete[] buffer;
    delete[] id_buffer;
}

template<class T, class IsLess>
const T &Heap<T, IsLess>::PeekMax() const {
    assert(heap_size != 0);
    return buffer[0];
}

template<class T, class IsLess>
T Heap<T, IsLess>::ExtractMax(size_t* id) {
    assert(heap_size != 0);

    T max = buffer[0];
    *id = id_buffer[0];
    buffer[0] = buffer[heap_size - 1];
    id_buffer[0] = id_buffer[heap_size - 1];
    --heap_size;

    SiftDown(0);
    return max;
}

template<class T, class IsLess>
void Heap<T, IsLess>::SiftDown(size_t position) {
    while (position < heap_size) {
        size_t left = 2 * position + 1;
        size_t right = 2 * position + 2;
        size_t largest = position;
        if (left < heap_size && is_less(buffer[largest], buffer[left])) {
            largest = left;
        }
        if (right < heap_size && is_less(buffer[largest], buffer[right])) {
            largest = right;
        }
        if (largest != position) {
            std::swap(buffer[position], buffer[largest]);
            std::swap(id_buffer[position], id_buffer[largest]);
            position = largest;
        } else {
            return;
        }
    }
}

template<class T, class IsLess>
void Heap<T, IsLess>::SiftUp(size_t position) {
    while (position > 0) {
        size_t parent = (position - 1) / 2;
        if (is_less(buffer[parent], buffer[position])) {
            std::swap(buffer[parent], buffer[position]);
            std::swap(id_buffer[parent], id_buffer[position]);
            position = parent;
        } else {
            return;
        }
    }
}

template<class T, class IsLess>
void Heap<T, IsLess>::BuildHeap(T *array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        Add(array[i], i);
    }

    for (long i = (long)size / 2 - 1; i >= 0; --i) {
        SiftDown(i);
    }
}

template<class T, class IsLess>
int Heap<T, IsLess>::Size() {
    return heap_size;
}

template<class T, class IsLess>
Heap<T, IsLess>::Heap(const IsLess &is_less)
        : is_less(is_less) {}

template<class T, class IsLess>
Sort<T, IsLess>::Sort(const IsLess &is_less)
        : is_less(is_less) {}

template<class T, class IsLess>
Sort<T, IsLess>::~Sort() {
    for (int i = 0; i < number_of_arrays; ++i) {
        delete[] arrays_buffer[i];
    }
    delete[] arrays_buffer;
    delete[] result_buffer;
    delete[] arrays_length;
}

template<class T, class IsLess>
void Sort<T, IsLess>::SetNumberOfArrays(size_t number) {
    number_of_arrays = number;
    arrays_buffer = new T*[number];
    arrays_length = new size_t[number];
}

template<class T, class IsLess>
void Sort<T, IsLess>::SetArraySize(size_t number_of_array, size_t size) {
    assert(number_of_array <= number_of_arrays);
    assert(arrays_buffer != nullptr);

    arrays_buffer[number_of_array] = new T[size];
    arrays_length[number_of_array] = size;
    number_of_values += size;
}

template<class T, class IsLess>
void Sort<T, IsLess>::AddValue(size_t number_of_array, size_t position, const T &value) {
    assert(number_of_array <= number_of_arrays);
    assert(position <= arrays_length[number_of_array]);
    assert(arrays_buffer != nullptr);
    assert(arrays_buffer[position] != nullptr);

    arrays_buffer[number_of_array][position] = value;
}

template<class T, class IsLess>
const T *Sort<T, IsLess>::GetResult(size_t& size) {
    result_buffer = new T[number_of_values];

    for (size_t i = 0; i < number_of_arrays; ++i) {                 //  Добавляем в кучу последние элементы всех
        heap.Add(arrays_buffer[i][--arrays_length[i]], i);    //  массивов.
    }

    size_t id = 0;
    for (long i = number_of_values - 1; i >= 0; --i) {    //  Заполняем массив с конца.
        result_buffer[i] = heap.ExtractMax(&id);
        if(arrays_length[id] != 0) {
            heap.Add(arrays_buffer[id][--arrays_length[id]], id);
        }
    }

    size = number_of_values;
    return result_buffer;
}