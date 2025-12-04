#pragma once

#include <string>
#include <map>

class CommandLineParser {
public:
    // Конструктор
    explicit CommandLineParser(int argc, char* argv[]);

    // Парсит аргументы командной строки и сохраняет в map
    std::map<std::string, std::string> parse();

private:
    int argc_;
    char** argv_;
    std::map<std::string, std::string> arguments;
    bool parsed;

    // Вспомогательная функция для проверки, является ли строка ключом
    [[nodiscard]] bool isKey(const std::string& str) const;

    // Вспомогательная функция для очистки ключа от префиксов
    [[nodiscard]] std::string cleanKey(const std::string& key) const;
};

