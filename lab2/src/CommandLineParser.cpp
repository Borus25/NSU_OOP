#include <iostream>

#include "CommandLineParser.h"

bool CommandLineParser::isKey(const std::string& str) const {
    return str.find("--") == 0 || str.find('-') == 0;
}

std::string CommandLineParser::cleanKey(const std::string& key) const {
    if (key.find("--") == 0) {
        return key.substr(2);
    } else if (key.find('-') == 0) {
        return key.substr(1);
    }
    return key;
}

CommandLineParser::CommandLineParser(int argc, char* argv[])
        : argc(argc), argv(argv), parsed(false) {}

void CommandLineParser::parse() {
    arguments.clear();

    for (int i = 1; i < argc; ++i) {
        std::string current = argv[i];

        if (isKey(current)) {
            std::string key = cleanKey(current);
            std::string value;

            size_t equalsPos = current.find('=');
            if (equalsPos != std::string::npos) {
                key = current.substr(0, equalsPos);
                key = cleanKey(key);
                value = current.substr(equalsPos + 1);
            }
            else if (i + 1 < argc && !isKey(argv[i + 1])) {
                value = argv[i + 1];
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

    parsed = true;
}

std::map<std::string, std::string> CommandLineParser::getArguments() const {
    if (!parsed) {
        throw std::runtime_error("Arguments not parsed yet. Call parse() first.");
    }
    return arguments;
}

bool CommandLineParser::hasKey(const std::string& key) const {
    auto args = getArguments();
    return args.find(key) != args.end();
}

std::string CommandLineParser::getValue(const std::string& key, const std::string& defaultValue) const {
    auto args = getArguments();
    auto it = args.find(key);
    if (it != args.end()) {
        return it->second;
    }
    return defaultValue;
}

void CommandLineParser::printArguments() const {
    auto args = getArguments();
    std::cout << "Parsed arguments:" << std::endl;
    for (const auto& pair : args) {
        std::cout << "  " << pair.first << " = " << pair.second << std::endl;
    }
}