//способ сохр аудио в wavформате
//запис аудио в wavфайл на диск
#pragma once

#include "AudioStream.h"
#include <string>
#include <fstream>
#include <vector>

class WAVWriter {
public:
    //запись готового объекта AudioStream в файл(числа и инфо о них;заголовок)
    static void write(const std::string& filename, const AudioStream& stream);
    //числа без методанных
    static void write(const std::string& filename, const std::vector<int16_t>& samples);//запись заголовкасемплы в файл
};