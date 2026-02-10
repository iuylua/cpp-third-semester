//созд конвертеры по типу
#pragma once

#include "Converter.h"
#include <string>
#include <vector>

class ConverterFactory {
public:
    //тип параметры список файлов
    static Converter* create(const std::string& type, const std::vector<std::string>& params, const std::vector<std::string>& input_files);
    //возврат готового конвертера нужного типа
    //static можно вызыв без созд объекта
};

//Converter* mute = ConverterFactory::create("mute", {"0", "30"}, {"input.wav"});