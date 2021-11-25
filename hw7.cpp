#include <iostream>
#include <cassert>
#include <sstream>

class MSE {
public:
    MSE() = default;
    MSE(const MSE &) = delete;
    ~MSE();

    MSE &operator=(const MSE &) = delete;

    void Add(const std::string& str);
    const std::string* GetSorted(size_t& size);


private:
    size_t sort_buf_size = 27; // 26 букв + '\0'

    std::string* buffer = nullptr;  //  буфер для хранения слов
    size_t* indexes_array = nullptr;  // буфер для хранения индексов отсортированных слов
    std::string* result = nullptr;  //  массив отсортированных слов

    size_t buffers_size = 0;
    size_t real_size = 0;

    size_t GetPosition(char value) const;

    void Sort(const int64_t start_pos, const int64_t end_pos, const size_t char_number, int64_t* sort_buf);
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
    delete[] result;
}

size_t MSE::GetPosition(char value) const {
    if (value =='\0'){
        return 0;
    }

    assert(static_cast<size_t>(value) >= static_cast<size_t>('a'));
    assert(static_cast<size_t>(value) <= static_cast<size_t>('z'));

    return (static_cast<size_t>(value) + 1) - static_cast<size_t>('a');  //  возвращаем позицию буквы в алфавите
}

void MSE::Sort(const int64_t start_pos, const int64_t end_pos, const size_t char_number, int64_t* sort_buf) {

    auto* temp_indexes = new int64_t[end_pos - start_pos];

    for(size_t i = 0; i < sort_buf_size; ++i ) {
        sort_buf[i] = 0;
    }

    for(size_t i = start_pos; i < end_pos; ++i) {
        char temp_char = buffer[indexes_array[i]][char_number];
        size_t temp_position = GetPosition(temp_char);
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

    for (int64_t i = end_pos - 1; i >= start_pos; --i) {
        char temp_char = buffer[indexes_array[i]][char_number];
        size_t temp_position = GetPosition(temp_char);
        int64_t index_position = --(sort_buf[temp_position]);
        temp_indexes[index_position] = indexes_array[i];   //////////////////////////////////// большая ошибка
    }
    //////////////
    std::cout << sort_buf[0] << " ";
    for(size_t i = 1; i < sort_buf_size; ++i) {
        std::cout << sort_buf[i] << " ";
    }
    std::cout << "\n\n";
    auto a = temp_indexes[0];
    auto b = temp_indexes[1];
    /////////

    for (int i = 0; i < end_pos - start_pos; ++i) {
        indexes_array[start_pos + i] = temp_indexes[i];
    }

    auto* new_sort_buf = new int64_t[sort_buf_size];  //
    for (int64_t i = sort_buf_size - 1; i >= 2; --i) {
        if ((sort_buf[i] - sort_buf[i - 1]) > 1) {
            Sort(start_pos + sort_buf[i - 1], start_pos + sort_buf[i], char_number + 1, new_sort_buf);
        }
    }
    delete[] temp_indexes;
    delete[] new_sort_buf;
}

const std::string *MSE::GetSorted(size_t& size) {
    ///
    std::cout << 0 << " ";
    for(size_t i = 0; i < 26; ++i) {  // получаем концы участков для записи
        std::cout << static_cast<char>(static_cast<size_t>('a') + i)  << " ";
    }std::cout << "\n";
    ///
    result = new std::string[real_size];
    InitIndexesArray(); // для неотсортированного массива "расставим" слова в порядке вхождения
    auto* sort_buf = new int64_t[sort_buf_size];  // буфер для записи количества вхождений слов
    Sort(0, real_size, 0, sort_buf);

    delete[] sort_buf;

    for (int i = 0; i < real_size; ++i) {
        result[i] = buffer[indexes_array[i]];
    }

    size = real_size;
    return result;
}

void MSE::InitIndexesArray() {
    for (size_t i = 0; i < real_size; ++i) {
        indexes_array[i] = i;
    }
}

int run(std::istream& input, std::ostream& output) {

    MSE mse;
    std::string word;

    while (input >> word) {
        mse.Add(word);
    }

    std::cout << "\n";
    size_t size = 0;
    const std::string* result = mse.GetSorted(size);

    for (int i = 0; i < size; ++i) {
        std::cout << result[i] << std::endl;
    }
    return 0;
}

void Tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "errrtf\n"
              << "qwsdf\n"
              << "errrrr\n"
                << "zzzzz\n"
                << "zzzz\n"
                << "errrrr\n"
              << "all";
        run(input, output);
        //assert(output.str() == "3");
    }
}

int main() {
    Tests();
    //run(std::cin, std::cout);
    return 0;
}