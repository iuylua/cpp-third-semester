#include "MuteConverter.h"
#include <algorithm>

MuteConverter::MuteConverter(size_t start, size_t end)//начало и конец интервала тишины в семплах
    : start_(start), end_(end) {
}

void MuteConverter::apply(const AudioStream& input, AudioStream& output) const {//константная ссылка для чтения и ссылка для записи
    output.clear();//очищаем выходн поток

    for (size_t i = 0; i < input.size(); ++i) {
        int16_t sample = input.read_sample(i);//читаем семпл (потоково)

        if (i >= start_ && i < end_) {//попадает ли в интервал???
            sample = 0;//устанавливаем его значение в 0
        }

        output.add_sample(sample);//добавляем семпл в рез
    }
}