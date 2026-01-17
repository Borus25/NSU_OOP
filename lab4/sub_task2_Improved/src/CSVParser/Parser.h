
#pragma once

#include <iostream>
#include "CSVIterator.h" // Подключаем итератор

template<typename... Args>
class CSVParser {
public:
    // Чтобы пользователю было удобно писать CSVParser<...>::iterator
    using iterator = CSVIterator<Args...>;

    CSVParser(std::istream& file, size_t skip_count = 0)
        : m_file(file), m_skip_count(skip_count) {}

    iterator begin() {
        return iterator(m_file, m_skip_count);
    }

    iterator end() {
        return iterator();
    }

private:
    std::istream& m_file;
    size_t m_skip_count;
};
