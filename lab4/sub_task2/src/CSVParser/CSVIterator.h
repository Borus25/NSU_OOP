#pragma once

#include <iostream>
#include <fstream>
#include <tuple>
#include <stdexcept>
#include "TupleDetails.h" // Подключаем логику парсинга

// Шаблон итератора теперь самостоятельный
template<typename... Args>
class CSVIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::tuple<Args...>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const std::tuple<Args...>*;
    using reference         = const std::tuple<Args...>&;

    // Конструктор конца (sentinel)
    CSVIterator() : stream_ptr(nullptr), current_line_idx(0) {}

    // Конструктор начала
    CSVIterator(std::istream& stream, size_t skip_lines)
        : stream_ptr(&stream), current_line_idx(0)
    {
        std::string dummy;
        for (size_t i = 0; i < skip_lines; ++i) {
            if (!std::getline(*stream_ptr, dummy)) {
                stream_ptr = nullptr;
                return;
            }
            current_line_idx++;
        }
        readNext();
    }

    reference operator*() const {
        return current_row;
    }

    pointer operator->() const {
        return &current_row;
    }

    CSVIterator& operator++() {
        readNext();
        return *this;
    }

    bool operator!=(const CSVIterator& other) const {
        return stream_ptr != other.stream_ptr;
    }

    bool operator==(const CSVIterator& other) const {
        return !(*this != other);
    }

private:
    std::istream* stream_ptr;
    std::tuple<Args...> current_row;
    size_t current_line_idx;
    std::string current_line_str;

    void readNext() {
        while (stream_ptr && std::getline(*stream_ptr, current_line_str)) {
            current_line_idx++;
            if (current_line_str.empty()) continue;

            std::stringstream ss(current_line_str);
            try {
                // Вызываем код из TupleDetails.h
                tupleFiller::TupleFiller<std::tuple<Args...>, 0>::fill(current_row, ss);
                return;
            } catch (...) {
                 throw std::runtime_error("CSV Error line " + std::to_string(current_line_idx));
            }
        }
        stream_ptr = nullptr;
    }
};
