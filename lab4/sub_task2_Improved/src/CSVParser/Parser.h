#pragma once
#include "CSVIterator.h"

template<typename... Args>
class CSVParser {
public:
    using iterator = CSVIterator<Args...>;

    // --- (2) Конфигурация через конструктор ---
    CSVParser(std::istream& file, size_t skip_count = 0, char delimiter = ',', char quote = '"')
        : m_file(file), m_skip_count(skip_count), m_delimiter(delimiter), m_quote(quote) {}

    iterator begin() {
        return iterator(m_file, m_skip_count, m_delimiter, m_quote);
    }

    iterator end() {
        return iterator();
    }

private:
    std::istream& m_file;
    size_t m_skip_count;
    char m_delimiter;
    char m_quote;
};