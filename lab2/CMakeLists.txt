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
        src/PrisonerDilemma/utils/CommandLineParser/CommandLineParser.cpp
        src/PrisonerDilemma/strategies/basic_strategy/History/History.cpp
        src/PrisonerDilemma/strategies/config/AlwaysCooperate/AlwaysCooperate.cpp
        src/PrisonerDilemma/strategies/config/AlwaysDefect/AlwaysDefect.cpp
        src/PrisonerDilemma/GameMatrix/GameMatrix.cpp
        src/PrisonerDilemma/strategies/config/RandomChoice/RandomChoice.cpp
        src/PrisonerDilemma/strategies/config/ToughTitForTat/ToughTitForTat.cpp
        src/PrisonerDilemma/strategies/config/SoftTitForTat/SoftTitForTat.cpp
        src/PrisonerDilemma/strategies/config/GoByMajority/GoByMajority.cpp
        src/PrisonerDilemma/Player/Player.cpp
        src/PrisonerDilemma/GameClass/Game.cpp
)
