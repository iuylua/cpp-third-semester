#pragma once
#include "Universe.h"
#include <memory>
#include <string>
//работа с файлами не хранит состояние только методы
class FileHandler {
public:
    //стат методы не требуют созд объекта
    static std::unique_ptr<Universe> loadFromFile(const std::string& filename); //загружает вселенную из файла;  может быть только один объект unique_ptr, который содержит один и тот же адрес
    //выз без созд объекта; возвращ указ на вселенную; имя файла по ссылке(не коп)
    static void saveToFile(const std::string& filename, const Universe& universe);//сохр вселенную в файл
};