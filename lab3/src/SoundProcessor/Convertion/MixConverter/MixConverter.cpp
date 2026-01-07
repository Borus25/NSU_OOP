#include "MixConverter.h"
#include "../../WorkingWithAudio/AudioStream.h"
#include "../../utils/Exceptions.h"
#include <stdexcept>
#include <utility>

MixConverter::MixConverter()
    : additionalStream_(nullptr), insertPosition_(0)
{
}

void MixConverter::process(AudioStream& input, AudioStream& output) {
    validateStream();

    int insertSample = secondsToSamples(insertPosition_);
    int sampleIndex = 0;

    // Если дополнительный поток нужно вставить позже - скопируем начало основного потока
    while (input.hasMoreSamples() && sampleIndex < insertSample) {
        int16_t sample = input.readSample();
        output.writeSample(sample);
        sampleIndex++;
    }

    // Теперь микшируем
    while (input.hasMoreSamples()) {
        int16_t mainSample = input.readSample();
        int16_t additionalSample;

        if (additionalStream_->hasMoreSamples()) {
            additionalSample = additionalStream_->readSample();
        } else {
            // Если дополнительный поток закончился, используем только основной
            additionalSample = 0;
        }

        int16_t mixedSample = mixSamples(mainSample, additionalSample);
        output.writeSample(mixedSample);
        sampleIndex++;
    }
}

void MixConverter::setParameters(const std::vector<std::string>& params) {
    if (!params.empty()) {
        try {
            insertPosition_ = std::stoi(params[0]);
        } catch (const std::exception&) { }
    }
}

std::string MixConverter::getName() const {
    return "mix";
}

std::string MixConverter::getDescription() const {
    return "Mixes (combines) two audio streams by averaging their samples.";
}

std::string MixConverter::getSyntax() const {
    return "mix <streamReference> [insertPosition]\n"
           "  streamReference - reference to additional stream ($n)\n"
           "  insertPosition  - where to insert additional stream (seconds, default: 0)";
}

void MixConverter::setAdditionalStream(std::shared_ptr<AudioStream> stream) {
    additionalStream_ = std::move(stream);
}

void MixConverter::setInsertPosition(int seconds) {
    insertPosition_ = seconds;
}

int16_t MixConverter::mixSamples(int16_t sample1, int16_t sample2) const {
    return static_cast<int16_t>((static_cast<int32_t>(sample1) + static_cast<int32_t>(sample2)) / 2);
}

void MixConverter::validateStream() const {
    if (!additionalStream_) {
        throw ConverterException("Mix converter: additional stream is not set");
    }
}

