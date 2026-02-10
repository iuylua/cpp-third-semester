//парсинг конфигурационных файлов(читает тексфайл с командами,разбивает каждую строку на имя команды и параметры
//возвращ спискок команд
#pragma once
#include <string>
#include <vector>

class ConfigParser {
public:
    struct Command {
        std::string type; //mute mix robot
        std::vector<std::string> params;//список параметров ["0","2"]
    };
    std::vector<Command> parse(const std::string& filename);//чтение config.txt
};


//mute 0 30 type = 'mute' params = '0', '30'
//Command { type: "mute", params: {"0", "30"} }