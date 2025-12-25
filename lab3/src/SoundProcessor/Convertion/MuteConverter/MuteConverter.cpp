#include "MuteConverter.h"
#include "../../WorkingWithAudio/AudioStream.h"
#include "../../../Exceptions.h"
#include <stdexcept>

MuteConverter::MuteConverter()
    : startTime_(0), endTime_(0)
{
}

void MuteConverter::process(AudioStream& input, AudioStream& output) {
    validateParameters();

    int startSample = secondsToSamples(startTime_);
    int endSample = secondsToSamples(endTime_);

    int sampleIndex = 0;

    while (input.hasMoreSamples()) {
        int16_t sample = input.readSample();

        // Если в диапазоне - записываем нуль (тишину), иначе оригинальный сэмпл
        if (sampleIndex >= startSample && sampleIndex < endSample) {
            output.writeSample(0);
        } else {
            output.writeSample(sample);
        }

        sampleIndex++;
    }
}

void MuteConverter::setParameters(const std::vector<std::string>& params) {
    if (params.size() < 2) {
        throw ConverterException("Mute converter requires 2 parameters: startTime endTime");
    }

    try {
        startTime_ = std::stoi(params[0]);
        endTime_ = std::stoi(params[1]);
    } catch (const std::exception& e) {
        throw ConverterException("Invalid parameters for mute converter: " + std::string(e.what()));
    }

    validateParameters();
}

std::string MuteConverter::getName() const {
    return "mute";
}

std::string MuteConverter::getDescription() const {
    return "Mutes (silences) the audio in a specified time interval.";
}

std::string MuteConverter::getSyntax() const {
    return "mute <startTime> <endTime>\n"
           "  startTime - start of mute interval (seconds)\n"
           "  endTime   - end of mute interval (seconds)";
}

void MuteConverter::setStartTime(int seconds) {
    startTime_ = seconds;
}

void MuteConverter::setEndTime(int seconds) {
    endTime_ = seconds;
}

void MuteConverter::validateParameters() const {
    if (startTime_ < 0) {
        throw ConverterException("Start time cannot be negative");
    }
    if (endTime_ < 0) {
        throw ConverterException("End time cannot be negative");
    }
    if (startTime_ > endTime_) {
        throw ConverterException("Start time cannot be greater than end time");
    }
}

