#pragma once

#include <string>
#include <vector>

/**
 * Класс, представляющий одну команду конвертера из конфигурационного файла
 */
class ConverterCommand {
public:
    /**
     * Конструктор команды конвертера
     */
    ConverterCommand(const std::string& name,
                     const std::vector<std::string>& parameters);

    /**
     * Конструктор по умолчанию
     */
    ConverterCommand() = default;

    ~ConverterCommand() = default;

    /**
     * Получение имени конвертера
     */
    std::string getName() const;

    /**
     * Получение всех параметров конвертера
     */
    const std::vector<std::string>& getParameters() const;

    /**
     * Получение количества параметров
     */
    size_t getParameterCount() const;

    /**
     * Получение параметра по индексу
     */
    std::string getParameter(size_t index) const;

    /**
     * Проверка, является ли параметр ссылкой на поток
     */
    static bool isStreamReference(const std::string& param);

    /**
     * Извлечение номера потока из ссылки
     */
    static int getStreamNumber(const std::string& param);

    /**
     * Проверка, является ли параметр целым числом
     */
    static bool isInteger(const std::string& param);

    /**
     * Проверка, является ли параметр числом с плавающей точкой
     */
    static bool isFloat(const std::string& param);

private:
    std::string name_;
    std::vector<std::string> parameters_;
};

