#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include "CSVParser/Parser.h"

// Принтер кортежей
namespace tuplePrinter {

    template <typename Tuple, std::size_t Index, std::size_t Size>
    struct TuplePrinter {
        template <typename Ch, typename Tr>
        static void print(std::basic_ostream<Ch, Tr>& os, const Tuple& t) {
            if (Index > 0) {
                os << ", ";
            }
            os << std::get<Index>(t);
            TuplePrinter<Tuple, Index + 1, Size>::print(os, t);
        }
    };

    template <typename Tuple, std::size_t Size>
    struct TuplePrinter<Tuple, Size, Size> {
        template <typename Ch, typename Tr>
        static void print(std::basic_ostream<Ch, Tr>& os, const Tuple& t) {
            // Пустое тело функции прерывает цепочку рекурсивных вызовов
        }
    };
}

template<typename Ch, typename Tr, typename... Args>
auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) -> std::basic_ostream<Ch, Tr>& {
    os << "(";
    tuplePrinter::TuplePrinter<decltype(t), 0, sizeof...(Args)>::print(os, t);
    os << ")";
    return os;
}

int main() {
    std::ifstream file("test_advanced.csv");
    try {
        CSVParser<int, std::string, std::string, int> parser(file, 1, ';', '"');
        for (const auto& row : parser) {
            std::cout << row << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}