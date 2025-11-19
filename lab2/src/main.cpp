#include <iostream>
#include "CommandLineParser.h"

int main(int argc, char* argv[]) {
    CommandLineParser parser(argc, argv);
    parser.parse();
    parser.printArguments();
    return 0;
}