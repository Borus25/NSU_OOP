#pragma once

#include "ConverterCommand.h"
#include <string>
#include <vector>
#include <memory>

class ConfigParser {
public:
    ConfigParser();
    ~ConfigParser() = default;

    void parse(const std::string& filename);

    const std::vector<ConverterCommand>& getCommands() const;

    void validateSyntax() const;

    size_t getCommandCount() const;

    void clear();

private:
    std::vector<ConverterCommand> commands_;
    std::string currentFile_;
    int currentLine_;

    ConverterCommand parseLine(const std::string& line);
    bool isComment(const std::string& line) const;
    std::string trim(const std::string& str) const;
    std::vector<std::string> tokenize(const std::string& line) const;
    bool isValidParameter(const std::string& param) const;
    std::string formatError(const std::string& message) const;
};

