#include "Converter.h"

int Converter::secondsToSamples(int seconds) const {
    return seconds * SAMPLE_RATE;
}

