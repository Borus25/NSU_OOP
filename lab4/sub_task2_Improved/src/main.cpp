#include <iostream>
#include <fstream>
#include "CSVParser/Parser.h" // <-- Достаточно только этого

int main() {
    std::ifstream file("test.csv");
    // ...
    CSVParser<int, std::string> parser(file);
    for (auto& i : parser) {
        std::cout << i << std::endl;
    }
}
