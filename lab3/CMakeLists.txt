cmake_minimum_required(VERSION 3.31)
project(lab3)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем отладочную информацию
set(CMAKE_BUILD_TYPE Debug)

if(MSVC)
    add_compile_options(/W4 /DEBUG)
else()
    add_compile_options(-g -Wall -Wextra)
endif()

add_executable(lab3 src/main.cpp
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
    src/SoundProcessor/SoundProcessor.cpp
        src/tests/Tests.cpp
)



