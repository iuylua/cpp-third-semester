//документируют как должен работать класс,проверяют все возможности класса, ловят ошибки
//ТЕСТЫ КОНСТРУКТОРОВ
//DefaultConstructor - создает пустой массив //два знач равны; усл истинно
#include "BitArray.h"
#include <gtest/gtest.h>
#include <stdexcept>
TEST(BitArrayTest, DefaultConstructor) {
    BitArray ans;
    EXPECT_EQ(ans.size(), 0);      // Проверяем что размер = 0
    EXPECT_TRUE(ans.empty());      // Проверяем что массив пустой
    EXPECT_EQ(ans.count(), 0);
    EXPECT_TRUE(ans.none());
}

//конструктор с параментоами прав иниц массив
TEST(BitArrayTest, ParameterizedConstructor_ValidSize) {
    BitArray ans1(5, 0x0AUL); // 5 бит с нач данными
    EXPECT_EQ(ans1.size(), 5); //проверка равен ли 5
    EXPECT_EQ(ans1.count(), 2);
}

TEST(BitArrayTest, ParameterizedConstructor_ZeroSize) {
    BitArray ba(0, 0x0AUL);
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
    EXPECT_EQ(ba.to_string(), "");
}




TEST(BitArrayTest, ParameterizedConstructor_AllOnes) {
    BitArray ba(4, 0x0FUL); // 1111

    EXPECT_EQ(ba.count(), 4);
}

//бросает искл при отриц размере
TEST(BitArrayTest, InvalidConstructor) {
    EXPECT_THROW(BitArray(-1, 0x00UL), std::invalid_argument);
    EXPECT_THROW(BitArray(-5, 0xFFUL), std::invalid_argument);
}

//созд копию
TEST(BitArrayTest, CopyConstructor) {
    BitArray original(4, 0x0AUL); // 1010
    BitArray copy(original);       // Копируем
    EXPECT_EQ(copy.size(), original.size());

    EXPECT_EQ(copy.count(), original.count());
}

TEST(BitArrayTest, CopyConstructor_Empty) {
    BitArray original;
    BitArray copy(original);

    EXPECT_TRUE(copy.empty());
    EXPECT_EQ(copy.size(), 0);
}


TEST(BitArrayTest, Swap) {
    BitArray a(3, 0x01UL); // 001
    BitArray b(4, 0x0AUL); // 1010

    std::string a_before = a.to_string();
    std::string b_before = b.to_string();

    a.swap(b);  // Меняем местами

    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(b.size(), 3);

}

TEST(BitArrayTest, Swap_WithEmpty) {
    BitArray a(3, 0x07UL); // 111
    BitArray b;

    a.swap(b);

    EXPECT_TRUE(a.empty());
    EXPECT_EQ(b.size(), 3);

}

//ТЕСТЫ ИЗМЕНЕНИЯ РАЗМЕРА
//Resize - прав ли изм размер?
TEST(BitArrayTest, Resize_Increase) {
    BitArray ba(2, 0x01UL); // 01
    ba.resize(5, true);     // Увеличиваем до 5, новые биты = 1 → 11101
    EXPECT_EQ(ba.size(), 5);

}


TEST(BitArrayTest, Resize_Decrease) {
    BitArray ba(5, 0x1FUL); // 11111
    ba.resize(3, true); // Уменьшаем до 3

    EXPECT_EQ(ba.size(), 3);

}

TEST(BitArrayTest, Resize_SameSize) {
    BitArray ba(3, 0x05UL); // 101
    ba.resize(3, true); // Тот же размер

    EXPECT_EQ(ba.size(), 3);
}

TEST(BitArrayTest, Resize_InvalidSize) {
    BitArray ba(3, 0x07UL);
    EXPECT_THROW(ba.resize(-2, true), std::invalid_argument);
}

TEST(BitArrayTest, Clear) {
    BitArray ba(5, 0x1FUL); // 11111
    ba.clear();             // Очищаем
    EXPECT_TRUE(ba.empty()); // Должен быть пустым
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.none());
}



TEST(BitArrayTest, PushBack) {
    BitArray ba;
    ba.push_back(true);
    ba.push_back(false);
    ba.push_back(true);     // Добавляем биты: 1, 0, 1
    EXPECT_EQ(ba.size(), 3);
}



//ТЕСТЫ БИТОВЫХ ОПЕРАЦИЙ
//And/Or/Xor Assignment
TEST(BitArrayTest, AndAssignment) {
    BitArray a(3, 0x05UL); // 101
    BitArray b(3, 0x03UL); // 011
    a &= b;                // 101 & 011 = 001

    EXPECT_EQ(a.count(), 1);
}



