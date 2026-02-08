#pragma once

#include <string>
#include <vector>

class ConverterCommand {
public:
    ConverterCommand(const std::string& name,
                     const std::vector<std::string>& parameters);

    ConverterCommand() = default;

    ~ConverterCommand() = default;

    std::string getName() const;

    const std::vector<std::string>& getParameters() const;

    size_t getParameterCount() const;

    std::string getParameter(size_t index) const;

    static bool isStreamReference(const std::string& param);

    static int getStreamNumber(const std::string& param);

    static bool isInteger(const std::string& param);

    static bool isFloat(const std::string& param);

private:
    std::string name_;
    std::vector<std::string> parameters_;
};

