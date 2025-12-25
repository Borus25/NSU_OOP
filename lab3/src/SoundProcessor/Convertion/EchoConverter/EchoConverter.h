#pragma once

#include "../BasicConverter/Converter.h"
#include <cstdint>
#include <vector>

class AudioStream;

class EchoConverter : public Converter {
public:
    EchoConverter();
    ~EchoConverter() override = default;

    void process(AudioStream& input, AudioStream& output) override;
    void setParameters(const std::vector<std::string>& params) override;

    std::string getName() const override;
    std::string getDescription() const override;
    std::string getSyntax() const override;

    void setDelay(int milliseconds);
    void setDecay(float decay);
    void setRepeatCount(int count);

private:
    int delayMs_;
    float decay_;
    int repeatCount_;

    std::vector<int16_t> delayBuffer_;

    void initializeDelayBuffer(uint32_t totalSamples);
    void validateParameters() const;
    int16_t applyEcho(int16_t inputSample, size_t sampleIndex);
    int16_t clamp(int32_t value) const;
};

