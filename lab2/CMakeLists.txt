cmake_minimum_required(VERSION 3.30)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

# Основное приложение
add_executable(lab2
        main.cpp
        GameRules.cpp
        Universe.cpp
        FileHandler.cpp
        GameController.cpp
)

# Поиск GoogleTest (после установки через brew)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Тестовое приложение
add_executable(run_tests
        tests/GameRulesTests.cpp
        tests/UniverseTests.cpp
        tests/FileHandlerTests.cpp
        GameRules.cpp
        Universe.cpp
        FileHandler.cpp
        GameController.cpp
)

# Подключение GoogleTest
target_link_libraries(run_tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
)

# Включение директорий
target_include_directories(lab2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(run_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Включение тестирования для CLion
enable_testing()
add_test(NAME AllTests COMMAND run_tests)
