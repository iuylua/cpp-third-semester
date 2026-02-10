#include "BitArray.h"
#include <stdexcept>
#include <algorithm>

const int ULONG_BITS = sizeof(unsigned long) * 8; //выч сколько бит в unsigned long

//КОНСТРУКТОР (создает пустой битовый массив)
BitArray::BitArray() {
    this->data_ = nullptr; //указ на массив, память не выделена
    this->size_ = 0; //колво бит в массиве
    this->capacity_ = 0; //колво памяти(в бл)
}

//ДЕСТРУКТОР (освоб память, когда объект уничтожается)
BitArray::~BitArray() {
    delete[] this->data_;
}

//КОНСТРУКТОР С ПАРАМЕТРАМИ
BitArray::BitArray(int num_bits, unsigned long value) { //созд массив из num_bits, иниц значением value
    if (num_bits < 0) {
        throw std::invalid_argument("Число битов не может быть отрицательным");//оператор исключения - бросает ошибку
    }
    this->size_ = num_bits;
    this->capacity_ = calculate_capacity(num_bits);//сколько битов нужно хранить и блоков памяти
    if (num_bits == 0) {//если 0 битов(обработка пустого массива - выход)
        this->data_ = nullptr;
        return;
    }
    //выделение памяти разм capacity
    this->data_ = new unsigned long[capacity_](); // () инициализирует нулями
    //инициализация значением
    if (this->capacity_ > 0) { //если память выделена - записываем переданное знач value в первую ячейку
        this->data_[0] = value;
        //обрез последний неполный блок бит ЛИШНИЙ
        if (this->size_ > 0) {
            int last_block = (this->size_ - 1) / ULONG_BITS;
            int bits_in_last_block = this->size_ % ULONG_BITS;
            if (bits_in_last_block > 0) {
                unsigned long mask = (1UL << bits_in_last_block) - 1; // 64 бита сдвигаем влево на биты в последнем блоке - 1(типо инвертирует)
                this->data_[last_block] &= mask; //побитовое и присваивание; ост только те биты, где в маске 1, ост обнуляет
            }
        }
    }
}

//КОНСТРУКТОР КОПИРОВАНИЯ, выд новую память
BitArray::BitArray(const BitArray& b){
    this->size_ = b.size_;
    this->capacity_ = b.capacity_;
    this->data_ = nullptr;

    if (b.data_ != nullptr && this->capacity_ > 0) {
        this->data_ = new unsigned long[this->capacity_]; //выд свою память
        for (int i = 0; i < this->capacity_; ++i) {
            this->data_[i] = b.data_[i]; //копирую данные
        }
    }
}

//МЕТОД SWAP
void BitArray::swap(BitArray& b) {
    std::swap(this->data_, b.data_); //указ на тек объект и  на b
    std::swap(this->size_, b.size_);
    std::swap(this->capacity_, b.capacity_);
}

//ОПЕРАТОР ПРИСВАИВАНИЯ(a = b - теперь a содержит копию b)
BitArray& BitArray::operator=(const BitArray& b){ //присваивает содержимое одного масссива другому
    if (this == &b){//чтоб сам себя не присвоил
        return *this;
    }
    delete[] this->data_; //удал старую память, выд новую, копируем данные
    this->size_ = b.size_;
    this->capacity_ = b.capacity_;
    this->data_ = nullptr;

    if (b.data_ != nullptr && this->capacity_ > 0) {
        this->data_ = new unsigned long[this->capacity_];
        for (int i = 0; i < this->capacity_; i++){
            this->data_[i] = b.data_[i]; //копир данные
        }
    }
    return *this;
}

//МЕТОД RESIZE - изменяет размер массива; выд доп память/обрез
void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) {
        throw std::invalid_argument("Число битов не может быть отрицательным");
    }
    //размер не изменился
    if (num_bits == this->size_) {
        // Если значение 1 - устанавливаем все биты в value, иначе сбрасываем
        if (value) {
            this->set();
        } else {
            this->reset();
        }
        return;
    }

    //увеличение
    if (num_bits > this->size_) {
        int old_size = this->size_; //старый размер
        int new_capacity = calculate_capacity(num_bits); //выч новую

        if (new_capacity > this->capacity_) { //если нужно больше памяти
            unsigned long* new_data = new unsigned long[new_capacity](); //выд новую память
            if (this->data_ != nullptr) {
                for (int i = 0; i < this->capacity_; i++) {
                    new_data[i] = this->data_[i]; //коп старые данные
                }
                delete[] this->data_; //удал старую память
            }
            this->data_ = new_data; //обн указатель и мощность
            this->capacity_ = new_capacity;
        }
        //новые биты в value
        this->size_ = num_bits;
        for (int i = old_size; i < num_bits; i++) {
            this->set(i, value);
        }
    }
    else {//уменьшение
        this->size_ = num_bits;
        // Обрезаем лишние биты
        if (this->size_ > 0 && this->data_ != nullptr) {
            int last_block = (this->size_ - 1) / ULONG_BITS;
            int bits_in_last_block = this->size_ % ULONG_BITS;
            if (bits_in_last_block > 0) {
                unsigned long mask = (1UL << bits_in_last_block) - 1;
                this->data_[last_block] &= mask;
            }
        }
    }
}

