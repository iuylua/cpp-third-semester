#ifndef BITARRAY_H //предотвращает множественнъе вкл файла
#define BITARRAY_H

#include <string>
#include <stdexcept>


class BitArray {
private: //внутри класса доступны:
    unsigned long* data_; //указ на массвв
    int size_; //колво бит в массиве
    int capacity_; //колво выд блоков

    //метод для расчета мощности
    int calculate_capacity(int num_bits) const {
        if (num_bits == 0) return 0;
        const int ULONG_BITS = sizeof(unsigned long) * 8;
        return (num_bits + ULONG_BITS - 1) / ULONG_BITS; //округ вверх
    }

    //проверка индекса
    void validate_index(int index) const {
        if (index < 0 || index >= size_) {
            throw std::invalid_argument("Индекс вне диапазона");
        }
    }

public:
    //КОНСТРУКТОР (создает пустой битовый массив)
    BitArray();

    //ДЕСТРУКТОР (освоб память, когда объект уничтожается)
    ~BitArray();

    //КОНСТРУКТОР С ПАРАМЕТРАМИ
    BitArray(int num_bits, unsigned long value = 0);

    //КОНСТРУКТОР КОПИРОВАНИЯ; создает новый объект как копию существующего
    BitArray(const BitArray& b);

    //ОПЕРАТОР ПРИСВАИВАНИЯ; копирует данные в уже существующий объект
    BitArray& operator=(const BitArray& b);

    //МЕТОД SWAP
    void swap(BitArray& b);

    //МЕТОД RESIZE
    void resize(int num_bits, bool value = false);

    //ОЧИСТКА
    void clear();

    //ДОБАВЛЕНИЕ ОДНОГО БИТА
    void push_back(bool bit);

    //БИТОВЫЕ ОПЕРАЦИИ С ПРИСВАИВАНИЕМ
    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);

    //БИТОВЫЕ СДВИГИ С ПРИСВАИВАНИЕМ
    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);

    //СДВИГИ БЕЗ ПРИСВАИВАНИЯ
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    //РАБОТА С ОТДЕЛЬНЫМИ БИТАМИ
    BitArray& set(int n, bool val = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();

    //ПРОВЕРКИ СОСТОЯНИЯ
    bool any() const;
    bool none() const;
    int count() const;

    //ОПЕРАТОРЫ
    BitArray operator~() const;
    bool operator[](int i) const;
    int size() const;
    bool empty() const;
    std::string to_string() const;

    //ПРОВЕРКА НА ОДИН ДЛИНУ (вспомогательный метод)
    void same_size(const BitArray& b) const;
};

//ВНЕШНИЕ ОПЕРАТОРЫ
bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);
BitArray operator&(const BitArray& a, const BitArray& b);
BitArray operator|(const BitArray& a, const BitArray& b);
BitArray operator^(const BitArray& a, const BitArray& b);

#endif
