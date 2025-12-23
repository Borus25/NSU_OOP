#pragma once

#include <string>
#include <vector>
#include "WordCounter.h"

class WordProcessor {
public:
    // Извлечь слова из строки (разделителями считаются все не буквы и не цифры)
    static std::vector<std::string> extractWords(const std::string& line);

    // Обработать все строки и заполнить счетчик
    static void processLines(
        const std::vector<std::string>& lines,
        WordCounter& counter
    );

private:
    // Преобразовать слово в нижний регистр
    static std::string toLower(const std::string& word);
};