//ПРОВЕРКА НА ОДИН ДЛИНУ
void BitArray::same_size(const BitArray& b) const {
    if (this->size_ != b.size_) {
        throw std::invalid_argument("Массивы должны быть одного размера");
    }
}

//ОЧИСТКА обн размер
void BitArray::clear() {
    this->size_ = 0;
    // Не освобождаем память, чтобы можно было использовать повторно
    if (this->data_ != nullptr && this->capacity_ > 0) {
        for (int i = 0; i < this->capacity_; i++) {
            this->data_[i] = 0;
        }
    }
}

//ДОБАВЛЕНИЕ ОДНОГО БИТА
void BitArray::push_back(bool bit) {
    resize(this->size_ + 1, bit);
}

//ОПЕРАТОР ПОБИТОВОГО И
BitArray& BitArray::operator&=(const BitArray& b){
    same_size(b);

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] &= b.data_[i];
    }

    return *this;
}

//ОПЕРАТОР ПОБИТОВОГО ИЛИ
BitArray& BitArray::operator|=(const BitArray& b){
    same_size(b); //один размер

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] |= b.data_[i]; //побитовое и к каждому блоку
    }

    return *this;
}

//ОПЕРАТОР ИСКЛЮЧАЮЩЕГО ИЛИ
BitArray& BitArray::operator^=(const BitArray& b){
    same_size(b);

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] ^= b.data_[i];
    }

    return *this;
}

//БИТОВЫЕ СДВИГИ
//СДВИГ ВЛЕВО с заполнением нулями с присваиванием
//СДВИГ ВЛЕВО с заполнением нулями с присваиванием
BitArray& BitArray::operator<<=(int n){
    if (n < 0){
        throw std::invalid_argument("Число сдвига не может быть отрицательным");
    }
    if (n == 0) return *this;

    if (n >= this->size_) { //если сдвиг больше размера - обнуляем
        this->reset();
        return *this;
    }

    // Улучшенный сдвиг - обрабатываем блоки правильно
    int block_shift = n / ULONG_BITS;
    int bit_shift = n % ULONG_BITS;

    if (block_shift > 0) {
        // Сдвиг целыми блоками
        for (int i = 0; i < capacity_ - block_shift; i++) {
            data_[i] = data_[i + block_shift];
        }
        // Заполняем освободившиеся блоки нулями
        for (int i = capacity_ - block_shift; i < capacity_; i++) {
            data_[i] = 0;
        }
    }

    if (bit_shift > 0) {
        // Сдвиг битов между блоками
        for (int i = 0; i < capacity_ - 1; i++) {
            data_[i] = (data_[i] << bit_shift) | (data_[i + 1] >> (ULONG_BITS - bit_shift));
        }
        // Последний блок
        data_[capacity_ - 1] <<= bit_shift;
    }

    // Обрезаем лишние биты в последнем блоке
    if (size_ > 0) {
        int last_block = (size_ - 1) / ULONG_BITS;
        int bits_in_last_block = size_ % ULONG_BITS;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            data_[last_block] &= mask;
        }
    }

    return *this;
}

//СДВИГ ВПРАВО
BitArray& BitArray::operator>>=(int n){
    if (n < 0){
        throw std::invalid_argument("Число сдвига не может быть отрицательным");
    }
    if (n == 0) return *this;

    if (n >= this->size_) { //если сдвиг больше размера - обнуляем
        this->reset();
        return *this;
    }

    // Улучшенный сдвиг
    int block_shift = n / ULONG_BITS;
    int bit_shift = n % ULONG_BITS;

    if (block_shift > 0) {
        // Сдвиг целыми блоками вправо
        for (int i = capacity_ - 1; i >= block_shift; i--) {
            data_[i] = data_[i - block_shift];
        }
        // Заполняем освободившиеся блоки нулями
        for (int i = 0; i < block_shift; i++) {
            data_[i] = 0;
        }
    }

    if (bit_shift > 0) {
        // Сдвиг битов между блоками
        for (int i = capacity_ - 1; i > 0; i--) {
            data_[i] = (data_[i] >> bit_shift) | (data_[i - 1] << (ULONG_BITS - bit_shift));
        }
        // Первый блок
        data_[0] >>= bit_shift;
    }

    // Обрезаем лишние биты в последнем блоке
    if (size_ > 0) {
        int last_block = (size_ - 1) / ULONG_BITS;
        int bits_in_last_block = size_ % ULONG_BITS;
        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            data_[last_block] &= mask;
        }
    }

    return *this;
}




//СДВИГИ БЕЗ ПРИСВАИВАНИЯ
//Битовый сдвиг влево без присваивания
BitArray BitArray::operator<<(int n) const {
    BitArray result(*this); //копирую массив чтобы не менять исходный массив
    result <<= n; //сдвиг к копии
    return result; //возврат рез(НОВОГО массива)
}

//Битовый сдвиг вправо без присваивания
BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}


