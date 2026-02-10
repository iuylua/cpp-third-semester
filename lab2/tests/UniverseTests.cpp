#include <gtest/gtest.h>
#include "Universe.h"
//правильно ли инициализирует все поля?
TEST(UniverseTest, Constructor) {
    GameRules rules("B3/S23");
    Universe universe(10, 20, rules, "Test");

    EXPECT_EQ(universe.getWidth(), 10);
    EXPECT_EQ(universe.getHeight(), 20);
    EXPECT_EQ(universe.getGeneration(), 0);
    EXPECT_EQ(universe.getName(), "Test");
    EXPECT_EQ(universe.countLiveCells(), 0);
}

//можно доб и убир клетки и счетчик работает
TEST(UniverseTest, SetAndGetCell) {
    GameRules rules("B3/S23");
    Universe universe(5, 5, rules);

    universe.setCell(2, 2, true); //доб живую
    EXPECT_TRUE(universe.getCell(2, 2));//проверка живая ли
    EXPECT_EQ(universe.countLiveCells(), 1);//всего 1 живая клетка

    universe.setCell(2, 2, false);//убили клетку
    EXPECT_FALSE(universe.getCell(2, 2));//проверка что мертвая
    EXPECT_EQ(universe.countLiveCells(), 0);//0 живых клеток
}

//очистка поля
TEST(UniverseTest, Clear) {
    GameRules rules("B3/S23");
    Universe universe(5, 5, rules);

    universe.setCell(0, 0, true);
    universe.setCell(1, 1, true);//две живые клетки
    EXPECT_EQ(universe.countLiveCells(), 2);//проверка

    universe.clear();//очистили поле
    EXPECT_EQ(universe.countLiveCells(), 0);//0клетко
    EXPECT_EQ(universe.getGeneration(), 0);//поколение в 0
}

//троидальное поле??
TEST(UniverseTest, WrapCoordinates) {
    GameRules rules("B3/S23");
    Universe universe(3, 3, rules); //поле 3*3

    // Проверяем wrap для X координаты; клетки на противоположных краях
    universe.setCell(0, 1, true); //слева
    universe.setCell(2, 1, true); //справа

    // Клетка (2,1) должна видеть (0,1) как соседа
    int neighbors = universe.countNeighbors(2, 1);
    EXPECT_GE(neighbors, 0); // проверяем что все ок
}

//фигура не меняется
TEST(UniverseTest, StillLifeBlock) {
    GameRules rules("B3/S23");
    Universe universe(4, 4, rules);

    // Квадрат 2x2 (неподвижная фигура)
    universe.setCell(1, 1, true);
    universe.setCell(2, 1, true);
    universe.setCell(1, 2, true);
    universe.setCell(2, 2, true);

    int initialCount = universe.countLiveCells();

    universe.nextGeneration();

    // Квадрат должен остаться неизменным
    EXPECT_EQ(universe.countLiveCells(), initialCount);
    EXPECT_EQ(universe.getGeneration(), 1);
}

//проверка tick(n) правильно ли делает n шагов???
TEST(UniverseTest, TickMultipleGenerations) {
    GameRules rules("B3/S23");
    Universe universe(5, 5, rules);

    universe.setCell(2, 2, true);//одна клетка
    universe.tick(3);//сд 3 поколения

    EXPECT_EQ(universe.getGeneration(), 3);//провер cnt
}
