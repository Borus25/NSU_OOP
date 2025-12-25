#pragma once

#include "ConverterCommand.h"
#include <string>
#include <vector>
#include <memory>

/**
 * Класс для разбора конфигурационного файла
 */
class ConfigParser {
public:
    ConfigParser();
    ~ConfigParser() = default;

    /**
     * Разбор конфигурационного файла
     * @param filename - путь к файлу конфигурации
     */
    void parse(const std::string& filename);

    /**
     * Получение списка команд конвертеров
     * @return вектор команд в порядке, заданном в файле
     */
    const std::vector<ConverterCommand>& getCommands() const;

    /**
     * Проверка синтаксиса конфигурационного файла
     */
    void validateSyntax() const;

    /**
     * Получение количества команд в конфигурации
     */
    size_t getCommandCount() const;

    /**
     * Очистка списка команд
     */
    void clear();

private:
    std::vector<ConverterCommand> commands_;
    std::string currentFile_;
    int currentLine_;

    std::unique_ptr<ConverterCommand> parseLine(const std::string& line);
    bool isComment(const std::string& line) const;
    std::string trim(const std::string& str) const;
    std::vector<std::string> tokenize(const std::string& line) const;
    bool isValidParameter(const std::string& param) const;
    std::string formatError(const std::string& message) const;
};

