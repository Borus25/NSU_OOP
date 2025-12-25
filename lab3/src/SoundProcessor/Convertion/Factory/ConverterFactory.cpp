#include "ConverterFactory.h"
#include "../MuteConverter/MuteConverter.h"
#include "../MixConverter/MixConverter.h"
#include "../VolumeConverter/VolumeConverter.h"
#include "../EchoConverter/EchoConverter.h"
#include "../../../Exceptions.h"

ConverterFactory& ConverterFactory::getInstance() {
    static ConverterFactory instance;
    return instance;
}

void ConverterFactory::registerConverter(const std::string& name,
                                        std::function<std::shared_ptr<Converter>()> creator) {
    converters_[name] = creator;
}

std::shared_ptr<Converter> ConverterFactory::createConverter(const std::string& name) {
    auto it = converters_.find(name);
    if (it == converters_.end()) {
        throw ConverterException("Unknown converter: " + name);
    }
    return it->second();
}

bool ConverterFactory::hasConverter(const std::string& name) const {
    return converters_.find(name) != converters_.end();
}

std::vector<std::string> ConverterFactory::getConverterNames() const {
    std::vector<std::string> names;
    for (const auto& kv : converters_) {
        names.push_back(kv.first);
    }
    return names;
}

std::string ConverterFactory::getConverterDescription(const std::string& name) {
    auto converter = createConverter(name);
    return converter->getDescription();
}

std::string ConverterFactory::getConverterSyntax(const std::string& name) {
    auto converter = createConverter(name);
    return converter->getSyntax();
}

void initializeConverterFactory() {
    ConverterFactory& factory = ConverterFactory::getInstance();

    factory.registerConverter("mute", []() { return std::make_shared<MuteConverter>(); });
    factory.registerConverter("mix", []() { return std::make_shared<MixConverter>(); });
    factory.registerConverter("volume", []() { return std::make_shared<VolumeConverter>(); });
    factory.registerConverter("echo", []() { return std::make_shared<EchoConverter>(); });
}

