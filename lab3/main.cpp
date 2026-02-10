#include "Parser.h"
#include "WAVReader.h"
#include "WAVWriter.h"
#include "ConfigParser.h"
#include "ConverterFactory.h"
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char* argv[]) {
    try {
        ProgramArgs args = parse_command_line(argc, argv);//разбор аргументов
        if (args.show_help) {//нужна справка
            print_usage();
            return 0;
        }

        std::cout << "=== Запуск Sound Processor ===" << std::endl;
        std::cout << "Режим: потоковая обработка" << std::endl;

        ConfigParser parser;//создаем объект парсера конфига
        auto commands = parser.parse(args.config_file);//парсим файл,получаем команды
        std::cout << "Команд в конфиге(вывод кол-ва): " << commands.size() << std::endl;

        AudioStream current = WAVReader::read(args.input_files[0]);//чтение первого wav-файла
        std::cout << "Загружено: " << current.size() << " семплов" << std::endl;


        for (size_t i = 0; i < commands.size(); ++i) {//цикл по всем командам
            const auto& cmd = commands[i];//ссылка на тек команду
            std::cout << "[" << (i+1) << "/" << commands.size()
                      << "] " << cmd.type;
            //вывод параметров
            for (const auto& param : cmd.params) {
                std::cout << " " << param;
            }
            std::cout << std::endl;

            //создание конвертера по типу команды
            auto converter = ConverterFactory::create(cmd.type, cmd.params, args.input_files);


            AudioStream result;//создаем пустой аудиопоток для результата
            converter->apply(current, result);//обрабатываем текущий поток и записываем в результат

            current = std::move(result);//перемещаем результат для следующей итерации(эффективно без копирования)

            delete converter;//освобождаем память
            std::cout << "  -> " << current.size() << " семплов" << std::endl;
        }

        //Запись результата
        std::cout << "Запись в файл: " << args.output_file << std::endl;
        WAVWriter::write(args.output_file, current);
        std::cout << "=== Обработка завершена ===" << std::endl;

    } catch (const char* e) {//строковые ошибки
        std::cerr << "ОШИБКА: " << e << std::endl;
        return 1;
    } catch (const std::exception& e) {//стандартные исключения
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "НЕИЗВЕСТНАЯ ОШИБКА" << std::endl;
        return 1;
    }

    return 0;
}