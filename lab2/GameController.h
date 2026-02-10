//управление игрой
#pragma once
#include "Universe.h"
#include "FileHandler.h"
#include <memory>
#include <string>

class GameController {
private:
    std::unique_ptr<Universe> universe;
//gamecontroller владеет вселенной через uni_prt
public:
    void runInteractive(); //интерактивный режим с командами
    void runOffline(const std::string& inputFile, const std::string& outputFile, int iterations);//оффлайн(файл-обработка-файл)
    void loadUniverse(const std::string& filename);//загрузка вселенной через файл

private:
    void createDefaultUniverse();//созд вселенной
    void displayUniverse() const;//показ поле с инфо
    void displayHelp() const;//справка по командам
    void processCommand(const std::string& command);//обработка введенной команды
    void saveUniverse(const std::string& filename) const;//сохранение в файл
};
