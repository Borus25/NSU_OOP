cmake_minimum_required(VERSION 3.31)
project(lab2)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем отладочную информацию
set(CMAKE_BUILD_TYPE Debug)

# Добавляем флаги компилятора для отладки
if(MSVC)
    add_compile_options(/W4 /DEBUG)
else()
    add_compile_options(-g -Wall -Wextra)
endif()

# Здесь добавьте исполняемый файл проекта
# Замените src/main.cpp на путь к вашему главному файлу
add_executable(${PROJECT_NAME} src/main.cpp
        src/utils/CommandLineParser/CommandLineParser.cpp
        src/basic_strategy/History/History.cpp
        src/configs/AlwaysCooperate/AlwaysCooperate.cpp
        src/configs/AlwaysDefect/AlwaysDefect.cpp
        src/utils/GameMatrix/GameMatrix.cpp
        src/configs/RandomChoice/RandomChoice.cpp
        src/configs/ToughTitForTat/ToughTitForTat.cpp
        src/configs/SoftTitForTat/SoftTitForTat.cpp
        src/configs/GoByMajority/GoByMajority.cpp
)
