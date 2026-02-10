#include <gtest/gtest.h>
#include "BitArray.h"
#include <iostream>

int main(int argc, char **argv) {
    //пример
    BitArray ans(8, 0xAAUL);
    std::cout << "Пример: " << ans.to_string() << std::endl;

    //запуск тестов
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
