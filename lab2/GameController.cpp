//реализация управления
#include "GameController.h"
#include <iostream>
#include <sstream>
//интерактивный режим
void GameController::runInteractive() {
    if (!universe) createDefaultUniverse();//проверяет пустой ли указатель;если вселенная не создана - созд

    std::cout << "=== ИГРА 'ЖИЗНЬ' ===\n";//вывожу заголовок игры в терминал
    displayHelp();//вызываю метод со справками по командам
    displayUniverse();//показ тек состояние вселенной

    std::string command; //объявл переменную для хранения введенной команды
    while (true) {//до команды exit
        std::cout << "> "; //вывожу приглашение для ввода команды
        std::getline(std::cin, command);//чтение строки из терминала до нажатия enter
        if (command == "exit") break;
        processCommand(command);
    }
}
//оффлайн без взаимодействия с пользователем; файл для загрузки, сохранения, сколько повторений
void GameController::runOffline(const std::string& inputFile, const std::string& outputFile, int iterations) {
    loadUniverse(inputFile);//загружаю вселенную из входного файла
    if (universe) {
        //вып несколько итераций игры;обр к объекту через указ
        universe->tick(iterations);
        saveUniverse(outputFile);//сохр рез в выходной файл
        std::cout << "Выполнено " << iterations << " итераций. Результат в " << outputFile << "\n";
    }
}
//загрузка вселенной из файла
void GameController::loadUniverse(const std::string& filename) {
    try {//т к мб ошибка
        //для загрузки: созд вселенную из файла; сохр рез в поле класса
        universe = FileHandler::loadFromFile(filename);
        std::cout << "Загружена вселенная: " << universe->getName() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки: " << e.what() << "\n";//ссылка на объект искл;текст ошибки из искл
    }
}
//созд стандартной вселенной
void GameController::createDefaultUniverse() {
    GameRules rules("B3/S23");
    universe = std::make_unique<Universe>(20, 10, rules, "Стандартная вселенная");//созд объект uni;размеры;правила;имя

    // Планер
    universe->setCell(1, 0, true);
    universe->setCell(2, 1, true);
    universe->setCell(0, 2, true);
    universe->setCell(1, 2, true);
    universe->setCell(2, 2, true);
}
//отображение вселенной в терминале
void GameController::displayUniverse() const {//не мен объект
    if (!universe) return;

    std::cout << "\nВселенная: " << universe->getName()
              << " | Правила: " << universe->getRules().toString()
              << " | Поколение: " << universe->getGeneration()
              << " | Клеток: " << universe->countLiveCells() << "\n\n";

    for (int y = 0; y < universe->getHeight(); y++) {//двойной цикл по всем клеткам поля
        for (int x = 0; x < universe->getWidth(); x++) {
            std::cout << (universe->getCell(x, y) ? "■ " : "· ");//вывод для каждой клетки:состояние(t/f),
        }
        std::cout << "\n";//переход на новую строку
    }
    std::cout << "\n";
}
//вывод списка команд
void GameController::displayHelp() const {
    std::cout << "КОМАНДЫ:\n";
    std::cout << "  help          - справка\n";
    std::cout << "  tick [n]      - сделать n ходов\n";
    std::cout << "  t [n]         - сокращение для tick\n";
    std::cout << "  dump <file>   - сохранить в файл\n";
    std::cout << "  load <file>   - загрузить из файла\n";
    std::cout << "  exit          - выход\n\n";
}
//обработка команд
void GameController::processCommand(const std::string& command) {//разбор и выполнение введенной команды
    //созд строковый поток
    std::istringstream iss(command);//поток для чтения из строки,iss-объект потока,строка с которой работаем
    std::string cmd;//чтение из потока до пробела
    iss >> cmd;//первое слово - Команда

    if (cmd == "help") displayHelp(); //показываем справку
    else if (cmd == "tick" || cmd == "t") {
        int n = 1;//по умол 1 итерация
        if (iss >> n && n > 0) {//читаем число из потока и обяз положительное
            universe->tick(n);//выполн n итераций
            displayUniverse();//вывод результата
        }
    }
    else if (cmd == "dump") {//если команда сохранение
        //читаем имя файла и сохр
        std::string filename;
        if (iss >> filename) saveUniverse(filename); //чит след слово(имя файла)
        //dump game.life > filename=game.life
    }
    else if (cmd == "load") {//загрузка
        std::string filename;
        if (iss >> filename) {
            loadUniverse(filename);
            if (universe) displayUniverse();
        }
    }
    else std::cout << "Неизвестная команда. Напишите 'help'\n";
}
//сохранение вселенной в файл
void GameController::saveUniverse(const std::string& filename) const {
    if (!universe) {
        std::cout << "Нет вселенной для сохранения!\n";
        return;
    }
    try {
        //выз st метод для сохранения;разымен указ(получ объект)
        FileHandler::saveToFile(filename, *universe);
        std::cout << "Сохранено в: " << filename << "\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка сохранения: " << e.what() << "\n";
    }
}