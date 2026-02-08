#pragma once

#include <string>
#include <vector>

class AudioStream;

class Converter {
public:
    virtual ~Converter() = default;

    virtual void process(AudioStream& input, AudioStream& output) = 0;
    virtual void setParameters(const std::vector<std::string>& params) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getSyntax() const = 0;

protected:
    static constexpr int SAMPLE_RATE = 44100;
    static constexpr int BYTES_PER_SAMPLE = 2;

    int secondsToSamples(int seconds) const;
};

