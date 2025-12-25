#include "AudioStream.h"
#include "../../Exceptions.h"
#include <cstring>
#include <limits>

AudioStream::AudioStream(std::shared_ptr<std::istream> input, uint32_t dataSize,
                         std::streampos startPosition)
    : inputStream_(input),
      dataSize_(dataSize),
      startPosition_(startPosition),
      totalSamples_(dataSize / BYTES_PER_SAMPLE),
      currentSampleIndex_(0),
      writtenSamples_(0),
      isInputMode_(true)
{
    if (!inputStream_ || !inputStream_->good()) {
        throw AudioStreamException("Invalid input stream");
    }
}

AudioStream::AudioStream(std::shared_ptr<std::ostream> output)
    : outputStream_(output),
      dataSize_(0),
      totalSamples_(0),
      currentSampleIndex_(0),
      writtenSamples_(0),
      isInputMode_(false)
{
    if (!outputStream_ || !outputStream_->good()) {
        throw AudioStreamException("Invalid output stream");
    }
}

int16_t AudioStream::readSample() {
    if (!isInputMode_) {
        throw AudioStreamException("Cannot read from output stream");
    }

    if (!hasMoreSamples()) {
        throw AudioStreamException("No more samples to read");
    }

    int16_t sample = 0;
    char buffer[BYTES_PER_SAMPLE];

    inputStream_->read(buffer, BYTES_PER_SAMPLE);

    if (inputStream_->gcount() != BYTES_PER_SAMPLE) {
        throw AudioStreamException("Failed to read sample from stream");
    }

    // Преобразование из little-endian
    sample = static_cast<int16_t>(
        (static_cast<uint8_t>(buffer[0])) |
        (static_cast<uint8_t>(buffer[1]) << 8)
    );

    currentSampleIndex_++;
    return sample;
}

void AudioStream::writeSample(int16_t sample) {
    if (isInputMode_) {
        throw AudioStreamException("Cannot write to input stream");
    }

    char buffer[BYTES_PER_SAMPLE];

    // Преобразование в little-endian
    buffer[0] = static_cast<char>(sample & 0xFF);
    buffer[1] = static_cast<char>((sample >> 8) & 0xFF);

    outputStream_->write(buffer, BYTES_PER_SAMPLE);

    if (!outputStream_->good()) {
        throw AudioStreamException("Failed to write sample to stream");
    }

    writtenSamples_++;
}

bool AudioStream::hasMoreSamples() const {
    if (!isInputMode_) {
        return true; // Для выходного потока всегда можно писать
    }
    return currentSampleIndex_ < totalSamples_;
}

void AudioStream::reset() {
    if (!isInputMode_) {
        throw AudioStreamException("Cannot reset output stream");
    }

    inputStream_->clear();
    inputStream_->seekg(startPosition_);
    currentSampleIndex_ = 0;

    if (!inputStream_->good()) {
        throw AudioStreamException("Failed to reset stream position");
    }
}

double AudioStream::getDuration() const {
    return static_cast<double>(totalSamples_) / SAMPLE_RATE;
}

uint32_t AudioStream::getTotalSamples() const {
    if (isInputMode_) {
        return totalSamples_;
    }
    return writtenSamples_;
}

uint32_t AudioStream::getCurrentSampleIndex() const {
    if (isInputMode_) {
        return currentSampleIndex_;
    }
    return writtenSamples_;
}

uint32_t AudioStream::getWrittenSamples() const {
    return writtenSamples_;
}

void AudioStream::skipSamples(uint32_t count) {
    if (!isInputMode_) {
        throw AudioStreamException("Cannot skip samples in output stream");
    }

    for (uint32_t i = 0; i < count && hasMoreSamples(); ++i) {
        char buffer[BYTES_PER_SAMPLE];
        inputStream_->read(buffer, BYTES_PER_SAMPLE);
        if (inputStream_->gcount() != BYTES_PER_SAMPLE) {
            throw AudioStreamException("Failed to skip samples");
        }
        currentSampleIndex_++;
    }
}

void AudioStream::seekToSecond(double seconds) {
    if (!isInputMode_) {
        throw AudioStreamException("Cannot seek in output stream");
    }

    uint32_t targetSample = static_cast<uint32_t>(seconds * SAMPLE_RATE);

    if (targetSample >= totalSamples_) {
        currentSampleIndex_ = totalSamples_;
        return;
    }

    inputStream_->clear();
    std::streampos newPos = startPosition_ + static_cast<std::streamoff>(targetSample * BYTES_PER_SAMPLE);
    inputStream_->seekg(newPos);

    if (!inputStream_->good()) {
        throw AudioStreamException("Failed to seek to position");
    }

    currentSampleIndex_ = targetSample;
}

