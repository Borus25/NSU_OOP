#include <iostream>
#include "SoundProcessor/SoundProcessor.h"
#include "tests/Tests.h"

int main(int argc, char* argv[]) {
    TestALL();
    try {
        SoundProcessor processor;
        return processor.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}

