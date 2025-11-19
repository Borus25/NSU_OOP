#pragma once

#include <string>
#include <map>

class CommandLineParser {
public:
    // Конструктор
    explicit CommandLineParser(int argc, char* argv[]);

    // Парсит аргументы командной строки и сохраняет в map
    void parse();

    // Возвращает map с распарсенными аргументами
    [[nodiscard]] std::map<std::string, std::string> getArguments() const;

    // Проверяет, существует ли ключ в аргументах
    [[nodiscard]] bool hasKey(const std::string& key) const;

    // Возвращает значение по ключу или значение по умолчанию
    [[nodiscard]] std::string getValue(const std::string& key, const std::string& defaultValue = "") const;

    // Выводит все аргументы (для отладки)
    void printArguments() const;

private:
    int argc;
    char** argv;
    std::map<std::string, std::string> arguments;
    bool parsed;

    // Вспомогательная функция для проверки, является ли строка ключом
    [[nodiscard]] bool isKey(const std::string& str) const;

    // Вспомогательная функция для очистки ключа от префиксов
    [[nodiscard]] std::string cleanKey(const std::string& key) const;
};

