#pragma once

#include "../WorkingWithAudio/WAVFile.h"
#include "../Convertion/BasicConverter/Converter.h"
#include "../utils/ConfigParser.h"
#include "../Convertion/Factory/ConverterFactory.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

/**
 * Управляющий класс Sound Processor
 * Связывает все компоненты системы: WAV файлы, конвертеры, конфигурацию
 * Отвечает за выполнение полного цикла обработки аудио
 */
class SoundProcessor {
public:
    SoundProcessor();
    ~SoundProcessor();

    /**
     * Главная функция запуска программы
     * @param argc - количество аргументов командной строки
     * @param argv - массив аргументов командной строки
     * @return код возврата (0 = успех, ненулевой = ошибка)
     */
    int run(int argc, char* argv[]);

    /**
     * Разбор аргументов командной строки
     * Формат: sound_processor [-h] [-c config.txt output.wav input1.wav [input2.wav ...]]
     * @param argc - количество аргументов
     * @param argv - массив аргументов
     * @throws InvalidArgumentException если аргументы некорректны
     */
    void parseCommandLine(int argc, char* argv[]);

    /**
     * Загрузка входных WAV файлов
     * @param filenames - вектор путей к входным файлам
     * @throws FileException если файлы не могут быть открыты
     * @throws InvalidFormatException если формат файлов не поддерживается
     */
    void loadInputFiles(const std::vector<std::string>& filenames);

    /**
     * Загрузка конфигурационного файла
     * @param configFile - путь к файлу конфигурации
     * @throws ConfigParseException если файл содержит ошибки
     */
    void loadConfig(const std::string& configFile);

    /**
     * Выполнение обработки аудио согласно конфигурации
     * @throws ConverterException если конвертер не может быть создан или выполнен
     */
    void process();

    /**
     * Сохранение результата в выходной файл
     * @param outputFile - путь к выходному файлу
     * @param inputStream - входной поток с данными для записи
     * @throws FileException если файл не может быть создан
     */
    void saveOutput(const std::string& outputFile, const std::shared_ptr<AudioStream>& inputStream);

    /**
     * Вывод справки об использовании программы
     */
    void printHelp(const char* programName) const;

    /**
     * Проверка, был ли установлен режим справки
     * @return true если нужно вывести справку
     */
    bool isHelpMode() const;

    /**
     * Проверка, была ли установлена полная конфигурация для обработки
     * @return true если все параметры установлены корректно
     */
    bool isConfigured() const;

private:
    std::vector<std::shared_ptr<WAVFile>> inputFiles_;  // Открытые входные файлы
    std::shared_ptr<WAVFile> outputFile_;               // Выходной файл
    ConfigParser configParser_;                         // Парсер конфигурации
    std::string outputFilePath_;                        // Путь к выходному файлу
    bool helpMode_;                                     // Флаг режима справки
    bool isConfigured_;                                 // Флаг настройки

    /**
     * Получение аудио потока для входного файла
     * Если поток уже использовался (для повторного чтения того же файла),
     * создается новый поток с начала файла
     * @param fileIndex - индекс входного файла (1-based)
     * @return аудио поток для чтения
     * @throws InvalidArgumentException если индекс выходит за границы
     */
    std::shared_ptr<AudioStream> getInputStream(int fileIndex);

    /**
     * Применение цепочки конвертеров к входному потоку
     * @return аудио поток с результатом обработки
     */
    std::shared_ptr<AudioStream> applyConverters(std::shared_ptr<AudioStream> input);

    /**
     * Применение одного конвертера
     * @param converter - конвертер для применения
     * @param input - входной аудио поток
     * @param output - выходной аудио поток
     */
    void applyConverter(const std::shared_ptr<Converter>& converter,
                       AudioStream& input, AudioStream& output);
};

