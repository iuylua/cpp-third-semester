#pragma once
#include "AudioStream.h"
//шаблон для всех конвентеров

class Converter {
public:
    virtual ~Converter() = default;//деструктор
    //метод реализ все конвертеры
    virtual void apply(const AudioStream& input, AudioStream& output) const = 0;//вход и выход поток;чисто виртуальный должен быть реал на наследниках
    //созд и возвращ новый AudioStream с рез
    AudioStream apply(const AudioStream& input) const {
        AudioStream output;
        apply(input, output);
        return output;
    }
};