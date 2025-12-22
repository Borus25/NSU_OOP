#include <iostream>
#include "bit_array.h"
#include "tests.h"

int main() {
    TestAll();

    // Создаем битовый массив из 16 бит, инициализированный числом 0xAAAA (1010101010101010)
    BitArray bits(16, 0xAAAA);
    std::cout << "Initial bits: " << bits.to_string() << std::endl;

    // Установим 0-й и 15-й бит в 1
    bits.set(0);
    bits.set(15);
    std::cout << "After set bits 0 and 15: " << bits.to_string() << std::endl;

    // Сбросим 1-й бит
    bits.reset(1);
    std::cout << "After reset bit 1: " << bits.to_string() << std::endl;

    // Проверим отдельные биты
    std::cout << "Bit at index 0: " << bits[0] << std::endl;
    std::cout << "Bit at index 1: " << bits[1] << std::endl;

    // Подсчитаем количество установленных в 1 битов
    std::cout << "Count of set bits: " << bits.count() << std::endl;

    // Инвертируем биты
    BitArray inverted = ~bits;
    std::cout << "Inverted bits: " << inverted.to_string() << std::endl;

    // Логические операции с другим BitArray
    BitArray other(16, 0x0F0F);
    std::cout << "Other bits: " << other.to_string() << std::endl;

    BitArray and_result = bits & other;
    std::cout << "AND result: " << and_result.to_string() << std::endl;

    BitArray or_result = bits | other;
    std::cout << "OR result: " << or_result.to_string() << std::endl;

    BitArray xor_result = bits ^ other;
    std::cout << "XOR result: " << xor_result.to_string() << std::endl;

    // Сдвиг бит влево и вправо
    BitArray shifted_left = bits << 2;
    std::cout << "Shifted left by 2: " << shifted_left.to_string() << std::endl;

    BitArray shifted_right = bits >> 3;
    std::cout << "Shifted right by 3: " << shifted_right.to_string() << std::endl;

    // Проверка, пуст ли массив и содержит ли хотя бы один установленный бит
    std::cout << "Any bit set? " << (bits.any() ? "Yes" : "No") << std::endl;
    std::cout << "None bits set? " << (bits.none() ? "Yes" : "No") << std::endl;

    // Изменим размер массива и добавим новые биты
    bits.resize(20, false);
    std::cout << "Resized bits (20): " << bits.to_string() << std::endl;

    bits.push_back(true);
    bits.push_back(false);
    std::cout << "After push_back bits: " << bits.to_string() << std::endl;

    // Очистка массива
    bits.clear();
    std::cout << "After clear: size = " << bits.size() << ", empty = " << bits.empty() << std::endl;

    return 0;
}
