#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>  // для vector
#include <algorithm>  // для sort

using namespace std;

//функция проверки символа
bool isLetterOrDigit(char c) {
    return (c >= 'a' && c <= 'z')|| (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}
bool sortByFreq(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;  //true - если частота первого слова больше второго
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    string input =  argv[1];
    string output = argv[2];

    //создание объекта для чтения файла
    ifstream inFile(input);
    if (!inFile) {
        return 1;
    }

    //читаем строки в список
    list<string> lines; //контейнер для хранения всех строк файла
    string line; //тек переменная для хранения одной строки
    while (getline(inFile, line)) {
        lines.push_back(line); //доб в конец
    }
    inFile.close();

    //читаем слова из строк и подсчит частоту
    map<string, int> wordCount; //слово, количество повторений
    int total_cnt = 0; //общ колво всех слов
    for (list<string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {//итератор, который ходит по списку, первый элемент списка,
        const string& line = *it; //разыменовывание ссылка на строку
        string word;
        for (size_t i = 0; i < line.length(); i++) { //по симв строки
            char x = line[i]; //тек символ
            if (isLetterOrDigit(x)) {
                word += x; //добавляем символ к текущему слову
            }
            else {
                if (!word.empty()) { //уже встречалось это слово
                    wordCount[word]++; //увел счетчик для этого слова
                    total_cnt ++;
                    word = ""; //очист для нового слова

                }
            }
        }
        if (!word.empty()) { //последнее слово и его обработка
            wordCount[word]++;
            total_cnt++;
        }
    }

    //map - сортирует по алфавиту, а надо по частоте
    vector<pair<string, int>> wordsVec; //слово,частота
    for (map<string, int>::const_iterator it = wordCount.begin(); it != wordCount.end(); ++it) {
    wordsVec.push_back(make_pair(it->first, it->second));} //делаем пару и кидаем в вектор
    //сортировка по частоте
    sort(wordsVec.begin(), wordsVec.end(), sortByFreq);
    ofstream outFile(output);  // для записи в файл
    if (!outFile) {
        return 1;
    }
    outFile << "Слово,Частота,Частота (%)" << endl;
    for (vector<pair<string, int>>::const_iterator it = wordsVec.begin(); it != wordsVec.end(); ++it) {
        float percentage = (static_cast<float>(it->second) / total_cnt) * 100.0; //меняет на дробное(частота слова / на кол-во)
        outFile << it->first << "," << it->second << "," << percentage << endl;
    }

    outFile.close();  //закрытие файла

    return 0;
}