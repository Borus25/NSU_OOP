#pragma once

#include <sstream>
#include <tuple>
#include <string>

namespace tupleFiller {

    // --- (A) Функция для чтения одной ячейки ---
    template<typename T>
    T parseCell(std::stringstream& ss) {
        std::string cell;
        if (!std::getline(ss, cell, ',')) return T{};
        std::stringstream converter(cell);
        T value;
        converter >> value;
        return value;
    }

    // Специализация для string
    template<>
    inline std::string parseCell<std::string>(std::stringstream& ss) {
        std::string cell;
        std::getline(ss, cell, ',');
        return cell;
    }

    // --- (B) Рекурсивный заполнитель кортежа ---
    template<typename Tuple, std::size_t Index>
    struct TupleFiller {
        static void fill(Tuple& t, std::stringstream& ss) {
            // Проверяем условие прямо здесь
            if constexpr (Index < std::tuple_size<Tuple>::value) {

                using ElementType = typename std::tuple_element<Index, Tuple>::type;
                std::get<Index>(t) = parseCell<ElementType>(ss);

                TupleFiller<Tuple, Index + 1>::fill(t, ss);
            }
        }
    };
}

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