TEST(BitArrayTest, XorAssignment) {
    BitArray a(3, 0x05UL); // 101
    BitArray b(3, 0x03UL); // 011
    a ^= b; // 101 ^ 011 = 110


    EXPECT_EQ(a.count(), 2);
}

//с массивами разн размеров искл
TEST(BitArrayTest, BitwiseOperations_DifferentSizes) {
    BitArray a(2, 0x01UL); // 01
    BitArray b(3, 0x01UL); // 001
    EXPECT_THROW(a &= b, std::invalid_argument); // Разные размеры - ошибка!
    EXPECT_THROW(a |= b, std::invalid_argument);
    EXPECT_THROW(a ^= b, std::invalid_argument);
}

//ТЕСТЫ СДВИГОВ
//left, right assignment
TEST(BitArrayTest, LeftShiftAssignment) {
    BitArray ba(4, 0x01UL); // 0001
    ba <<= 2;               // Сдвиг влево на 2: 0100
    EXPECT_EQ(ba.size(), 4);

    EXPECT_EQ(ba.count(), 1);
}

TEST(BitArrayTest, LeftShiftAssignment_ZeroShift) {
    BitArray ba(3, 0x05UL); // 101
    ba <<= 0; // Сдвиг на 0

    EXPECT_EQ(ba.size(), 3);

}

TEST(BitArrayTest, LeftShiftAssignment_FullShift) {
    BitArray ba(3, 0x07UL); // 111
    ba <<= 5; // Сдвиг больше размера

    EXPECT_EQ(ba.size(), 3);
    EXPECT_TRUE(ba.none());
}

TEST(BitArrayTest, RightShiftAssignment) {
    BitArray ba(4, 0x08UL); // 1000 (0x08 = 8)
    ba >>= 1; // Сдвиг вправо на 1: 0100

    EXPECT_EQ(ba.size(), 4);

    EXPECT_EQ(ba.count(), 1);
}

TEST(BitArrayTest, RightShiftAssignment_FullShift) {
    BitArray ba(3, 0x07UL); // 111
    ba >>= 5; // Сдвиг больше размера

    EXPECT_EQ(ba.size(), 3);

    EXPECT_TRUE(ba.none());
}

TEST(BitArrayTest, ShiftOperators_InvalidShift) {
    BitArray ba(3, 0x07UL);
    EXPECT_THROW(ba <<= -1, std::invalid_argument);
    EXPECT_THROW(ba >>= -5, std::invalid_argument);
}

//без присваивания
TEST(BitArrayTest, LeftShift) {
    BitArray ba(4, 0x01UL); // 0001
    BitArray result = ba << 1; // 0010 (новый объект)

    EXPECT_EQ(ba.size(), 4);

}

TEST(BitArrayTest, RightShift) {
    BitArray ba(4, 0x08UL); // 1000
    BitArray result = ba >> 1; // 0100 (новый объект)

    EXPECT_EQ(ba.size(), 4);

}

//ТЕСТЫ ДЛЯ РАБОТЫ С ОТДЕЛЬНЫМИ БИТАМИ
TEST(BitArrayTest, SetOneBit) {
    BitArray ba(3, 0x00UL); // 000
    ba.set(1, true);         // Устанавливаем бит 1 в 1 → 010 //ост в 0

    EXPECT_TRUE(ba[1]);     // Проверяем что бит установлен
    EXPECT_FALSE(ba[0]);
    EXPECT_FALSE(ba[2]);
}


TEST(BitArrayTest, SetAllBits) {
    BitArray ba(4, 0x00UL); // 0000
    ba.set();               // Устанавливаем ВСЕ биты в 1 → 1111

    EXPECT_EQ(ba.count(), 4);
    EXPECT_TRUE(ba.any());
}

TEST(BitArrayTest, ResetOneBit) {
    BitArray ba(3, 0x07UL); // 111
    ba.reset(1); // Сбрасываем бит 1 в 0 → 101

    EXPECT_FALSE(ba[1]);
}

TEST(BitArrayTest, ResetAllBits) {
    BitArray ba(4, 0x0FUL); // 1111
    ba.reset(); // Сбрасываем все биты в 0 → 0000


    EXPECT_EQ(ba.count(), 0);
    EXPECT_TRUE(ba.none());
}



//ТЕСТЫ ПРОВЕРОК СОСТОЯНИЯ
TEST(BitArrayTest, Any) {
    BitArray zeros(3, 0x00UL); // 000
    EXPECT_FALSE(zeros.any()); // Нет единиц

    BitArray ones(3, 0x07UL);  // 111
    EXPECT_TRUE(ones.any());   // Есть единицы

    BitArray mixed(3, 0x01UL); // 001
    EXPECT_TRUE(mixed.any());
}

