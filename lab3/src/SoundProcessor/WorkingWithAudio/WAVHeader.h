#pragma once

#include <cstdint>
#include <iostream>
#include <string>

class WAVHeader {
public:
    WAVHeader();
    ~WAVHeader() = default;

    void readHeader(std::istream& input);
    void writeHeader(std::ostream& output, uint32_t dataSize);
    void validate() const;

    [[nodiscard]] uint16_t getAudioFormat() const { return audioFormat_; }

    [[nodiscard]] uint16_t getNumChannels() const { return numChannels_; }

    [[nodiscard]] uint32_t getSampleRate() const { return sampleRate_; }

    [[nodiscard]] uint32_t getByteRate() const { return byteRate_; }

    [[nodiscard]] uint16_t getBlockAlign() const { return blockAlign_; }

    [[nodiscard]] uint16_t getBitsPerSample() const { return bitsPerSample_; }

    [[nodiscard]] uint32_t getDataSize() const { return dataSize_; }


    [[nodiscard]] double getDuration() const;

    void createDefaultHeader(uint32_t dataSize);

    static constexpr uint16_t SUPPORTED_AUDIO_FORMAT = 1;
    static constexpr uint16_t SUPPORTED_NUM_CHANNELS = 1;
    static constexpr uint32_t SUPPORTED_SAMPLE_RATE = 44100;
    static constexpr uint16_t SUPPORTED_BITS_PER_SAMPLE = 16;

private:
    char chunkId_[4];
    uint32_t chunkSize_;
    char format_[4];

    char subchunk1Id_[4];
    uint32_t subchunk1Size_;
    uint16_t audioFormat_;
    uint16_t numChannels_;
    uint32_t sampleRate_;
    uint32_t byteRate_;
    uint16_t blockAlign_;
    uint16_t bitsPerSample_;

    char subchunk2Id_[4];
    uint32_t dataSize_;

    void readString(std::istream& input, char* buffer, size_t length);
    void writeString(std::ostream& output, const char* buffer, size_t length);
};

