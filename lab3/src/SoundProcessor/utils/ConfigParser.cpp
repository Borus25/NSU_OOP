#include "ConfigParser.h"
#include "../../Exceptions.h"
#include <fstream>
#include <sstream>
#include <algorithm>

ConfigParser::ConfigParser() : currentLine_(0)
{
}

void ConfigParser::parse(const std::string& filename) {
    currentFile_ = filename;
    currentLine_ = 0;
    commands_.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ConfigParseException("Cannot open config file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        currentLine_++;

        line = trim(line);

        if (line.empty() || isComment(line)) {
            continue;
        }

        auto cmd = parseLine(line);
        if (cmd) {
            commands_.push_back(*cmd);
        }
    }

    file.close();
    validateSyntax();
}

const std::vector<ConverterCommand>& ConfigParser::getCommands() const {
    return commands_;
}

void ConfigParser::validateSyntax() const {
    // Базовая валидация - проверяем, что все команды имеют имена
    for (const auto& cmd : commands_) {
        if (cmd.getName().empty()) {
            throw ConfigParseException("Empty converter name in configuration");
        }
    }
}

size_t ConfigParser::getCommandCount() const {
    return commands_.size();
}

void ConfigParser::clear() {
    commands_.clear();
    currentLine_ = 0;
    currentFile_.clear();
}

std::unique_ptr<ConverterCommand> ConfigParser::parseLine(const std::string& line) {
    std::vector<std::string> tokens = tokenize(line);

    if (tokens.empty()) {
        return nullptr;
    }

    std::string converterName = tokens[0];
    std::vector<std::string> params(tokens.begin() + 1, tokens.end());

    return std::make_unique<ConverterCommand>(converterName, params);
}

bool ConfigParser::isComment(const std::string& line) const {
    return !line.empty() && line[0] == '#';
}

std::string ConfigParser::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

std::vector<std::string> ConfigParser::tokenize(const std::string& line) const {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

bool ConfigParser::isValidParameter(const std::string& param) const {
    return ConverterCommand::isInteger(param) ||
           ConverterCommand::isFloat(param) ||
           ConverterCommand::isStreamReference(param);
}

std::string ConfigParser::formatError(const std::string& message) const {
    return currentFile_ + ":" + std::to_string(currentLine_) + ": " + message;
}

