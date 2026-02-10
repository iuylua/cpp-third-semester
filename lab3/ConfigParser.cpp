#include "ConfigParser.h"
#include <sstream>
#include <fstream>

std::vector<ConfigParser::Command> ConfigParser::parse(const std::string& filename) {
    std::ifstream f(filename);//чтение файла
    if (!f) {
        throw "ConfigParser::parse: Не удалось открыть файл конфигурации: " + filename;
    }

    std::vector<Command> commands;//найденные командв
    std::string line;//тек строка
    while (std::getline(f, line)) {
        //поиск первого непробельного симв
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;//пропуск пустая строка
        if (line[start] == '#') continue;

        size_t end = line.find_last_not_of(" \t");//обрез пробел
        std::string trimmed = line.substr(start, end - start + 1);

        std::stringstream ss(trimmed);//строка в поток
        std::string type;
        ss >> type;//тип команды
        if (type.empty()) continue;
        //сбор параметров
        std::vector<std::string> params;//вектор для параметров
        std::string param;//тек
        while (ss >> param) {//до конца строки
            params.push_back(param);
        }

        Command cmd;//создаю команду
        cmd.type = type;
        cmd.params = params;
        commands.push_back(cmd);//доб в общ список
    }
    if (commands.empty()) {
        throw "ConfigParser::parse: Файл конфигурации пуст или не содержит правильных команд";
    }
    return commands;//возврат всех команд
}