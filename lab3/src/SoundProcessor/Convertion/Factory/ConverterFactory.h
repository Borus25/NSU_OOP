#pragma once

#include "../BasicConverter/Converter.h"
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <vector>

class ConverterFactory {
public:
    static ConverterFactory& getInstance();

    void registerConverter(const std::string& name,
                          std::function<std::shared_ptr<Converter>()> creator);

    std::shared_ptr<Converter> createConverter(const std::string& name);

    bool hasConverter(const std::string& name) const;

    std::vector<std::string> getConverterNames() const;


    std::string getConverterDescription(const std::string& name);

    std::string getConverterSyntax(const std::string& name);

private:
    ConverterFactory() = default;
    ~ConverterFactory() = default;

    ConverterFactory(const ConverterFactory&) = delete;
    ConverterFactory(ConverterFactory&&) = delete;
    ConverterFactory& operator=(const ConverterFactory&) = delete;
    ConverterFactory& operator=(ConverterFactory&&) = delete;

    using ConverterCreator = std::function<std::shared_ptr<Converter>()>;
    std::map<std::string, ConverterCreator> converters_;
};

