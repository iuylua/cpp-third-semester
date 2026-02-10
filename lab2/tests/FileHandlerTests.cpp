#include <gtest/gtest.h>
#include "FileHandler.h"
#include <fstream>
#include <cstdio>

class FileHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем тестовый файл перед каждым тестом
        std::ofstream file("test_life.life");
        file << "#Life 1.06\n";
        file << "#N Test Universe\n";
        file << "#R B3/S23\n";
        file << "1 1\n";
        file << "2 2\n";
        file << "3 3\n";
        file.close();
    }

    void TearDown() override {
        // Удаляем тестовые файлы
        std::remove("test_life.life");
        std::remove("test_output.life");
        std::remove("temp_test.life");
    }
};
//корректно ли загружается читается файл?
TEST_F(FileHandlerTest, LoadFromValidFile) {
    auto universe = FileHandler::loadFromFile("test_life.life");

    ASSERT_NE(universe, nullptr);
    EXPECT_EQ(universe->getName(), "Test Universe");
    EXPECT_EQ(universe->getRules().toString(), "B3/S23");
    EXPECT_EQ(universe->countLiveCells(), 3); // Просто проверяем количество
}
//сохранение файла
TEST_F(FileHandlerTest, SaveToFile) {
    // Создаем вселенную
    GameRules rules("B3/S23");
    Universe universe(40, 20, rules, "Save Test");
    universe.setCell(5, 5, true);
    universe.setCell(6, 6, true);

    // Сохраняем
    EXPECT_NO_THROW({
        FileHandler::saveToFile("test_output.life", universe);
    });

    // Проверяем что файл создан
    std::ifstream infile("test_output.life");
    EXPECT_TRUE(infile.good());
    infile.close();
}
//сохр+загрузка
TEST_F(FileHandlerTest, SaveAndLoadRoundtrip) {
    // Упрощенный тест - просто проверяем что сохранение и загрузка работают
    GameRules rules("B3/S23");
    Universe original(10, 10, rules, "Simple Test");
    original.setCell(3, 3, true);

    // Сохраняем
    FileHandler::saveToFile("test_output.life", original);

    // Загружаем
    auto loaded = FileHandler::loadFromFile("test_output.life");

    // Проверяем базовые вещи
    ASSERT_NE(loaded, nullptr);
    EXPECT_EQ(loaded->getName(), "Simple Test");
    EXPECT_EQ(loaded->getRules().toString(), "B3/S23");
}
//ошибки
TEST_F(FileHandlerTest, LoadNonExistentFileThrows) {
    EXPECT_THROW({
        FileHandler::loadFromFile("non_existent_file.life");
    }, std::runtime_error);
}
//с комментариями
TEST_F(FileHandlerTest, FileWithComments) {
    // Создаем файл с комментариями
    std::ofstream file("temp_test.life");
    file << "#Life 1.06\n";
    file << "#N Commented Universe\n";
    file << "# This is a comment\n";
    file << "#R B3/S23\n";
    file << "1 1\n";
    file << "# Another comment\n";
    file << "2 2\n";
    file.close();

    auto universe = FileHandler::loadFromFile("temp_test.life");
    EXPECT_NE(universe, nullptr);
    EXPECT_EQ(universe->getName(), "Commented Universe");
}
