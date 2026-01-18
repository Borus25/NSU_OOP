#include "EchoConverter.h"
#include "../../WorkingWithAudio/AudioStream.h"
#include "../../utils/Exceptions.h"
#include <limits>

EchoConverter::EchoConverter()
    : delayMs_(250), decay_(0.6f), repeatCount_(1)
{
}

void EchoConverter::process(AudioStream& input, AudioStream& output) {
    validateParameters();

    initializeDelayBuffer(input.getTotalSamples());

    size_t sampleIndex = 0;
    while (input.hasMoreSamples()) {
        int16_t inputSample = input.readSample();
        int16_t outputSample = applyEcho(inputSample, sampleIndex);
        output.writeSample(outputSample);
        sampleIndex++;
    }
}

void EchoConverter::setParameters(const std::vector<std::string>& params) {
    if (params.empty()) {
        throw ConverterException("Echo converter requires at least 1 parameter: delay");
    }

    try {
        delayMs_ = std::stoi(params[0]);

        if (params.size() > 1) {
            decay_ = std::stof(params[1]);
        }
        if (params.size() > 2) {
            repeatCount_ = std::stoi(params[2]);
        }
    } catch (const std::exception& e) {
        throw ConverterException("Invalid parameters for echo converter: " + std::string(e.what()));
    }

    validateParameters();
}

std::string EchoConverter::getName() const {
    return "echo";
}

std::string EchoConverter::getDescription() const {
    return "Adds an echo effect to the audio with repeating delayed copies.";
}

std::string EchoConverter::getSyntax() const {
    return "echo <delay> [decay] [repeatCount]\n"
           "  delay       - echo delay (milliseconds)\n"
           "  decay       - amplitude decay factor (0.0-1.0, default: 0.6)\n"
           "  repeatCount - number of echo repeats (default: 1)";
}

void EchoConverter::setDelay(int milliseconds) {
    delayMs_ = milliseconds;
}

void EchoConverter::setDecay(float decay) {
    decay_ = decay;
}

void EchoConverter::setRepeatCount(int count) {
    repeatCount_ = count;
}

void EchoConverter::initializeDelayBuffer(uint32_t totalSamples) {
    delayBuffer_.assign(totalSamples, 0);
}

void EchoConverter::validateParameters() const {
    if (delayMs_ <= 0) {
        throw ConverterException("Echo delay must be positive");
    }
    if (decay_ < 0.0f || decay_ > 1.0f) {
        throw ConverterException("Decay factor must be between 0.0 and 1.0");
    }
    if (repeatCount_ < 0) {
        throw ConverterException("Repeat count must be non-negative");
    }
}

int16_t EchoConverter::applyEcho(int16_t inputSample, size_t sampleIndex) {
    int delaySamples = (delayMs_ * SAMPLE_RATE) / 1000;
    int32_t output = static_cast<int32_t>(inputSample);

    for (int r = 0; r < repeatCount_; ++r) {
        size_t echoPos = sampleIndex + delaySamples * (r + 1);
        if (echoPos < delayBuffer_.size()) {
            float decayFactor = 1.0f;
            for (int i = 0; i < r + 1; ++i) {
                decayFactor *= decay_;
            }

            int16_t echoSample = delayBuffer_[echoPos];
            output += static_cast<int32_t>(echoSample * decayFactor);
        }
    }

    if (sampleIndex < delayBuffer_.size()) {
        delayBuffer_[sampleIndex] = inputSample;
    }

    return clamp(output);
}

int16_t EchoConverter::clamp(int32_t value) const {
    const int32_t MIN_VALUE = std::numeric_limits<int16_t>::min();
    const int32_t MAX_VALUE = std::numeric_limits<int16_t>::max();

    if (value < MIN_VALUE) return MIN_VALUE;
    if (value > MAX_VALUE) return MAX_VALUE;
    return static_cast<int16_t>(value);
}

