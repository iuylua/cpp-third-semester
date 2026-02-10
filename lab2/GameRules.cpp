//РЕАЛИЗАЦИЯ ПРАВИЛ
#include "GameRules.h"
#include <algorithm>
GameRules::GameRules(const std::string& ruleStr) { //реал Конструктора класса GameRules(из gamerules.h);
    // принимает строку правил по ссылке и не меняет ее (const)
    std::string clean = ruleStr; //созд копию строки
    //удал пробелов, сдвиг всех непробельных симв влево, удал хвоста
    clean.erase(std::remove(clean.begin(), clean.end(), ' '), clean.end());

    size_t bPos = clean.find('B'); //первое вхожд симв(возвращ позицию)
    size_t slashPos = clean.find('/');

    //проверка корректности(нет не найденных: / B в строке)
    if (bPos != std::string::npos && slashPos != std::string::npos) {
        //ПАРСИНГ УСЛОВИЙ РОЖДЕНИЯ
        for (size_t i = bPos + 1; i < slashPos; i++) {//с симв после B; до симв после /
            //проверка что символ цифра - конвертирую в число и доб в множество
            if (std::isdigit(clean[i])) birth.insert(clean[i] - '0');
        }
        //ПАРСИНГ УСЛОВИЙ ВЫЖИВАНИЯ
        for (size_t i = slashPos + 1; i < clean.length(); i++) {
            if (std::isdigit(clean[i])) survival.insert(clean[i] - '0');
        }
    }
}
//проверка усл рождения
bool GameRules::shouldBorn(int neighbors) const { //не меняет сост объекта
    return birth.count(neighbors) > 0; //преобраз в bool; ищу соседей в множестве birth
    //родится если кол-во соседей есть в birth
}
//проверка усл выживания
bool GameRules::shouldSurvive(int neighbors) const {
    return survival.count(neighbors) > 0;
}


std::string GameRules::toString() const {
    std::string result = "B"; //созд строку-рез, начинаем с B
    //проход по всем элементам множества birth, конвертируем число в строку
    for (int n : birth) result += std::to_string(n);
    result += "/S";
    //после /S доб цифры из survival
    for (int n : survival) result += std::to_string(n);
    return result;
}

//ПРИМЕР РАБОТЫ
//GameRules rules("B3/S23");
// birth = {3}, survival = {2, 3}

//rules.shouldBorn(3) → true      // 3 ∈ {3}
//rules.shouldBorn(2) → false     // 2 ∉ {3}
//rules.shouldSurvive(2) → true   // 2 ∈ {2,3}
//rules.shouldSurvive(4) → false  // 4 ∉ {2,3}