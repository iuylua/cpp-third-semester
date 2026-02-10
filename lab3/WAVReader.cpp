#include "WAVReader.h"
#include <cstring>
#include <cstdint>
#include <iostream>

AudioStream WAVReader::read(const std::string& filename) {
    auto file = std::make_unique<std::ifstream>(filename, std::ios::binary);
    if (!file->is_open()) {
        throw "WAVReader::read: Не удалось открыть файл: " + filename;
    }

    //Чтение заголовка RIFF
    char header[4];
    file->read(header, 4);
    if (std::strncmp(header, "RIFF", 4) != 0) {
        throw "WAVReader::read: Файл не является WAV";
    }

    //Пропускаем размер файла
    file->ignore(4);

    //Проверка WAVE
    file->read(header, 4);
    if (std::strncmp(header, "WAVE", 4) != 0) {
        throw "WAVReader::read: Файл не является WAVE";
    }

    size_t data_offset = 0;//позиция начала данных
    size_t data_size = 0;//размер данных в байтах
    bool fmt_found = false;//нашли fmt чанк??

    //Поиск чанков в файле
    while (file->good()) {
        file->read(header, 4);//читаем идентификатор чанка
        if (!file->good()) break;

        uint32_t chunk_size;//размер чанка
        file->read(reinterpret_cast<char*>(&chunk_size), 4);//чит размер
        //инфо о формате
        if (std::strncmp(header, "fmt ", 4) == 0) {
            //Пропускаем fmt чанк (нам нужны только данные)
            file->ignore(chunk_size);
            fmt_found = true;//отмечаем что нашли fmt
        }
        else if (std::strncmp(header, "data", 4) == 0) {
            if (!fmt_found) throw "WAVReader::read: fmt чанк не найден перед data;ЧАНК data - АУДИОДАННЫЕ";

            data_offset = file->tellg();//запоминаем начало данных и размер
            data_size = chunk_size;
            break;
        }
        else {
            //Пропускаем другие чанки
            file->ignore(chunk_size);
        }
    }

    if (data_size == 0) {
        throw "WAVReader::read: data чанк не найден";
    }

    size_t num_samples = data_size / 2;  //16-bit = 2 байта на семпл

    std::cout << "Файл " << filename << ": " << num_samples
              << " семплов (режим потокового чтения)" << std::endl;

    //Возвращаем AudioStream для потокового чтения
    return AudioStream(std::move(file), data_offset, num_samples);
}