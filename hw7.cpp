#include <iostream>
#include <cassert>

class MSE {
public:
    MSE() = default;
    MSE(const MSE &) = delete;
    ~MSE();

    MSE &operator=(const MSE &) = delete;

    void Add(const std::string& str);
    const std::string* GetSorted(const size_t& size);

    void a (){
        for (int i = 0; i < real_size; ++i) {
            std::cout << buffer[indexes_array[i]] << std::endl;
        }
    }


private:
    size_t sort_buf_size = 27; //// 26 букв + '\0'

    std::string* buffer = nullptr;
    size_t* indexes_array = nullptr;

    size_t buffers_size = 0;
    size_t real_size = 0;

    size_t GetPosition(char value) const;

    void Sort(const int64_t start_pos, const int64_t end_pos, const size_t char_number, size_t* sort_buf);
    void InitIndexesArray();
    void GrowBuffers();
};

void MSE::GrowBuffers() {
    size_t new_size = (buffers_size == 0) ? 1 : buffers_size * 2;
    auto* new_buffer = new std::string[new_size];
    auto* new_indexes = new size_t[new_size];

    for (size_t i = 0; i < buffers_size; ++i) {
        new_buffer[i] = buffer[i];
    }


    delete[] buffer;
    delete[] indexes_array;

    buffer = new_buffer;
    indexes_array = new_indexes;
    buffers_size = new_size;
}

void MSE::Add(const std::string& str) {
    if (real_size == buffers_size) {
        GrowBuffers();
    }
    buffer[real_size] = str;
    ++real_size;
}

MSE::~MSE() {
    delete[] buffer;
    delete[] indexes_array;
}

size_t MSE::GetPosition(char value) const {
    if (value =='\0'){
        return 0;
    }

    assert(static_cast<size_t>(value) >= static_cast<size_t>('a'));
    assert(static_cast<size_t>(value) <= static_cast<size_t>('z'));

    return (static_cast<size_t>(value) + 1) - static_cast<size_t>('a');
}

void MSE::Sort(const int64_t start_pos, const int64_t end_pos, const size_t char_number, size_t* sort_buf) {

    auto* temp_indexes = new size_t[end_pos - start_pos];

    for(size_t i = 0; i < sort_buf_size; ++i ) {
        sort_buf[i] = 0;
    }

    for(size_t i = start_pos; i < end_pos; ++i) {
        char temp_char = buffer[indexes_array[i]][char_number];
        size_t temp_position = GetPosition(buffer[indexes_array[i]][char_number]);
        ++sort_buf[temp_position]; //  получаем номер позиции буквы
                                   //  стоящей на позиции из массива с
                                   //  отсортированными индексами и
                                   //  повышаем значение соответствующего
                                   //  элемента в массиве sort_buf[]
    }

    std::cout << sort_buf[0] << " ";
    for(size_t i = 1; i < sort_buf_size; ++i) {
        sort_buf[i] += sort_buf[i - 1];                 // получаем концы участков для записи
        std::cout << sort_buf[i] << " ";
    }
    std::cout << "\n";

    for(int64_t i = end_pos - 1; i >= start_pos; --i) {
        char temp_char = buffer[indexes_array[i]][char_number];
        size_t temp_position = GetPosition(temp_char);
//        int64_t index_position =
//                --sort_buf[GetPosition(buffer[indexes_array[i]][char_number])];  //  позиция для записи
        int64_t index_position =
                --(sort_buf[temp_position]);
       // indexes_array[start_pos + index_position] = i;      //  смещаем на нужный участок
        temp_indexes[index_position] = indexes_array[i];
    }

    for (int i = 0; i < end_pos - start_pos; ++i) {
        std::cout << temp_indexes[i] << " ";
    } std::cout << "\n";

    for (int i = 0; i < end_pos - start_pos; ++i) {
        indexes_array[start_pos + i] = temp_indexes[i];
    }

    for(size_t i = 0; i < sort_buf_size; ++i) {
        std::cout << sort_buf[i] << " ";
    }
    std::cout << "\n";

    for(int64_t i = sort_buf_size - 1; i >= 3; --i) {
        if ((sort_buf[i] - sort_buf[i - 1]) > 1) {
            std::cout << sort_buf[i - 1] << sort_buf[i] << "\n";
        }
    }

    auto* new_sort_buf = new size_t[sort_buf_size];  //
    for(int64_t i = sort_buf_size - 1; i >= 2; --i) {
        if ((sort_buf[i] - sort_buf[i - 1]) > 1) {
            Sort(sort_buf[i - 1], sort_buf[i], char_number + 1, new_sort_buf);
        }
    }
    delete[] temp_indexes;
    delete[] new_sort_buf;
}

const std::string *MSE::GetSorted(const size_t& size) {
    InitIndexesArray();
    auto* sort_buf = new size_t[sort_buf_size];
    Sort(0, real_size, 0, sort_buf);
    delete[] sort_buf;
    return nullptr;
}

void MSE::InitIndexesArray() {
    for (size_t i = 0; i < real_size; ++i) {
        indexes_array[i] = i;
    }
}

int main() {
    MSE mse;
    mse.Add("aaaa");
    mse.Add("a");
    mse.Add("baaaaaa");
    mse.Add("aa");
    mse.Add("ea");
    mse.Add("ee");
    mse.Add("baaaaag");
    mse.Add("aa");

    //mse.a();

    std::cout << "\n";
    size_t size = 0;
    mse.GetSorted(size);
    mse.a();

    return 0;
}