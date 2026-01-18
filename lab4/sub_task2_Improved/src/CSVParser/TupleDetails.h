#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <stdexcept>

namespace detail {

    // --- (1) Логика экранирования ---
    class CSVLineSplitter {
    public:
        static std::vector<std::string> split(const std::string& line, char delimiter, char quote) {
            std::vector<std::string> cells;
            std::string currentCell;
            bool insideQuotes = false;

            for (size_t i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (c == quote) {
                    // Если это двойная кавычка ("") внутри кавычек - это экранирование
                    if (insideQuotes && i + 1 < line.size() && line[i + 1] == quote) {
                        currentCell += quote;
                        ++i; // Пропускаем вторую
                    } else {
                        insideQuotes = !insideQuotes;
                    }
                } else if (c == delimiter && !insideQuotes) {
                    cells.push_back(currentCell);
                    currentCell.clear();
                } else {
                    currentCell += c;
                }
            }
            cells.push_back(currentCell);
            return cells;
        }
    };

    template<typename T>
    T stringToType(const std::string& s, size_t row, size_t col) {
        if (s.empty()) return T{};
        std::stringstream ss(s);
        T value;
        ss >> value;
        if (ss.fail() || !ss.eof()) {
             throw std::runtime_error("Type error at line " + std::to_string(row) +
                                      ", col " + std::to_string(col) + ": '" + s + "' is invalid");
        }
        return value;
    }

    template<>
    inline std::string stringToType<std::string>(const std::string& s, size_t, size_t) { return s; }

    template<typename Tuple, std::size_t Index>
    struct TupleFiller {
        static void fill(Tuple& t, const std::vector<std::string>& cells, size_t rowIdx) {
            if (Index >= cells.size()) {
                throw std::runtime_error("Format error at line " + std::to_string(rowIdx) +
                                         ": expected " + std::to_string(std::tuple_size<Tuple>::value) + " columns");
            }
            using Type = typename std::tuple_element<Index, Tuple>::type;
            std::get<Index>(t) = stringToType<Type>(cells[Index], rowIdx, Index + 1);
            TupleFiller<Tuple, Index + 1>::fill(t, cells, rowIdx);
        }
    };

    template<typename Tuple>
    struct TupleFiller<Tuple, std::tuple_size<Tuple>::value> {
        static void fill(Tuple&, const std::vector<std::string>&, size_t) {}
    };
}