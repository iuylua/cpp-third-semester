cmake_minimum_required(VERSION 3.30)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

#найти GoogleTest
find_package(GTest REQUIRED)

#создать исполняемый файл со ВСЕМИ исходными файлами
add_executable(lab1
        main.cpp
        test_BitArray.cpp
        BitArray.cpp
        BitArray.h
)

#подключить библиотеки GTest
target_link_libraries(lab1 GTest::gtest GTest::gtest_main)

#добавить include директории
target_include_directories(lab1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})