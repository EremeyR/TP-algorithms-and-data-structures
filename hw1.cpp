/*
 * Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в
 * случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и
 * проверки принадлежности данной строки множеству.
 * 1_2. Для разрешения коллизий используйте двойное хеширование.
 */

#include<iostream>
#include<vector>
#include<string>
#include <sstream>
#include <fstream>
#include<cassert>

using std::vector;
using std::string;

struct StringHasher1 {
    unsigned int operator()( const string& str ) const {
        unsigned int hash = 0;

        for(auto& chr: str)
            hash = hash * 17 + chr;
        return hash;
    }
};

struct StringHasher2 {
    unsigned int operator()( const string& str ) const {
        unsigned int hash = 0;

        for(auto& chr: str)
            hash = hash * 7 + chr;
        return hash;
    }
};

template<class T, class H1, class H2>
class HashTable {
public:
    HashTable(const H1& hasher1,  const H2& hasher2);
    HashTable( const HashTable& table ) = delete;
    HashTable& operator=( const HashTable& table ) = delete;
    ~HashTable() = default;

    // Если Del - идём дальше
    // Если Data - Data == data ? return true : идём дальше
    // Если Empty - return false
    bool Has( const T& key ) const;
    // Если Del - запоминаем позицию если это первый Del, идём дальше
    // Если Data - Data == data return false : идём дальше
    // Если Empty - вставляем в запомненную Del позицию, если была, или в текущую в ином случае
    bool Add( const T& key );
    // Если Del - идём дальше
    // Если Data - Data == data ? помечаем Del и return true : идём дальше
    // Если Empty - return false
    bool Delete( const T& key );

private:
    struct HashTableCell {
        T data;
        size_t hash1;
        size_t hash2;
        char state; //  E - empty, D - del, T - taken

        HashTableCell() : hash1(0), hash2(0), state('E') {}
    };

    H1 hasher1;
    H2 hasher2;
    vector<HashTableCell> table;
    size_t keys_count;
    size_t del_count;

    void growTable();
    size_t GetDoubleHash(size_t hash1, size_t hash2, size_t iterator);
};

template<class T, class H1, class H2>
HashTable<T, H1, H2>::HashTable(const H1& hasher1,  const H2& hasher2) :
        hasher1(hasher1),
        hasher2(hasher2),
        table(8),
        keys_count(0),
        del_count(0)
{}

template<class T, class H1, class H2>
bool  HashTable<T, H1, H2>::Has( const T& data ) const {
    size_t absHash1 = hasher1(data);
    size_t hash1 = absHash1 % table.size();

    if (table[hash1].state == 'E') {
        return false;
    } else if (table[hash1].state == 'T' && table[hash1].data == data) {
        return true;
    } else {
        size_t absHash2 = hasher2(data);
        size_t hash2 = absHash2 % table.size() * 2 + 1;

        size_t i = 1;
        while (i < table.size()) {
            if (table[(hash1 + i * hash2) % table.size()].state == 'E') {
                return false;
            } else if (table[(hash1 + i * hash2) % table.size()].state == 'T'
                       && table[(hash1 + i * hash2) % table.size()].data == data) {
                return true;
            }
            ++i;
        }
    }
    return false;
}