//Устанавливает бит с индексом n в значение val.
BitArray& BitArray::set(int n, bool val) {
    validate_index(n); //проверка

    int block = n / ULONG_BITS; // в каком блоке нах бит
    int position = n % ULONG_BITS; //где в блоке

    if (val) {
        this->data_[block] |= (1UL << position); //установка 1 на место position в блоке data
    } else {
        this->data_[block] &= ~(1UL << position);//установка 0
    }

    return *this;
}

//заполняет массив истиной
BitArray& BitArray::set() {
    if (this->data_ == nullptr) return *this;

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] = ~0UL;  //побитовая инверсия на все 1111
    }

    //очистка последних битов
    if (this->size_ > 0) {
        int last_block = (this->size_ - 1) / ULONG_BITS;
        int bits_in_last_block = this->size_ % ULONG_BITS;

        if (bits_in_last_block > 0) {
            unsigned long mask = (1UL << bits_in_last_block) - 1;
            this->data_[last_block] &= mask;
        }
    }

    return *this;
}

//Устанавливает бит с индексом n в значение false.
BitArray& BitArray::reset(int n){
    return this->set(n,false);
}

//Заполняет массив ложью.
BitArray& BitArray::reset(){
    if (this->data_ == nullptr) return *this;

    for (int i = 0; i < this->capacity_; i++) {
        this->data_[i] = 0UL;  //все биты блока в 0
    }
    return *this;
}

//ПРОВЕРКИ СОСТОЯНИЯ
//true, если массив содержит хотя бы 1 истинный бит
bool BitArray::any() const{
    if (this->data_ == nullptr) return false;

    for (int i = 0; i < this->capacity_; i++) {
        if (this->data_[i] != 0UL) {
            return true;
        }
    }
    return false;  // все биты false
}

//true, если все биты массива ложны(0)
bool BitArray::none() const {
    return !this->any();
}

//ОПЕРАТОР ПОБИТОВОЙ ИНВЕРСИИ; меняю все биты на противоположные
BitArray BitArray::operator~() const{
    BitArray result(*this); //создаю копию массива

    if (result.data_ != nullptr) {
        for (int i = 0; i < this->capacity_; i++) {
            result.data_[i] = ~this->data_[i]; //кажд блок
        }

        //очистка последних битов
        if (this->size_ > 0) {
            int last_block = (this->size_ - 1) / ULONG_BITS;
            int bits_in_last_block = this->size_ % ULONG_BITS;
            if (bits_in_last_block > 0) {
                unsigned long mask = (1UL << bits_in_last_block) - 1;
                result.data_[last_block] &= mask;
            }
        }
    }

    return result;
}

//КОЛВО ЕДИНИЧНЫХ БИТ
int BitArray::count() const {
    int count = 0;

    for (int i = 0; i < this->size_; i++) {
        if ((*this)[i]) {
            count++;
        }
    }

    return count;
}

//Возвращает значение бита по индексу i.
bool BitArray::operator[](int i) const{
    validate_index(i); //в блоке сдвиг вправо - нужный бит получится самый младший
                        //через побитовое и с единицей получим только младший бит!
    int block = i / ULONG_BITS;
    int position = i % ULONG_BITS;
    return (this->data_[block] >> position) & 1;
}

//значение поля size_; пол размер
int BitArray::size() const{
    return this->size_;
}

//проверка на пустой массив
bool BitArray::empty() const{
    return this->size_ == 0;
}

//ПРЕОБРАЗОВАНИЕ В СТРОКУ вывод массива в строковом виде от старшего к младшему
std::string BitArray::to_string() const{
    std::string result;

    if (this->size_ == 0) {
        return result;
    }

    // вывод от старшего бита к младшему
    for (int i = this->size_ - 1; i >= 0; i--) {
        if ((*this)[i]) {
            result += '1';
        } else {
            result += '0';
        }
    }

    return result;
}

//оператор сравнения
bool operator==(const BitArray& a, const BitArray& b){
    if (a.size() != b.size()) { //разные размеры
        return false;
    }
    for (int i = 0; i < a.size(); i++) {//побитово
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

//оператор не равно
bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

//оператор побитового И без присваивания
BitArray operator&(const BitArray& a, const BitArray& b) {
    // Проверяем одинаковый размер
    if (a.size() != b.size()) {
        throw std::invalid_argument("Массивы не могут быть разного размера для &");
    }

    BitArray result(a);
    result &= b;
    return result;
}

//оператор побитового или без присваивания
BitArray operator|(const BitArray& a, const BitArray& b) {
    // Проверяем одинаковый размер
    if (a.size() != b.size()) {
        throw std::invalid_argument("Массивы не могут быть разного размера для |");
    }

    BitArray result(a);
    result |= b;
    return result;
}

//Оператор побитового исключающего или без присваивания
BitArray operator^(const BitArray& a, const BitArray& b) {
    // Проверяем одинаковый размер
    if (a.size() != b.size()) {
        throw std::invalid_argument("Массивы не могут быть разного размера для ^");
    }

    BitArray result(a);
    result ^= b;
    return result;
}