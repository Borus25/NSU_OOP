cmake_minimum_required(VERSION 3.31)
project(lab4)

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
add_executable(sub_task1
        sub_task1/src/main.cpp)

add_executable(sub_task2
        sub_task2/src/main.cpp
        sub_task2/src/CSVParser/CSVIterator.h
        sub_task2/src/CSVParser/Parser.h
        sub_task2/src/CSVParser/TupleDetails.h)

add_executable(sub_task2_Improved
        sub_task2_Improved/src/main.cpp
        sub_task2_Improved/src/CSVParser/CSVIterator.h
        sub_task2_Improved/src/CSVParser/Parser.h
        sub_task2_Improved/src/CSVParser/TupleDetails.h)




