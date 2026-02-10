#include "RobotVoiceConverter.h"
#include <cmath>
#include <algorithm>

RobotVoiceConverter::RobotVoiceConverter(double freq, size_t start_seconds, size_t end_seconds){
    frequency_ = freq;
    start_ = start_seconds * AudioStream::sample_rate;//переводим секунды в семплы
    end_ = end_seconds * AudioStream::sample_rate;//когда начать и закончить эффект
}

void RobotVoiceConverter::apply(const AudioStream& input, AudioStream& output) const {
    output.clear();//очищаем выход

    //Если end_ = 0, обрабатываем до конца файла
    size_t actual_end = (end_ == 0) ? input.size() :
                       std::min(end_, input.size());//мин(конец в семплах, размер файл)

    double phase_step = 2.0 * M_PI * frequency_ / AudioStream::sample_rate;//шаг фазы для синусоиды

    for (size_t i = 0; i < input.size(); ++i) {//цикл по всем семплам
        int16_t sample = input.read_sample(i);//читаем семпл

        if (i >= start_ && i < actual_end) {//в интервале??
            double phase = i * phase_step;//вычисляем текущую фазу(номер сэмпла * шаг фазы)
            double modulation = std::sin(phase);//значение синуса для текущей фазы
            int32_t modulated = static_cast<int32_t>(sample * modulation);//умножаем сэмпл на значение синуса
            //Если modulation = 1 → звук без изменений
            //Если modulation = 0 → тишина
            //Если modulation = -1 → инвертированный звук


            if (modulated > 32767) modulated = 32767;
            if (modulated < -32768) modulated = -32768;

            sample = static_cast<int16_t>(modulated);
        }

        output.add_sample(sample);//добавляем обработанный семпл
    }
}