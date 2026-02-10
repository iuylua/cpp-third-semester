#include "FileHandler.h"
#include <fstream> //для работы с файлами
#include <sstream>
#include <iostream>

std::unique_ptr<Universe> FileHandler::loadFromFile(const std::string& filename) { //созд вселенную из файла;умн указ на созданную вселенную
    std::ifstream file(filename); //поток для чтения файл
    if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);//ошибка искл

    std::string line; //для чтения строк из файла
    std::string universeName = "Unnamed"; //имя вселенной
    GameRules rules("B3/S23");//правила

    std::getline(file, line); // заголовок перв строка

    if (std::getline(file, line) && line.substr(0, 3) == "#N ") //пров что 3 первые символа строки; пров что это имя; берем все после #N
        universeName = line.substr(3);//счит имя вселенной

    if (std::getline(file, line) && line.substr(0, 3) == "#R ")//проверяем что это правила(#R),берем строку из правил,созд объект правил из строки
        rules = GameRules(line.substr(3));

    auto universe = std::make_unique<Universe>(40, 20, rules, universeName);//созд объект вселенной и возвр uni_ptr
    //с загруж размерами и правилами
    while (std::getline(file, line)) {//чит до конца файла
        if (line.empty() || line[0] == '#') continue;//пропускаю пустые строки и комментарии
        std::istringstream iss(line); //созд поток из строки
        int x, y; //чит координаты
        if (iss >> x >> y) universe->setCell(x, y, true);//ставим живую клетку
    }

    return universe; //возвр созд вселенную
}

void FileHandler::saveToFile(const std::string& filename, const Universe& universe) {//сохр вселенную в файл savetoload
    //имя файла и вселенная по ссылке не коп
    std::ofstream file(filename);//поток для записи в файл
    if (!file.is_open()) throw std::runtime_error("Cannot create file: " + filename);
    //запись заголовка
    file << "#Life 1.06\n";//формат
    file << "#N " << universe.getName() << "\n";//имя
    file << "#R " << universe.getRules().toString() << "\n";//правила

    for (int y = 0; y < universe.getHeight(); y++) {
        for (int x = 0; x < universe.getWidth(); x++) { //двойной цикл по всем клеткам
            if (universe.getCell(x, y)) file << x << " " << y << "\n";//живая ли?? запись координат
        }
    }
}
