#pragma once

#include <exception>
#include <string>
#include <iostream>

class SPException : public std::exception {
public:
    explicit SPException(const std::string& message) : message_(message) {}
    virtual ~SPException() = default;

    const char* what() const noexcept override {
        return message_.c_str();
    }

    virtual int getErrorCode() const {
        return 1;
    }

protected:
    std::string message_;
};

class FileException : public SPException {
public:
    explicit FileException(const std::string& message)
        : SPException("File Error: " + message) {}

    int getErrorCode() const override { return 2; }
};

class InvalidFormatException : public SPException {
public:
    explicit InvalidFormatException(const std::string& message)
        : SPException("Format Error: " + message) {}

    int getErrorCode() const override { return 3; }
};

class InvalidArgumentException : public SPException {
public:
    explicit InvalidArgumentException(const std::string& message)
        : SPException("Argument Error: " + message) {}

    int getErrorCode() const override { return 4; }
};

class ConfigParseException : public SPException {
public:
    explicit ConfigParseException(const std::string& message)
        : SPException("Config Parse Error: " + message) {}

    int getErrorCode() const override { return 5; }
};

class ConverterException : public SPException {
public:
    explicit ConverterException(const std::string& message)
        : SPException("Converter Error: " + message) {}

    int getErrorCode() const override { return 6; }
};

class AudioStreamException : public SPException {
public:
    explicit AudioStreamException(const std::string& message)
        : SPException("Audio Stream Error: " + message) {}

    int getErrorCode() const override { return 7; }
};

