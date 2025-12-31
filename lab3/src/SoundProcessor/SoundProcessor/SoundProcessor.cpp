#include "SoundProcessor.h"
#include "../../Exceptions.h"
#include "../Convertion/MixConverter/MixConverter.h"
#include <iostream>
#include <memory>
#include <sstream>

extern void initializeConverterFactory();

SoundProcessor::SoundProcessor()
    : helpMode_(false), isConfigured_(false)
{
    initializeConverterFactory();
}

SoundProcessor::~SoundProcessor() {
    if (outputFile_) {
        outputFile_->close();
    }
    for (auto& file : inputFiles_) {
        if (file && file->isOpen()) {
            file->close();
        }
    }
}

int SoundProcessor::run(int argc, char* argv[]) {
    try {
        parseCommandLine(argc, argv);

        if (helpMode_) {
            printHelp(argv[0]);
            return 0;
        }

        if (!isConfigured_) {
            printHelp(argv[0]);
            return 4;
        }

        process();
        return 0;
    } catch (const FileException& e) {
        std::cerr << e.what() << std::endl;
        return e.getErrorCode();
    } catch (const InvalidFormatException& e) {
        std::cerr << e.what() << std::endl;
        return e.getErrorCode();
    } catch (const InvalidArgumentException& e) {
        std::cerr << e.what() << std::endl;
        return e.getErrorCode();
    } catch (const ConfigParseException& e) {
        std::cerr << e.what() << std::endl;
        return e.getErrorCode();
    } catch (const ConverterException& e) {
        std::cerr << e.what() << std::endl;
        return e.getErrorCode();
    } catch (const SPException& e) {
        std::cerr << e.what() << std::endl;
        return e.getErrorCode();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

void SoundProcessor::parseCommandLine(int argc, char* argv[]) {
    if (argc < 2) {
        throw InvalidArgumentException("No arguments provided");
    }

    // Проверка на флаг -h
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            helpMode_ = true;
            return;
        }
    }

    // Ищем флаг -c для режима с конфигурацией
    bool configMode = false;
    int configIndex = -1;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-c") {
            configMode = true;
            configIndex = i;
            break;
        }
    }

    if (!configMode) {
        throw InvalidArgumentException("Expected -c flag");
    }

    // После -c должны идти: config.txt output.wav input1.wav [input2.wav ...]
    if (configIndex + 3 >= argc) {
        throw InvalidArgumentException(
            "Expected: -c <config.txt> <output.wav> <input1.wav> [input2.wav ...]");
    }

    std::string configFile = argv[configIndex + 1];
    outputFilePath_ = argv[configIndex + 2];

    std::vector<std::string> inputFiles;
    for (int i = configIndex + 3; i < argc; ++i) {
        inputFiles.push_back(argv[i]);
    }

    try {
        loadConfig(configFile);
        loadInputFiles(inputFiles);
        isConfigured_ = true;
    } catch (const SPException&) {
        isConfigured_ = false;
        throw;
    }
}

void SoundProcessor::loadInputFiles(const std::vector<std::string>& filenames) {
    inputFiles_.clear();

    for (const auto& filename : filenames) {
        auto file = std::make_shared<WAVFile>();
        file->open(filename);
        inputFiles_.push_back(file);
    }
}

void SoundProcessor::loadConfig(const std::string& configFile) {
    configParser_.parse(configFile);
}

void SoundProcessor::process() {
    if (inputFiles_.empty()) {
        throw InvalidArgumentException("No input files loaded");
    }

    // Берем поток из первого входного файла
    auto currentStream = getInputStream(1);

    // Применяем все конвертеры
    currentStream = applyConverters(currentStream);

    // Сохраняем результат
    saveOutput(outputFilePath_, currentStream);
}

void SoundProcessor::saveOutput(const std::string& outputFile, const std::shared_ptr<AudioStream>& inputStream) {
    outputFile_ = std::make_shared<WAVFile>();
    outputFile_->createOutput(outputFile);

    auto outStream = outputFile_->getStream();

    // Копируем все сэмплы из входного потока в выходной
    while (inputStream->hasMoreSamples()) {
        int16_t sample = inputStream->readSample();
        outStream->writeSample(sample);
    }

    outputFile_->finalizeOutput();
}

