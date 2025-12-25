#pragma once

#include "../BasicConverter/Converter.h"
#include <memory>

class AudioStream;

class MixConverter : public Converter {
public:
    MixConverter();
    ~MixConverter() override = default;

    void process(AudioStream& input, AudioStream& output) override;
    void setParameters(const std::vector<std::string>& params) override;

    std::string getName() const override;
    std::string getDescription() const override;
    std::string getSyntax() const override;

    void setAdditionalStream(std::shared_ptr<AudioStream> stream);
    void setInsertPosition(int seconds);

private:
    std::shared_ptr<AudioStream> additionalStream_;
    int insertPosition_;

    int16_t mixSamples(int16_t sample1, int16_t sample2) const;
    void validateStream() const;
};

