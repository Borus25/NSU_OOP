#pragma once

#include <string>
#include <vector>
#include <utility>

class CSVWriter {
private:
    std::string filename;

public:
    explicit CSVWriter(const std::string& fname);

    // Написать CSV файл со словами и частотами
    void writeWordFrequency(
        const std::vector<std::pair<std::string, int>>& words,
        int totalWords
    );
};


