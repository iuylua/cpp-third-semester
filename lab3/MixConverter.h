//конвертер для смешивания двух аудиопотоков(осн с доп с указ позиции)
#pragma once

#include "Converter.h"
#include "AudioStream.h"

class MixConverter : public Converter { //наслед Converter
    size_t offset_; //смещение в семплах (где начинать смешивание)
    std::vector<int16_t> mix_samples_;//семплы второго трека
public:
    MixConverter(size_t offset, AudioStream mixStream);//конструктор
    void apply(const AudioStream& input, AudioStream& output) const override;
};








//правильное переопред вирт метода из род класса; в классе converter есть вирт метод apply

//конвертер: смешивать со вторым треком с 10-й секунды
//MixConverter mixer(10 * 44100, secondTrack);

// Применение: каждый сэмпл = среднее основного и дополнительного Берет два трека, складывает их сэмплы и усредняет, создавая эффект одновременного звучания.
//mixer.apply(mainTrack, resultTrack);