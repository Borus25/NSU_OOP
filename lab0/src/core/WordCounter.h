#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>

class WordCounter {
private:
    std::map<std::string, int> wordFrequency;

public:
    WordCounter();

    // Добавить слово в счетчик
    void addWord(const std::string& word);

    // Получить частоту слова
    int getFrequency(const std::string& word) const;

    // Получить все слова и их частоты отсортированные по убыванию (в виде вектора пар)
    std::vector<std::pair<std::string, int>> getSortedWords() const;

    // Получить общее количество слов
    int getTotalWords() const;
};


