#include "CSVWriter.h"
#include <fstream>
#include <iomanip>
#include <sstream>

CSVWriter::CSVWriter(const std::string& fname) : filename(fname) {}

void CSVWriter::writeWordFrequency(
    const std::vector<std::pair<std::string, int>>& words,
    int totalWords
) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    // Пишем заголовок
    file << "Слово,Частота,Частота (%)" << std::endl;

    // Пишем данные (вектор уже отсортирован по убыванию частоты)
    for (const auto& pair : words) {
        std::string word = pair.first;
        int frequency = pair.second;
        double percentage = (totalWords > 0) ? (100.0 * frequency / totalWords) : 0.0;

        file << word << "," << frequency << ",";
        file << std::fixed << std::setprecision(2) << percentage << std::endl;
    }

    file.close();
}

