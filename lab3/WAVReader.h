//читатает wav-файлы с диска и созд из них объекты audiostream
#pragma once

#include "AudioStream.h"
#include <string>
#include <fstream>

class WAVReader {
public:
    static AudioStream read(const std::string& filename);
    // метод read, который берет имя файла и возвращает готовый AudioStream
    //можно вызыв без созд объекта wavreader
};