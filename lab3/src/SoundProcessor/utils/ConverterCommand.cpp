#include "ConverterCommand.h"
#include <stdexcept>

ConverterCommand::ConverterCommand(const std::string& name,
                                   const std::vector<std::string>& parameters)
    : name_(name), parameters_(parameters)
{
}

std::string ConverterCommand::getName() const {
    return name_;
}

const std::vector<std::string>& ConverterCommand::getParameters() const {
    return parameters_;
}

size_t ConverterCommand::getParameterCount() const {
    return parameters_.size();
}

std::string ConverterCommand::getParameter(size_t index) const {
    if (index >= parameters_.size()) {
        throw std::out_of_range("Parameter index out of range");
    }
    return parameters_[index];
}

bool ConverterCommand::isStreamReference(const std::string& param) {
    if (param.empty() || param[0] != '$') {
        return false;
    }

    try {
        std::stoi(param.substr(1));
        return true;
    } catch (...) {
        return false;
    }
}

int ConverterCommand::getStreamNumber(const std::string& param) {
    if (!isStreamReference(param)) {
        throw std::invalid_argument("Not a stream reference: " + param);
    }
    return std::stoi(param.substr(1));
}

bool ConverterCommand::isInteger(const std::string& param) {
    if (param.empty()) {
        return false;
    }

    try {
        std::stoi(param);
        return true;
    } catch (...) {
        return false;
    }
}

bool ConverterCommand::isFloat(const std::string& param) {
    if (param.empty()) {
        return false;
    }

    try {
        std::stof(param);
        return true;
    } catch (...) {
        return false;
    }
}

