#include "MixConverter.h"
#include <algorithm>

MixConverter::MixConverter(size_t offset, AudioStream mixStream)//смещение в семплах(когда нач), второй аудио
    : offset_(offset) {
    mix_samples_ = mixStream.read_all();
}//читаем все семплы 2 аудио и возвращаем их как vector<int16_t>

void MixConverter::apply(const AudioStream& input, AudioStream& output) const {
    output.clear();//чтобы начать запись с 0

    //цикл по всем семплам входного потока
    for (size_t i = 0; i < input.size(); ++i) {
        int16_t main_sample = input.read_sample(i);//читаем i-ый семпл без изм
        int16_t result_sample = main_sample;

        //условие смешивания
        if (i >= offset_ && (i - offset_) < mix_samples_.size()) {
            //текущая позиция не меньше начала смешивания
            //индекс во втором аудио должен быть в пределах его длины
            int16_t mix_sample = mix_samples_[i - offset_];
            //смешивание(преобр в 32-бит;без переполнения)
            int32_t mixed = (static_cast<int32_t>(main_sample) + static_cast<int32_t>(mix_sample)) / 2;
            //ограничиваем результат диапазоном int16_t
            if (mixed > 32767) mixed = 32767;
            if (mixed < -32768) mixed = -32768;
            result_sample = static_cast<int16_t>(mixed);
            //преобразуем 32-битный результат обратно в 16-битный сэмпл
        }

        output.add_sample(result_sample);//добавляем в выходной поток
    }
}