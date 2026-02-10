//потоковое представление аудиоданных(работа с данными из файла с буферизацией)
#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

struct AudioStream {
    std::unique_ptr<std::ifstream> file;
    size_t data_offset;  //смещение до начала аудиоданных в файле
    size_t num_samples;
    //буфер для эффективного чтения блоками (mutable = можно менять в константных методах)
    mutable std::vector<int16_t> buffer;  //хранит блок аудиосемплов для быстрого доступа
    mutable size_t current_position;


    static constexpr int sample_rate = 44100;//константа частота для аудио

    // Конструктор для потокового чтения
    AudioStream(std::unique_ptr<std::ifstream> f, size_t offset, size_t samples_count) :
        file(std::move(f)), //передаем владение файлом
        data_offset(offset), //начало аудиоданных
        num_samples(samples_count), //общее кол-во семплов
        current_position(0) {
        buffer.reserve(8192);  //буфер резерв (8192 сэмплов)
    }

    // Конструктор для пустого потока (для записи результата)
    AudioStream() : data_offset(0), num_samples(0), current_position(0) {
        buffer.reserve(8192); //заранее выделяем память для буфера
    }

    // Чтение одного семпла
    int16_t read_sample(size_t index) const {
        if (index >= num_samples) return 0;//выход за границы

        // Если семпл уже в буфере - возвращаем его
        if (index >= current_position && index < current_position + buffer.size()) {
            return buffer[index - current_position];//выч смещение в буфере
        }

        // Загрузка новый блока семплов
        read_block(index);

        //вернуть семпл из нового буфера
        if (index >= current_position && index < current_position + buffer.size()) {
            return buffer[index - current_position];
        }

        return 0;
    }

    // Чтение блока семплов в буфер
    void read_block(size_t start_index) const {
        if (!file) return;//файл не открыт

        buffer.clear();//очищаем старый
        current_position = start_index;//обновл позицию

        //Вычисляем позицию в файле в байтах
        std::streampos pos = data_offset + start_index * 2;//*2 т.к. 16-бит = 2 байта
        file->seekg(pos);//перемещаем указатель чтения

        // Читаем блок (8192 семплов или до конца файла)
        size_t samples_to_read = std::min(static_cast<size_t>(8192), num_samples - start_index);


        buffer.reserve(samples_to_read);

        // Читаем семплы по одному
        for (size_t i = 0; i < samples_to_read; ++i) {
            char buf[2];//буфер для 2 байт
            file->read(buf, 2);
            if (file->gcount() != 2) break;//если прочитали меньше 2 байт

            // Преобразование байтов в 16-битное число
            int16_t sample = static_cast<int16_t>(
                static_cast<uint16_t>(static_cast<unsigned char>(buf[0])) |
                (static_cast<uint16_t>(static_cast<unsigned char>(buf[1])) << 8)
            );
            buffer.push_back(sample);  // добавляем в буфер
        }
    }

    // Потоковое чтение диапазона
    std::vector<int16_t> read_range(size_t start, size_t end) const {
        std::vector<int16_t> result;
        size_t actual_end = std::min(end, num_samples);

        if (start >= actual_end) {
            return result;//пустой результат
        }

        result.reserve(actual_end - start);//заранее выделяем память

        // Читаем семплы по одному
        for (size_t i = start; i < actual_end; ++i) {
            result.push_back(read_sample(i));
        }

        return result;
    }

    // Чтение всех семплов (используется только для небольших файлов при смешивании)
    std::vector<int16_t> read_all() const {
        if (num_samples > 1000000) {  // если больше ~22 секунд (1,000,000 / 44,100 ≈ 22.7)
            std::cerr << "Предупреждение: чтение большого файла целиком в память ("
                      << num_samples << " семплов)" << std::endl;
        }
        return read_range(0, num_samples);//читаем весь файл
    }

    // Добавление семпла (для записи результата обработки)
    void add_sample(int16_t sample) {
        buffer.push_back(sample);//добавляем в конец буфера
        num_samples++;//увеличиваем счетчик
    }

    // Добавление нескольких семплов
    void add_samples(const std::vector<int16_t>& new_samples) {
        buffer.insert(buffer.end(), new_samples.begin(), new_samples.end());//вставляем в конец
        num_samples += new_samples.size();//обновляем счетчик
    }

    // Получение всех данных для записи в файл
    const std::vector<int16_t>& get_samples() const {
        return buffer;//возвращаем буфер с результатами
    }

    // Очистка потока
    void clear() {
        buffer.clear();
        num_samples = 0;
        file.reset();
        data_offset = 0;//сбрасываем смещение
        current_position = 0;//сбрасываем позицию
    }

    size_t size() const { return num_samples; }//возвращаем количество семплов
    bool empty() const { return num_samples == 0; }//проверка на пустоту
};