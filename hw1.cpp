// Во всех задачах необходимо использование битовых операций.
// Использование арифметических операций запрещено.
// Входное число лежит в диапазоне 0..232 -1 и вводится в десятичном виде.
// 1.2 Вернуть значение бита в числе N по его номеру K.
// Формат входных данных. Число N, номер бита K

#include <iostream>
#include <cassert>

bool CheckBit(size_t number, size_t bit_position) {
    assert(bit_position < sizeof(number) * 8);
    return (number >> bit_position) & 1;
}

int main() {
    size_t number = 0;
    size_t bit_position = 0;
    std::cin >> number >> bit_position;
    std::cout << CheckBit(number, bit_position);
}