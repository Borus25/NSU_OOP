#pragma once

#include "../BasicConverter/Converter.h"

class VolumeConverter : public Converter {
public:
    VolumeConverter();
    ~VolumeConverter() override = default;

    void process(AudioStream& input, AudioStream& output) override;
    void setParameters(const std::vector<std::string>& params) override;

    std::string getName() const override;
    std::string getDescription() const override;
    std::string getSyntax() const override;

    void setVolumeFactor(float factor);
    void setTimeRange(int start = -1, int end = -1);

private:
    float volumeFactor_;
    int startTime_;
    int endTime_;

    int16_t applySampleVolume(int16_t sample) const;
    int16_t clamp(int32_t value) const;
    void validateParameters() const;
};