template<class T, class H1, class H2>
bool  HashTable<T, H1, H2>::Add( const T& data )
{
    if(((keys_count + del_count) * 4) > (3 * table.size())) {
        growTable();
    }

    size_t first_del_cell = 0;
    bool is_write_to_del = false;

    size_t absHash1 = hasher1(data);
    size_t hash1 = absHash1 % table.size();

    if (table[hash1].state == 'E') {
        table[hash1].state = 'T';
        table[hash1].data = data;
        table[hash1].hash1 = absHash1;
        table[first_del_cell].hash2 = hasher2(data);

    } else if (table[hash1].state == 'T' && table[hash1].data == data) {
            return false;
    } else {
        if (table[hash1].state == 'D') {
            first_del_cell = hash1;
            is_write_to_del = true;
        }

        size_t absHash2 = hasher2(data);
        size_t hash2 = absHash2 % table.size() * 2 + 1;

        size_t i = 1;
        while (table[(hash1 + i * hash2) % table.size()].state != 'E' && i < table.size()) {
            if (table[(hash1 + i * hash2) % table.size()].state == 'D' &&
                !is_write_to_del) {
                first_del_cell = (hash1 + i * hash2)  % table.size();
                is_write_to_del = true;
            } else if (table[(hash1 + i * hash2) % table.size()].state == 'T' &&
                       table[(hash1 + i * hash2) % table.size()].data == data) {
                return false;
            }

            ++i;
        }

        if (is_write_to_del) {
            table[first_del_cell].state = 'T';
            table[first_del_cell].data = data;
            table[first_del_cell].hash1 = absHash1;
            table[first_del_cell].hash2 = absHash2;
        } else {
            table[(hash1 + i * hash2) % table.size()].state = 'T';
            table[(hash1 + i * hash2) % table.size()].data = data;
            table[(hash1 + i * hash2) % table.size()].hash1 = absHash1;
            table[(hash1 + i * hash2) % table.size()].hash2 = absHash2;
        }
    }
    ++keys_count;
    return true;
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Delete( const T& data )
{
    size_t absHash1 = hasher1(data);
    size_t hash1 = absHash1 % table.size();

    if (table[hash1].state == 'E') {
        return false;
    } else if (table[hash1].state == 'T' && table[hash1].data == data) {
        table[hash1].state = 'D';
        --keys_count;
        ++del_count;
        return true;
    } else {
        size_t absHash2 = hasher2(data);
        size_t hash2 = absHash2 % table.size() * 2 + 1;

        size_t i = 1;
        while (i < table.size()) {
            if (table[(hash1 + i * hash2) % table.size()].state == 'E' && i < table.size()) {
                return false;
            } else if (table[(hash1 + i * hash2) % table.size()].state == 'T'
                       && table[(hash1 + i * hash2) % table.size()].data == data) {
                table[(hash1 + i * hash2) % table.size()].state = 'D';
                --keys_count;
                ++del_count;
                return true;
            }
            ++i;
        }
    }
}

template<class T, class H1, class H2>
void HashTable<T, H1, H2>::growTable() {
    vector<HashTableCell> new_table(table.size() * 2);
    for (auto &cell: table) {
        if (cell.state == 'T'){
            size_t new_hash1 = cell.hash1 % new_table.size();
            if (new_table[new_hash1].state == 'E') {
                new_table [new_hash1] = std::move(cell);
            } else {
                if (cell.hash2 == 0) {
                    cell.hash2 = hasher2(cell.data);
                }
                size_t new_hash2 = cell.hash2 % new_table.size() * 2 + 1;

                size_t i = 1;
                while (new_table[(new_hash1 + i * new_hash2) % new_table.size()].state != 'E') {
                    ++i;
                }
                new_table [(new_hash1 + i * new_hash2) % new_table.size()] = cell;
            }
        }
    }
    table = new_table;
    del_count = 0;
}

template<class T, class H1, class H2>
size_t HashTable<T, H1, H2>::GetDoubleHash(size_t hash1, size_t hash2, size_t iterator) {
    return (hash1 + iterator * (hash2 * 2 + 1)) % table.size();
}

int run(std::istream& input, std::ostream& output) {
    StringHasher1 hasher1;
    StringHasher2  hasher2;

    HashTable<string, StringHasher1, StringHasher2> table(hasher1, hasher2);
    char operation = 0;
    string data;
    while( input >> operation >> data ) {
        switch( operation ) {
            case '+':
                output << (table.Add( data ) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                output << (table.Delete( data ) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                output << (table.Has( data ) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}

void Tests() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "+ 3";
        run(input, output);
        assert(output.str() == "OK\n");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "+3 +3";
        run(input, output);
        assert(output.str() == "OK\nFAIL\n");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 +2 +3 +4 +5 +6 +7 +8 + 9 +10";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 +2 +3 +4 +5 +7 +8 + 9 +10 -6";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\n");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 +2 +3 +4 +5 +7 +8 + 9 +10 -6 +6";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\n");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 +2 +3 +4 +5 +7 +8 + 9 +10 -6 +6 ?6 -6 ?6";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nOK\nOK\nFAIL\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 +2 +3 +4 +5 +7 +8 + 9 +10 -6 +6 ?6 -6 ?6 ?5 ?4 ?3 ?2 ?1";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nOK\nOK\nFAIL\nOK\nOK\nOK\nOK\nOK\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 +2 +3 +4 +5 +7 +8 + 9 +10 -6 +6 ?6 -6 ?6 ?6 ?6 ?6 ?6 ?6";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nOK\nOK\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 ";
        run(input, output);
        assert(output.str() == "OK\n");
    }
    {
        std::ifstream input("../006");
        run(input, std::cout);

    }
}

int main() {
    //Tests();
    run(std::cin, std::cout);
    return 0;
}