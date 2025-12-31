#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

class AudioStream {
public:
    // для входных аудио потоков
    AudioStream(std::shared_ptr<std::istream> input, uint32_t dataSize,
                std::streampos startPosition);

    // для выходных аудио потоков
    explicit AudioStream(std::shared_ptr<std::ostream> output);

    ~AudioStream() = default;

    int16_t readSample();

    void writeSample(int16_t sample);

    bool hasMoreSamples() const;

    void reset();

    double getDuration() const;

    uint32_t getTotalSamples() const;

    uint32_t getCurrentSampleIndex() const;

    uint32_t getWrittenSamples() const;

    void skipSamples(uint32_t count);

    void seekToSecond(double seconds);

    static constexpr uint32_t SAMPLE_RATE = 44100;
    static constexpr uint16_t BYTES_PER_SAMPLE = 2;

private:
    std::shared_ptr<std::istream> inputStream_;
    std::shared_ptr<std::ostream> outputStream_;

    uint32_t dataSize_;
    std::streampos startPosition_;
    uint32_t totalSamples_;
    uint32_t currentSampleIndex_;
    uint32_t writtenSamples_;
    bool isInputMode_;
};

