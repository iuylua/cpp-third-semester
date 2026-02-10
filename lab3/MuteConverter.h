//заглушает указанный интервал времени
#pragma once

#include "Converter.h"
#include "AudioStream.h"

class MuteConverter : public Converter {//наслед от converter
    size_t start_;//в семплах
    size_t end_;
public:
    MuteConverter(size_t start, size_t end);//конструктор
    void apply(const AudioStream& input, AudioStream& output) const override;//реализация;коп input в output
};