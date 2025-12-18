#include "Array.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

const int DynamicArray::MIN_CAPACITY = 10;

void DynamicArray::ensureCapacity() {
    if (size >= capacity) {
        int newCap = (capacity > 0) ? capacity * 2 : MIN_CAPACITY;
        std::string* newData = new std::string[newCap];
        if (data && size > 0) {
            std::copy(data, data + size, newData);
        }
        delete[] data;
        data = newData;
        capacity = newCap;
    }
}

void DynamicArray::shrinkCapacity() {
    if (capacity > MIN_CAPACITY && size < capacity / 4) {
        int newCap = std::max(capacity / 2, MIN_CAPACITY);
        std::string* newData = new std::string[newCap];
        if (data && size > 0) {
            std::copy(data, data + size, newData);
        }
        delete[] data;
        data = newData;
        capacity = newCap;
    }
}

DynamicArray::DynamicArray(int initialCapacity) 
    : size(0), capacity(std::max(initialCapacity, MIN_CAPACITY)) {
    data = new std::string[capacity];
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

void DynamicArray::addEnd(const std::string& val) {
    ensureCapacity();
    data[size++] = val;
}

bool DynamicArray::addAt(int index, const std::string& val) {
    if (index < 0 || index > size) return false;
    ensureCapacity();
    for (int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = val;
    size++;
    return true;
}

std::string DynamicArray::get(int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

bool DynamicArray::set(int index, const std::string& val) {
    if (index < 0 || index >= size) return false;
    data[index] = val;
    return true;
}

bool DynamicArray::removeAt(int index) {
    if (index < 0 || index >= size) return false;
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
    shrinkCapacity();
    return true;
}

int DynamicArray::getSize() const { return size; }
int DynamicArray::getCapacity() const { return capacity; }

// Файловые операции
void DynamicArray::readFromFile(const std::string& filename) {
    clear();
    std::ifstream in(filename);
    if (!in.is_open()) return;
    
    std::string val;
    while (in >> val) {
        addEnd(val);
    }
    in.close();
}

void DynamicArray::writeToFile(const std::string& filename) {
    std::ofstream out(filename);
    for (int i = 0; i < size; i++) {
        out << data[i];
        if (i < size - 1) out << " ";
    }
    out.close();
}

// Бинарная сериализация
bool DynamicArray::serializeToBinary(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) return false;
    
    // Записываем размер массива
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    
    // Записываем каждый элемент
    for (int i = 0; i < size; i++) {
        size_t len = data[i].size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(data[i].c_str(), len);
    }
    
    out.close();
    return true;
}

bool DynamicArray::deserializeFromBinary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) return false;
    
    clear();
    
    // Читаем размер массива
    int savedSize = 0;
    in.read(reinterpret_cast<char*>(&savedSize), sizeof(savedSize));
    
    // Читаем каждый элемент
    for (int i = 0; i < savedSize; i++) {
        size_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        
        std::string value(len, '\0');
        in.read(&value[0], len);
        
        addEnd(value);
    }
    
    in.close();
    return true;
}

void DynamicArray::clear() {
    delete[] data;
    data = new std::string[MIN_CAPACITY];
    size = 0;
    capacity = MIN_CAPACITY;
}

void DynamicArray::print() const {
    for (int i = 0; i < size; i++) {
        std::cout << data[i];
        if (i < size - 1) std::cout << " ";
    }
    std::cout << std::endl;
}

bool DynamicArray::checkIntegrity() const {
    return data != nullptr && size >= 0 && size <= capacity && capacity >= MIN_CAPACITY;
}
