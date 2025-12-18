#define BOOST_TEST_MODULE DynamicArrayBenchmark
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <chrono>
#include "Array.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(benchmark_add_end) {
    DynamicArray arr;
    const int OPS = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < OPS; i++) {
        arr.addEnd("value_" + std::to_string(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "[BENCH] addEnd (" << OPS << " ops): " << dur.count() << " ms" << std::endl;
    std::cout << "       Final size: " << arr.getSize() << ", capacity: " << arr.getCapacity() << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_add_at_beginning) {
    DynamicArray arr;
    const int OPS = 5000; // Меньше, потому что O(n) операция
    
    // Сначала добавляем несколько элементов
    for (int i = 0; i < 100; i++) {
        arr.addEnd("base_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < OPS; i++) {
        arr.addAt(0, "insert_" + std::to_string(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "[BENCH] addAt(0) (" << OPS << " ops): " << dur.count() << " ms" << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_remove_from_end) {
    DynamicArray arr;
    const int OPS = 50000;
    
    // Заполняем массив
    for (int i = 0; i < OPS; i++) {
        arr.addEnd("item_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < OPS / 2; i++) {
        arr.removeAt(arr.getSize() - 1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "[BENCH] removeAt(end) (" << OPS/2 << " ops): " << dur.count() << " ms" << std::endl;
    std::cout << "       Remaining size: " << arr.getSize() << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_get_random_access) {
    DynamicArray arr;
    const int SIZE = 100000;
    const int ACCESS_OPS = 100000;
    
    // Заполняем большой массив
    for (int i = 0; i < SIZE; i++) {
        arr.addEnd("data_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ACCESS_OPS; i++) {
        // Доступ к случайным элементам
        int index = i % SIZE;
        std::string value = arr.get(index);
        (void)value; // Чтобы избежать warning
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "[BENCH] get(random) (" << ACCESS_OPS << " ops): " << dur.count() << " µs" << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_binary_serialization) {
    DynamicArray arr;
    const int SIZE = 50000;
    
    // Заполняем массив
    for (int i = 0; i < SIZE; i++) {
        arr.addEnd("serialize_item_" + std::to_string(i));
    }
    
    // Бенчмарк сериализации
    auto start = std::chrono::high_resolution_clock::now();
    bool result = arr.serializeToBinary("bench_array.bin");
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_CHECK(result == true);
    std::cout << "[BENCH] Binary serialize (" << SIZE << " items): " << dur.count() << " ms" << std::endl;
    
    // Бенчмарк десериализации
    DynamicArray arr2;
    start = std::chrono::high_resolution_clock::now();
    result = arr2.deserializeFromBinary("bench_array.bin");
    end = std::chrono::high_resolution_clock::now();
    dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_CHECK(result == true);
    std::cout << "[BENCH] Binary deserialize (" << SIZE << " items): " << dur.count() << " ms" << std::endl;
    
    std::remove("bench_array.bin");
}

BOOST_AUTO_TEST_CASE(benchmark_capacity_growth) {
    DynamicArray arr;
    const int OPS = 100000;
    
    std::cout << "[BENCH] Capacity growth during " << OPS << " addEnd operations:" << std::endl;
    std::cout << "  Initial capacity: " << arr.getCapacity() << std::endl;
    
    int resize_count = 0;
    int last_capacity = arr.getCapacity();
    
    for (int i = 0; i < OPS; i++) {
        arr.addEnd("test");
        if (arr.getCapacity() != last_capacity) {
            resize_count++;
            std::cout << "  Resize #" << resize_count << ": capacity " << last_capacity 
                      << " -> " << arr.getCapacity() << " (size=" << arr.getSize() << ")" << std::endl;
            last_capacity = arr.getCapacity();
        }
    }
    
    std::cout << "  Final: size=" << arr.getSize() << ", capacity=" << arr.getCapacity() 
              << ", resizes=" << resize_count << std::endl;
    BOOST_CHECK(true);
}
