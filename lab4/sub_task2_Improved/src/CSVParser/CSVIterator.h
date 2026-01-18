#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include "TupleDetails.h"

template<typename... Args>
class CSVIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::tuple<Args...>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const std::tuple<Args...>*;
    using reference         = const std::tuple<Args...>&;

    CSVIterator() : stream_ptr(nullptr), delimiter(','), quote('"') {}

    CSVIterator(std::istream& stream, size_t skip_lines, char delim = ',', char q = '"')
        : stream_ptr(&stream), current_line_idx(0), delimiter(delim), quote(q)
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
    char delimiter;
    char quote;

    void readNext() {
        while (stream_ptr && std::getline(*stream_ptr, current_line_str)) {
            current_line_idx++;
            if (current_line_str.empty()) continue;

            auto cells = detail::CSVLineSplitter::split(current_line_str, delimiter, quote);

            if (cells.size() != sizeof...(Args)) {
                 throw std::runtime_error("Column count mismatch at line " + std::to_string(current_line_idx));
            }

            try {
                detail::TupleFiller<std::tuple<Args...>, 0>::fill(current_row, cells, current_line_idx);
                return;
            } catch (...) {
                throw;
            }
        }
        stream_ptr = nullptr;
    }
};