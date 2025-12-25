#pragma once

#include "WAVHeader.h"
#include "AudioStream.h"
#include <string>
#include <memory>
#include <fstream>

/**
 * Класс для работы с WAV файлами
 * Объединяет работу с заголовком и аудио потоком
 */
class WAVFile {
public:
    WAVFile();
    ~WAVFile();

    /**
     * Открытие WAV файла для чтения
     * @param filename - путь к файлу
     * @throws FileException если файл не может быть открыт
     * @throws InvalidFormatException если формат не поддерживается
     */
    void open(const std::string& filename);

    /**
     * Создание нового WAV файла для записи
     * @param filename - путь к файлу
     * @throws FileException если файл не может быть создан
     */
    void createOutput(const std::string& filename);

    /**
     * Закрытие файла
     * При закрытии выходного файла обновляется заголовок с корректным размером
     */
    void close();

    /**
     * Получение заголовка файла
     * @return константная ссылка на заголовок
     */
    const WAVHeader& getHeader() const;

    /**
     * Получение аудио потока
     * @return указатель на аудио поток
     */
    std::shared_ptr<AudioStream> getStream();

    /**
     * Создание нового экземпляра потока для того же файла
     * Позволяет многократно читать один и тот же входной файл
     * @return новый указатель на аудио поток
     */
    std::shared_ptr<AudioStream> createNewStream();

    /**
     * Проверка, открыт ли файл
     * @return true если файл открыт
     */
    bool isOpen() const;

    /**
     * Проверка, является ли файл входным
     * @return true если файл открыт для чтения
     */
    bool isInputFile() const;

    /**
     * Проверка, является ли файл выходным
     * @return true если файл открыт для записи
     */
    bool isOutputFile() const;

    /**
     * Получение имени файла
     * @return путь к файлу
     */
    std::string getFilename() const;

    /**
     * Финализация выходного файла
     * Обновляет заголовок с корректным размером данных
     */
    void finalizeOutput();

private:
    std::string filename_;                      // Имя файла
    std::shared_ptr<std::ifstream> inputFile_;  // Входной файловый поток
    std::shared_ptr<std::ofstream> outputFile_; // Выходной файловый поток

    WAVHeader header_;                          // Заголовок WAV файла
    std::shared_ptr<AudioStream> stream_;       // Аудио поток

    bool isInput_;                              // Флаг входного файла
    bool isOutput_;                             // Флаг выходного файла

    std::streampos dataStartPosition_;          // Позиция начала аудио данных
};

