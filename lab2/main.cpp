#include "GameController.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    GameController game;//выд память,выз конструктор gamecontroller,созд поле universe=nullptr,настеке

    if (argc == 1) {//без аргументов
        game.runInteractive();//интерактивный режим
    }
    else if (argc == 2) {//щзагрузка файла+интеракт режим
        game.loadUniverse(argv[1]);//выз метод загрузки
        game.runInteractive();
    }
    else { //без взаимодействия
        std::string inputFile, outputFile;//вход выход кол-во итераций
        int iterations = 1;

        for (int i = 1; i < argc; i++) {//по всем арг командной строки
            std::string arg = argv[i];//преобраз в С-строку(массив char) из аргументов
            if (arg == "-i" && i + 1 < argc) iterations = std::stoi(argv[++i]);//тек эл=i и есть след
            //увел i и берем след арг;stoi - преоб строку в число
            else if (arg == "-o" && i + 1 < argc) outputFile = argv[++i];//чит след аргумент как имя выход файла
            else if (arg.find("--iterations=") == 0) iterations = std::stoi(arg.substr(13));//ищет подстроку в аргументе
            //проверка, что найдено в начале строки == 0
            //извлекаю число из строки(нач с 13 символадлинаiteratinons=13)
            //аналог дл output
            else if (arg.find("--output=") == 0) outputFile = arg.substr(9);
            //агрумент не опция = выходн файл(не начинается с - --)
            else inputFile = arg;
        }
        //оба файла не пустые
        if (!inputFile.empty() && !outputFile.empty()) {
            game.runOffline(inputFile, outputFile, iterations);
        }
    }

    return 0;
}
// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.