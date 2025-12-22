#pragma once

#include <string>
#include <vector>

class FileReader {
private:
    std::string filename;

public:
    explicit FileReader(const std::string& fname);

    // Прочитать весь файл и вернуть строки
    std::vector<std::string> readLines() const;

    // Проверить существование файла
    bool fileExists() const;
};


