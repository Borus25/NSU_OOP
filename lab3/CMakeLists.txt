cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp

    src/SoundProcessor/WorkingWithAudio/AudioStream.cpp
    src/SoundProcessor/WorkingWithAudio/WAVFile.cpp
    src/SoundProcessor/WorkingWithAudio/WAVHeader.cpp

    src/SoundProcessor/Convertion/BasicConverter/Converter.cpp

    src/SoundProcessor/Convertion/MuteConverter/MuteConverter.cpp

    src/SoundProcessor/Convertion/VolumeConverter/VolumeConverter.cpp

    src/SoundProcessor/Convertion/MixConverter/MixConverter.cpp

    src/SoundProcessor/Convertion/EchoConverter/EchoConverter.cpp

    src/SoundProcessor/Convertion/Factory/ConverterFactory.cpp

    src/SoundProcessor/utils/ConfigParser.cpp
    src/SoundProcessor/utils/ConverterCommand.cpp

    src/SoundProcessor/SoundProcessor/SoundProcessor.cpp
)

add_executable(lab3 ${SOURCES})

target_include_directories(lab3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(lab3 PRIVATE /W4)
else()
    target_compile_options(lab3 PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()

set(TEST_SOURCES
    src/SoundProcessor/utils/ConverterCommand.cpp
    src/SoundProcessor/utils/ConfigParser.cpp
    src/SoundProcessor/Convertion/BasicConverter/Converter.cpp
    src/SoundProcessor/Convertion/MuteConverter/MuteConverter.cpp
    src/SoundProcessor/Convertion/VolumeConverter/VolumeConverter.cpp
    src/SoundProcessor/Convertion/EchoConverter/EchoConverter.cpp
    src/SoundProcessor/Convertion/MixConverter/MixConverter.cpp
    src/SoundProcessor/Convertion/Factory/ConverterFactory.cpp
    src/SoundProcessor/WorkingWithAudio/WAVHeader.cpp
    src/SoundProcessor/WorkingWithAudio/AudioStream.cpp
)

# Тест ConverterCommand
add_executable(test_converter_command
    ${TEST_SOURCES}
    src/tests/test_converter_command.cpp
)
target_include_directories(test_converter_command PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME ConverterCommandTest COMMAND test_converter_command)

# Тест ConfigParser
add_executable(test_config_parser
    ${TEST_SOURCES}
    src/tests/test_config_parser.cpp
)
target_include_directories(test_config_parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME ConfigParserTest COMMAND test_config_parser)

# Тест ConverterFactory
add_executable(test_converter_factory
    ${TEST_SOURCES}
    src/tests/test_converter_factory.cpp
)
target_include_directories(test_converter_factory PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME ConverterFactoryTest COMMAND test_converter_factory)

# Тест MuteConverter
add_executable(test_mute_converter
    ${TEST_SOURCES}
    src/tests/test_mute_converter.cpp
)
target_include_directories(test_mute_converter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME MuteConverterTest COMMAND test_mute_converter)

# Тест VolumeConverter
add_executable(test_volume_converter
    ${TEST_SOURCES}
    src/tests/test_volume_converter.cpp
)
target_include_directories(test_volume_converter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME VolumeConverterTest COMMAND test_volume_converter)

# Тест EchoConverter
add_executable(test_echo_converter
    ${TEST_SOURCES}
    src/tests/test_echo_converter.cpp
)
target_include_directories(test_echo_converter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME EchoConverterTest COMMAND test_echo_converter)

# Тест MixConverter
add_executable(test_mix_converter
    ${TEST_SOURCES}
    src/tests/test_mix_converter.cpp
)
target_include_directories(test_mix_converter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME MixConverterTest COMMAND test_mix_converter)

# Тест WAVHeader
add_executable(test_wav_header
    ${TEST_SOURCES}
    src/tests/test_wav_header.cpp
)
target_include_directories(test_wav_header PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME WAVHeaderTest COMMAND test_wav_header)

# Тест AudioStream
add_executable(test_audio_stream
    ${TEST_SOURCES}
    src/tests/test_audio_stream.cpp
)
target_include_directories(test_audio_stream PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME AudioStreamTest COMMAND test_audio_stream)

# Тест Exceptions
add_executable(test_exceptions
    src/tests/test_exceptions.cpp
)
target_include_directories(test_exceptions PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME ExceptionsTest COMMAND test_exceptions)

