#include <gtest/gtest.h>
#include "GameRules.h"

TEST(GameRulesTest, DefaultConstructor) {
    GameRules rules; //созд объект правил
    // Правило B3/S23
    //проверка на рождение
    EXPECT_TRUE(rules.shouldBorn(3)); //при 3 соседях - рождение
    EXPECT_FALSE(rules.shouldBorn(2)); //не рождается ✗
    EXPECT_FALSE(rules.shouldBorn(4)); //✗
    //проверка на выживание
    EXPECT_TRUE(rules.shouldSurvive(2)); //при 2 соседях - выживает
    EXPECT_TRUE(rules.shouldSurvive(3)); //при 3 - выживает
    EXPECT_FALSE(rules.shouldSurvive(4));//при 4 - ✗
}

//правильно ли разбирается строка с правилами??
TEST(GameRulesTest, CustomRulesParsing) {
    GameRules rules("B36/S23");//пользовательские правила
    //B36(рождение при 3 или 6 соседях)
    EXPECT_TRUE(rules.shouldBorn(3)); //✓
    EXPECT_TRUE(rules.shouldBorn(6)); //✓
    EXPECT_FALSE(rules.shouldBorn(2)); //✗
    //S23(выживание при 2 или 3 соседях)
    EXPECT_TRUE(rules.shouldSurvive(2)); //✓
    EXPECT_TRUE(rules.shouldSurvive(3)); //✓
    EXPECT_FALSE(rules.shouldSurvive(6));//✗
}

//правильно ли преобраз правила обратно в строку??
TEST(GameRulesTest, ToStringMethod) {
    GameRules rules1("B3/S23"); //строку в объект
    EXPECT_EQ(rules1.toString(), "B3/S23"); //объект обратно в строку

    GameRules rules2("B36/S235");
    EXPECT_EQ(rules2.toString(), "B36/S235");
}

//при неккоректных данных программа не падает
TEST(GameRulesTest, InvalidRulesFallback) {
    EXPECT_NO_THROW({
        GameRules rules("InvalidString");
        // Можно проверить какие-то базовые вещи
        std::string str = rules.toString();
        EXPECT_FALSE(str.empty());
    });
}

//игнор пробелов в строке правил
TEST(GameRulesTest, SpacesInRules) {
    GameRules rules("B 3 6 / S 2 3 5");
    EXPECT_TRUE(rules.shouldBorn(3));
    EXPECT_TRUE(rules.shouldBorn(6));
    EXPECT_FALSE(rules.shouldBorn(2));

    EXPECT_TRUE(rules.shouldSurvive(2));
    EXPECT_TRUE(rules.shouldSurvive(3));
    EXPECT_TRUE(rules.shouldSurvive(5));
}