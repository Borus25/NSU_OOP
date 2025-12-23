#include "WordCounter.h"
#include <algorithm>

WordCounter::WordCounter() {}

void WordCounter::addWord(const std::string& word) {
    if (!word.empty()) {
        wordFrequency[word]++;
    }
}

int WordCounter::getFrequency(const std::string& word) const {
    auto it = wordFrequency.find(word);
    if (it != wordFrequency.end()) {
        return it->second;
    }
    return 0;
}

std::vector<std::pair<std::string, int>> WordCounter::getSortedWords() const {
    // Создаем вектор пар из map для сортировки
    std::vector<std::pair<std::string, int>> result(
        wordFrequency.begin(),
        wordFrequency.end()
    );

    // Сортируем по убыванию частоты
    std::sort(result.begin(), result.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        }
    );

    return result;
}

int WordCounter::getTotalWords() const {
    int total = 0;
    for (const auto& pair : wordFrequency) {
        total += pair.second;
    }
    return total;
}

