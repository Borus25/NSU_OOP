#include "VolumeConverter.h"
#include "../../WorkingWithAudio/AudioStream.h"
#include "../../../Exceptions.h"
#include <stdexcept>
#include <limits>
#include <climits>

VolumeConverter::VolumeConverter()
    : volumeFactor_(1.0f), startTime_(-1), endTime_(-1)
{
}

void VolumeConverter::process(AudioStream& input, AudioStream& output) {
    validateParameters();

    int startSample = (startTime_ >= 0) ? secondsToSamples(startTime_) : 0;
    int endSample = (endTime_ >= 0) ? secondsToSamples(endTime_) : INT_MAX;

    int sampleIndex = 0;

    while (input.hasMoreSamples()) {
        int16_t sample = input.readSample();

        if (sampleIndex >= startSample && sampleIndex < endSample) {
            sample = applySampleVolume(sample);
        }

        output.writeSample(sample);
        sampleIndex++;
    }
}

void VolumeConverter::setParameters(const std::vector<std::string>& params) {
    if (params.empty()) {
        throw ConverterException("Volume converter requires at least 1 parameter: factor");
    }

    try {
        volumeFactor_ = std::stof(params[0]);

        if (params.size() > 1) {
            startTime_ = std::stoi(params[1]);
        }
        if (params.size() > 2) {
            endTime_ = std::stoi(params[2]);
        }
    } catch (const std::exception& e) {
        throw ConverterException("Invalid parameters for volume converter: " + std::string(e.what()));
    }

    validateParameters();
}

std::string VolumeConverter::getName() const {
    return "volume";
}

std::string VolumeConverter::getDescription() const {
    return "Changes the volume (amplitude) of the audio.";
}

std::string VolumeConverter::getSyntax() const {
    return "volume <factor> [startTime] [endTime]\n"
           "  factor    - volume factor (0.5 = half, 1.0 = normal, 2.0 = double)\n"
           "  startTime - optional start of effect (seconds)\n"
           "  endTime   - optional end of effect (seconds)";
}

void VolumeConverter::setVolumeFactor(float factor) {
    volumeFactor_ = factor;
}

void VolumeConverter::setTimeRange(int start, int end) {
    startTime_ = start;
    endTime_ = end;
}

int16_t VolumeConverter::applySampleVolume(int16_t sample) const {
    int32_t scaled = static_cast<int32_t>(sample) * volumeFactor_;
    return clamp(scaled);
}

int16_t VolumeConverter::clamp(int32_t value) const {
    const int32_t MIN_VALUE = std::numeric_limits<int16_t>::min();
    const int32_t MAX_VALUE = std::numeric_limits<int16_t>::max();

    if (value < MIN_VALUE) return MIN_VALUE;
    if (value > MAX_VALUE) return MAX_VALUE;
    return static_cast<int16_t>(value);
}

void VolumeConverter::validateParameters() const {
    if (volumeFactor_ < 0.0f) {
        throw ConverterException("Volume factor cannot be negative");
    }
    if (startTime_ >= 0 && endTime_ >= 0 && startTime_ > endTime_) {
        throw ConverterException("Start time cannot be greater than end time");
    }
}

