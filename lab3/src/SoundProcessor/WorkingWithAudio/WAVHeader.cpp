#include "WAVHeader.h"
#include "../utils/Exceptions.h"
#include <cstring>

WAVHeader::WAVHeader()
    : chunkSize_(0), subchunk1Size_(16), audioFormat_(1),
      numChannels_(1), sampleRate_(44100), byteRate_(88200),
      blockAlign_(2), bitsPerSample_(16), dataSize_(0)
{
    strcpy(chunkId_, "RIFF");
    strcpy(format_, "WAVE");
    strcpy(subchunk1Id_, "fmt ");
    strcpy(subchunk2Id_, "data");
}

void WAVHeader::readHeader(std::istream& input) {
    readString(input, chunkId_, 4);
    input.read(reinterpret_cast<char*>(&chunkSize_), 4);
    readString(input, format_, 4);

    char subchunkId[5] = {0};
    uint32_t subchunkSize = 0;

    readString(input, subchunkId, 4);
    while (std::string(subchunkId) != "fmt ") {
        input.read(reinterpret_cast<char*>(&subchunkSize), 4);
        input.ignore(subchunkSize);
        readString(input, subchunkId, 4);
    }

    strcpy(subchunk1Id_, "fmt ");
    input.read(reinterpret_cast<char*>(&subchunk1Size_), 4);

    input.read(reinterpret_cast<char*>(&audioFormat_), 2);
    input.read(reinterpret_cast<char*>(&numChannels_), 2);
    input.read(reinterpret_cast<char*>(&sampleRate_), 4);
    input.read(reinterpret_cast<char*>(&byteRate_), 4);
    input.read(reinterpret_cast<char*>(&blockAlign_), 2);
    input.read(reinterpret_cast<char*>(&bitsPerSample_), 2);

    if (subchunk1Size_ > 16) {
        input.ignore(subchunk1Size_ - 16);
    }

    readString(input, subchunkId, 4);
    while (std::string(subchunkId) != "data") {
        input.read(reinterpret_cast<char*>(&subchunkSize), 4);
        input.ignore(subchunkSize);
        readString(input, subchunkId, 4);
    }

    strcpy(subchunk2Id_, "data");
    input.read(reinterpret_cast<char*>(&dataSize_), 4);

    if (!input.good()) {
        throw FileException("Failed to read WAV header");
    }

    validate();
}

void WAVHeader::writeHeader(std::ostream& output, uint32_t dataSize) {
    chunkSize_ = 36 + dataSize;
    dataSize_ = dataSize;

    writeString(output, chunkId_, 4);
    output.write(reinterpret_cast<const char*>(&chunkSize_), 4);
    writeString(output, format_, 4);

    writeString(output, subchunk1Id_, 4);
    output.write(reinterpret_cast<const char*>(&subchunk1Size_), 4);

    output.write(reinterpret_cast<const char*>(&audioFormat_), 2);
    output.write(reinterpret_cast<const char*>(&numChannels_), 2);
    output.write(reinterpret_cast<const char*>(&sampleRate_), 4);
    output.write(reinterpret_cast<const char*>(&byteRate_), 4);
    output.write(reinterpret_cast<const char*>(&blockAlign_), 2);
    output.write(reinterpret_cast<const char*>(&bitsPerSample_), 2);

    writeString(output, subchunk2Id_, 4);
    output.write(reinterpret_cast<const char*>(&dataSize_), 4);

    if (!output.good()) {
        throw FileException("Failed to write WAV header");
    }
}

void WAVHeader::validate() const {
    if (audioFormat_ != SUPPORTED_AUDIO_FORMAT) {
        throw InvalidFormatException(
            "Unsupported audio format. Only PCM (format 1) is supported.");
    }

    if (numChannels_ != SUPPORTED_NUM_CHANNELS) {
        throw InvalidFormatException(
            "Unsupported channel count. Only mono (1 channel) is supported.");
    }

    if (sampleRate_ != SUPPORTED_SAMPLE_RATE) {
        throw InvalidFormatException(
            "Unsupported sample rate. Only 44100 Hz is supported.");
    }

    if (bitsPerSample_ != SUPPORTED_BITS_PER_SAMPLE) {
        throw InvalidFormatException(
            "Unsupported bits per sample. Only 16-bit is supported.");
    }
}

double WAVHeader::getDuration() const {
    if (sampleRate_ == 0) {
        return 0.0;
    }
    return static_cast<double>(dataSize_) / (sampleRate_ * blockAlign_);
}

void WAVHeader::createDefaultHeader(uint32_t dataSize) {
    audioFormat_ = SUPPORTED_AUDIO_FORMAT;
    numChannels_ = SUPPORTED_NUM_CHANNELS;
    sampleRate_ = SUPPORTED_SAMPLE_RATE;
    bitsPerSample_ = SUPPORTED_BITS_PER_SAMPLE;
    blockAlign_ = numChannels_ * (bitsPerSample_ / 8);
    byteRate_ = sampleRate_ * blockAlign_;
    dataSize_ = dataSize;
    chunkSize_ = 36 + dataSize;
}

void WAVHeader::readString(std::istream& input, char* buffer, size_t length) {
    input.read(buffer, length);
    if (!input.good()) {
        throw FileException("Failed to read from file");
    }
}

void WAVHeader::writeString(std::ostream& output, const char* buffer, size_t length) {
    output.write(buffer, length);
    if (!output.good()) {
        throw FileException("Failed to write to file");
    }
}

