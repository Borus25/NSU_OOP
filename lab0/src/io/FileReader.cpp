#include "FileReader.h"
#include <fstream>
#include <sstream>

FileReader::FileReader(const std::string& fname) : filename(fname) {}

std::vector<std::string> FileReader::readLines() const {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

bool FileReader::fileExists() const {
    std::ifstream file(filename);
    return file.good();
}

