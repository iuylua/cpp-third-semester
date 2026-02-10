//ИНТЕРФЕЙС ПРАВИЛ
#pragma once //защита от повторного включения файла
#include <set>
#include <string>

class GameRules {
private:
    //приватные поля - инкапсуляция(поля скрыты, а работа с ними идет через публичные методы)
    std::set<int> birth; //мн-во усл для рождения; уникальные эл, отсортированные эл
    std::set<int> survival; //мн-во усл для выживания

public:
    //конструктор(инициализация объекта, установка коррект начального значения)
    GameRules(const std::string& ruleStr = "B3/S23"); //rulestr нельзя изменять внутри метода
    //передача по ссылке, передаю ссылку на сущест ориг строку, а не копию строки

    //публичные методы(описание того, что объект делает, без деталей как он устроен)
    bool shouldBorn(int neighbors) const; //метод не меняет объект
    bool shouldSurvive(int neighbors) const;
    std::string toString() const;
};