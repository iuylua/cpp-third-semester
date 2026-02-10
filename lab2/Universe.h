//ИНТЕРФЕЙС ИГРОВОГО ПОЛЯ
#pragma once
#include "GameRules.h"
#include <vector>
#include <string>

class Universe {
private: //доступ только через методы класса
    std::vector<std::vector<bool>> grid; //двумерный массив для хранения состояния клеток:внеш(строки),внут(столбцы):клетки в строке
    //bool-живая/мертвая
    int width, height; //размеры поля
    int generation; //тек поколение
    GameRules rules;
    std::string name; //имя вселенной пользователя

public:
    Universe(int w, int h, const GameRules& r, const std::string& n = "");//параметры размера,правила по ссылке+const
    void nextGeneration(); //расчет след поколения
    void tick(int n = 1); //расчет след нескольких поколений
    //методы доступа к данным
    bool getCell(int x, int y) const; //узнать состояние клетки
    void setCell(int x, int y, bool state); //изменить состояние клетки
    void clear();

    int countNeighbors(int x, int y) const; //сколько соседей у клетки
    int countLiveCells() const; //сколько всего живых клеток
    //READ-ONLY доступ ГЕТТЕРЫ ПОЛУЧИТЬ ИНФО
    int getWidth() const { return width; } //какая ширина поля
    int getHeight() const { return height; } //высота поля
    int getGeneration() const { return generation; } //узнать номер поколения
    const GameRules& getRules() const { return rules; } //узнать правила
    const std::string& getName() const { return name; } //имя

private:
    int wrapX(int x) const;
    int wrapY(int y) const;
};

//Universe - это "чёрный ящик":

//Знаю: какие методы вызывать (интерфейс)
//Не знаю: как именно он работает внутри (реализация)