void SoundProcessor::printHelp(const char* programName) const {
    std::cout << "Sound Processor - WAV audio file processing tool\n\n";
    std::cout << "Usage:\n";
    std::cout << "  " << programName << " -h\n";
    std::cout << "  " << programName << " -c <config.txt> <output.wav> <input1.wav> [input2.wav ...]\n\n";

    std::cout << "Options:\n";
    std::cout << "  -h              Show this help message\n";
    std::cout << "  -c              Configuration mode\n\n";

    std::cout << "Supported Converters:\n";
    ConverterFactory& factory = ConverterFactory::getInstance();

    for (const auto& name : factory.getConverterNames()) {
        std::cout << "\n  " << name << "\n";
        try {
            std::cout << "    Description: " << factory.getConverterDescription(name) << "\n";
            std::cout << "    Syntax:\n";

            std::string syntax = factory.getConverterSyntax(name);
            std::istringstream iss(syntax);
            std::string line;
            while (std::getline(iss, line)) {
                std::cout << "      " << line << "\n";
            }
        } catch (...) {
            // Пропускаем ошибки при выводе справки
        }
    }

    std::cout << "\nExample config file:\n";
    std::cout << "  # Mute first 10 seconds\n";
    std::cout << "  mute 0 10\n";
    std::cout << "  # Increase volume to 1.5x\n";
    std::cout << "  volume 1.5\n";
    std::cout << "  # Mix with another file starting at second 5\n";
    std::cout << "  mix $2 5\n";
}

bool SoundProcessor::isHelpMode() const {
    return helpMode_;
}

bool SoundProcessor::isConfigured() const {
    return isConfigured_;
}

std::shared_ptr<AudioStream> SoundProcessor::getInputStream(int fileIndex) {
    if (fileIndex < 1 || fileIndex > static_cast<int>(inputFiles_.size())) {
        throw InvalidArgumentException(
            "Invalid file index: " + std::to_string(fileIndex));
    }

    // Возвращаем новый поток для файла
    return inputFiles_[fileIndex - 1]->createNewStream();
}

std::shared_ptr<AudioStream> SoundProcessor::applyConverters(std::shared_ptr<AudioStream> input) {
    const auto& commands = configParser_.getCommands();

    if (commands.empty()) {
        return input;
    }

    auto currentStream = input;

    for (const auto& command : commands) {
        auto converter = ConverterFactory::getInstance().createConverter(command.getName());
        auto params = command.getParameters();

        std::vector<std::string> processedParams;
        for (size_t i = 0; i < params.size(); ++i) {
            const auto& param = params[i];
            if (ConverterCommand::isStreamReference(param)) {
                int streamNum = ConverterCommand::getStreamNumber(param);
                if (command.getName() == "mix") {
                    auto mixConverter = std::dynamic_pointer_cast<MixConverter>(converter);
                    if (mixConverter) {
                        auto additionalStream = getInputStream(streamNum);
                        mixConverter->setAdditionalStream(additionalStream);
                    }
                    continue;
                } else {
                    // Для других конвертеров пропускаем stream references
                    continue;
                }
            }
            processedParams.push_back(param);
        }

        converter->setParameters(processedParams);

        // ВАЖНО: stringstream нужен в режиме read+write для использования как входной и выходной поток
        auto outputBuffer = std::make_shared<std::stringstream>(std::ios::binary | std::ios::in | std::ios::out);
        std::shared_ptr<std::ostream> outputPtr = outputBuffer;

        AudioStream outStream(outputPtr);
        applyConverter(converter, *currentStream, outStream);

        uint32_t writtenSamples = outStream.getWrittenSamples();
        outputBuffer->seekg(0);

        // Для следующего конвертера создаем входной поток из буфера
        auto inputBuffer = std::dynamic_pointer_cast<std::stringstream>(outputBuffer);
        if (inputBuffer) {
            currentStream = std::make_shared<AudioStream>(
                inputBuffer, writtenSamples * AudioStream::BYTES_PER_SAMPLE, std::streampos(0));
        }
    }

    return currentStream;
}

void SoundProcessor::applyConverter(const std::shared_ptr<Converter>& converter,
                                   AudioStream& input, AudioStream& output) {
    converter->process(input, output);
}

