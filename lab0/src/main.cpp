#include <iostream>
#include "io/FileReader.h"
#include "core/WordCounter.h"
#include "core/WordProcessor.h"
#include "io/CSVWriter.h"

int main(int argc, char* argv[]) {
    // Проверяем количество аргументов
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.txt> <output.csv>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    try {
        // Читаем входной файл
        FileReader reader(inputFile);
        std::vector<std::string> lines = reader.readLines();

        // Подсчитываем частоты слов
        WordCounter counter;
        WordProcessor::processLines(lines, counter);

        // Получаем отсортированные слова
        std::vector<std::pair<std::string, int>> sortedWords = counter.getSortedWords();
        int totalWords = counter.getTotalWords();

        // Пишем результаты в CSV файл
        CSVWriter writer(outputFile);
        writer.writeWordFrequency(sortedWords, totalWords);

        std::cout << "Successfully processed " << totalWords << " words." << std::endl;
        std::cout << "Results written to " << outputFile << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

