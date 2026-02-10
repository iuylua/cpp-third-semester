//РЕАЛИЗАЦИЯ ЛОГИКИ ИГРЫ; конструктор созд вселенной
#include "Universe.h"
//Конструктор(созд объекта, список иниц до входа)
Universe::Universe(int w, int h, const GameRules& r, const std::string& n)
    : width(w), height(h), generation(0), rules(r), name(n) {//ширина высота поколение копир объект коп строку с именем
    grid.resize(height, std::vector<bool>(width, false));//созд height строк,кажд вектор из wigth эл,кл мертвые
    //созд игровое поле заданного размера
}
//grid = [[false, false, false, ...]
// строка 0[false, false, false, ...]
 // строка 1
 // height строк

void Universe::nextGeneration() {
    //автоматическое опред типа
    auto newGrid = grid;//созд коп тек поля
    //двойной цикл по всем клеткам
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int neighbors = countNeighbors(x, y);//подсчет соседей
            bool alive = grid[y][x];//определение тек состояния живая/мертвая
            //применение правил
            if (alive) newGrid[y][x] = rules.shouldSurvive(neighbors);
            else newGrid[y][x] = rules.shouldBorn(neighbors);
        }
    }

    grid = newGrid;//замена старого поля на новое
    generation++;
}

void Universe::tick(int n) {
    for (int i = 0; i < n; i++) nextGeneration();
}

bool Universe::getCell(int x, int y) const {
    return grid[y][x];
}

void Universe::setCell(int x, int y, bool state) {
    grid[y][x] = state;
}

void Universe::clear() {
    for (auto& row : grid) std::fill(row.begin(), row.end(), false);
    generation = 0;
}

int Universe::countNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {//dx dy смещение относ тек клетки диапазон -1 0 1
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; //пропускаем когда сама клетка а не сосед
            int nx = wrapX(x + dx), ny = wrapY(y + dy);
            //заворачиваем коорд если она вышла за границы(выш слева появл справа)
            //Клетки на краю имеют меньше соседей:-Угловые:3соседа(вместо 8)
            //- Краевые:5 соседей(вместо 8) троидальное поле
            //Все клетки имеют 8 соседей:-Угловая(0, 0) считает:(1, 0), (0, 1), (1, 1) +(9, 0), (0, 9), (9, 9), (1, 9), (9, 1)
            if (grid[ny][nx]) count++; //доступ к соседней клетке
        }
    }
    return count;
}

int Universe::countLiveCells() const {
    int count = 0;
    for (const auto& row : grid)
        for (bool cell : row)
            if (cell) count++; //увел если клетка живая
    return count;
}

int Universe::wrapX(int x) const {
    if (x < 0) return width - 1; //ушел влево появл справа wrapX(-1) = 9
    if (x >= width) return 0; //ушел вправо появл слева wrapX(10) = 0
    return x; //без изменений
}

int Universe::wrapY(int y) const {
    if (y < 0) return height - 1;
    if (y >= height) return 0;
    return y;
}