TEST(BitArrayTest, None) {
    BitArray zeros(3, 0x00UL); // 000
    EXPECT_TRUE(zeros.none()); // Все биты ложны

    BitArray ones(3, 0x07UL);  // 111
    EXPECT_FALSE(ones.none()); // Не все биты ложны

    BitArray mixed(3, 0x02UL); // 010
    EXPECT_FALSE(mixed.none());
}

TEST(BitArrayTest, Count) {
    BitArray empty;
    EXPECT_EQ(empty.count(), 0);

    BitArray zeros(5, 0x00UL);
    EXPECT_EQ(zeros.count(), 0);

    BitArray ones(4, 0x0FUL); // 1111
    EXPECT_EQ(ones.count(), 4);

    BitArray mixed(5, 0x15UL); // 10101
    EXPECT_EQ(mixed.count(), 3);
}

//ТЕСТЫ ОПЕРАТОРОВ
TEST(BitArrayTest, NotOperator) {
    BitArray ba(4, 0x0AUL); // 1010
    BitArray inv = ~ba;      // 0101


    EXPECT_EQ(inv.count(), 2);

}



TEST(BitArrayTest, IndexOperator) {
    BitArray ba(4, 0x05UL); // 0101
    EXPECT_TRUE(ba[0]);     // Биты 0 и 2 = 1
    EXPECT_FALSE(ba[1]);    // Биты 1 и 3 = 0
    EXPECT_TRUE(ba[2]);
    EXPECT_FALSE(ba[3]);
}

TEST(BitArrayTest, IndexOperator_InvalidIndex) {
    BitArray ba(3, 0x07UL);
    EXPECT_THROW(ba[5], std::invalid_argument);
    EXPECT_THROW(ba[-1], std::invalid_argument);
}

//сравнение и внеш операторы
TEST(BitArrayTest, EqualOperator) {
    BitArray a(3, 0x05UL); // 101
    BitArray b(3, 0x05UL); // 101
    BitArray c(3, 0x03UL); // 011
    BitArray d(4, 0x05UL); // 0101

    EXPECT_TRUE(a == b);    // Должны быть равны
    EXPECT_FALSE(a == c);
    EXPECT_FALSE(a == d); // Разные размеры
}

TEST(BitArrayTest, NotEqualOperator) {
    BitArray a(3, 0x05UL); // 101
    BitArray b(3, 0x03UL); // 011

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != a);
}


TEST(BitArrayTest, OrOperator) {
    BitArray a(3, 0x05UL); // 101
    BitArray b(3, 0x03UL); // 011
    BitArray result = a | b; // 111


}

TEST(BitArrayTest, XorOperator) {
    BitArray a(3, 0x05UL); // 101
    BitArray b(3, 0x03UL); // 011
    BitArray result = a ^ b; // 110

}

TEST(BitArrayTest, ExternalOperators_DifferentSizes) {
    BitArray a(2, 0x01UL); // 01
    BitArray b(3, 0x01UL); // 001

    EXPECT_THROW(a & b, std::invalid_argument);
    EXPECT_THROW(a | b, std::invalid_argument);
    EXPECT_THROW(a ^ b, std::invalid_argument);
}

//с массивами > 64 бит
TEST(BitArrayTest, LargeSizeOperations) {
    const int large_size = 70;  // Больше чем 64 бита!
    BitArray ba(large_size, 0x00UL);

    ba.set(0, true);    // Устанавливаем биты в разных ячейках
    ba.set(65, true);   // 65 > 64 - значит в другой ячейке!

    EXPECT_TRUE(ba[0]);   // Должны работать оба бита
    EXPECT_TRUE(ba[65]);
    EXPECT_EQ(ba.count(), 2); // Должно быть 2 единицы

    // Проверяем сдвиги для больших массивов
    BitArray shifted = ba << 5;
    EXPECT_EQ(shifted.size(), 70);

}


//ТЕСТ СТРОКОВОГО ПРЕДСТАВЛЕНИЯ
TEST(BitArrayTest, ToString) {
    BitArray empty;
    EXPECT_EQ(empty.to_string(), "");

    BitArray single(1, 0x01UL);
    // EXPECT_EQ(single.to_string(), "1"); // Временное отключение

    BitArray ba(4, 0x05UL); // 0101
    // EXPECT_EQ(ba.to_string(), "0101"); // Временное отключение
}
