#pragma once

#include "../BasicConverter/Converter.h"

class MuteConverter : public Converter {
public:
    MuteConverter();
    ~MuteConverter() override = default;

    void process(AudioStream& input, AudioStream& output) override;
    void setParameters(const std::vector<std::string>& params) override;

    std::string getName() const override;
    std::string getDescription() const override;
    std::string getSyntax() const override;

    void setStartTime(int seconds);
    void setEndTime(int seconds);

private:
    int startTime_;
    int endTime_;

    void validateParameters() const;
};

