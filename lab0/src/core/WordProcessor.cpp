#include "WordProcessor.h"
#include <cctype>
#include <sstream>
#include <algorithm>

std::vector<std::string> WordProcessor::extractWords(const std::string& line) {
    std::vector<std::string> words;
    std::string currentWord;

    for (char c : line) {
        // Проверяем, является ли символ буквой или цифрой
        if (std::isalnum(static_cast<unsigned char>(c))) {
            currentWord += c;
        } else {
            // Это разделитель
            if (!currentWord.empty()) {
                words.push_back(toLower(currentWord));
                currentWord.clear();
            }
        }
    }

    // Добавляем последнее слово, если оно есть
    if (!currentWord.empty()) {
        words.push_back(toLower(currentWord));
    }

    return words;
}

void WordProcessor::processLines(
    const std::vector<std::string>& lines,
    WordCounter& counter
) {
    for (const auto& line : lines) {
        std::vector<std::string> words = extractWords(line);
        for (const auto& word : words) {
            counter.addWord(word);
        }
    }
}

std::string WordProcessor::toLower(const std::string& word) {
    std::string result = word;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    return result;
}

