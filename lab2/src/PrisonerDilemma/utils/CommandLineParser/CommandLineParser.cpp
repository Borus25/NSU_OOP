#include <iostream>

#include "CommandLineParser.h"

bool CommandLineParser::isKey(const std::string& str) const {
    return str.find("--") == 0 || str.find('-') == 0;
}

std::string CommandLineParser::cleanKey(const std::string& key) const {
    if (key.find("--") == 0) {
        return key.substr(2);
    }
    if (key.find('-') == 0) {
        return key.substr(1);
    }
    return key;
}

CommandLineParser::CommandLineParser(int argc, char* argv[])
        : argc_(argc), argv_(argv), parsed(false) {}

std::map<std::string, std::string> CommandLineParser::parse() {
    arguments.clear();

    for (int i = 1; i < argc_; ++i) {
        std::string current = argv_[i];

        if (isKey(current)) {
            std::string key = cleanKey(current);
            std::string value;

            size_t equalsPos = current.find('=');
            if (equalsPos != std::string::npos) {
                key = current.substr(0, equalsPos);
                key = cleanKey(key);
                value = current.substr(equalsPos + 1);

                // Случай 8: ключ с пустым значением
                if (value.empty()) {
                    value = "true";  // Используем значение по умолчанию
                }
            }
            else if (i + 1 < argc_ && !isKey(argv_[i + 1])) {
                value = argv_[i + 1];
                i++;
            }
            else {
                value = "true";
            }

            arguments[key] = value;
        }
        else {
            std::string key = "arg" + std::to_string(i);
            arguments[key] = current;
        }
    }

    return arguments;
}