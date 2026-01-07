#include <iostream>
#include "SoundProcessor/SoundProcessor.h"

int main(int argc, char* argv[]) {
    try {
        SoundProcessor processor;
        return processor.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}

