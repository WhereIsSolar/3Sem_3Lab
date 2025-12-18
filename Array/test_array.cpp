#define BOOST_TEST_MODULE DynamicArrayTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "Array.h"
#include <fstream>
#include <cstdio>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(test_constructor_destructor) {
    DynamicArray arr1;
    BOOST_CHECK(arr1.getSize() == 0);
    BOOST_CHECK(arr1.getCapacity() >= 10);
    BOOST_CHECK(arr1.checkIntegrity() == true);
    
    DynamicArray arr2(5);
    BOOST_CHECK(arr2.getCapacity() >= 10); // MIN_CAPACITY
    
    DynamicArray arr3(50);
    BOOST_CHECK(arr3.getCapacity() == 50);
}

BOOST_AUTO_TEST_CASE(test_add_end) {
    DynamicArray arr;
    
    arr.addEnd("first");
    BOOST_CHECK(arr.getSize() == 1);
    BOOST_CHECK(arr.get(0) == "first");
    
    arr.addEnd("second");
    BOOST_CHECK(arr.getSize() == 2);
    BOOST_CHECK(arr.get(0) == "first");
    BOOST_CHECK(arr.get(1) == "second");
    
    // Проверка расширения capacity
    for (int i = 0; i < 20; i++) {
        arr.addEnd("item_" + std::to_string(i));
    }
    BOOST_CHECK(arr.getSize() == 22);
    BOOST_CHECK(arr.checkIntegrity() == true);
}

BOOST_AUTO_TEST_CASE(test_add_at) {
    DynamicArray arr;
    
    BOOST_CHECK(arr.addAt(0, "first") == true);
    BOOST_CHECK(arr.getSize() == 1);
    BOOST_CHECK(arr.get(0) == "first");
    
    BOOST_CHECK(arr.addAt(0, "new_first") == true);
    BOOST_CHECK(arr.getSize() == 2);
    BOOST_CHECK(arr.get(0) == "new_first");
    BOOST_CHECK(arr.get(1) == "first");
    
    BOOST_CHECK(arr.addAt(2, "last") == true);
    BOOST_CHECK(arr.get(2) == "last");
    
    BOOST_CHECK(arr.addAt(5, "invalid") == false);
    BOOST_CHECK(arr.addAt(-1, "invalid") == false);
}

BOOST_AUTO_TEST_CASE(test_get_set) {
    DynamicArray arr;
    arr.addEnd("A");
    arr.addEnd("B");
    arr.addEnd("C");
    
    BOOST_CHECK(arr.get(0) == "A");
    BOOST_CHECK(arr.get(1) == "B");
    BOOST_CHECK(arr.get(2) == "C");
    
    BOOST_CHECK_THROW(arr.get(-1), std::out_of_range);
    BOOST_CHECK_THROW(arr.get(3), std::out_of_range);
    
    BOOST_CHECK(arr.set(1, "X") == true);
    BOOST_CHECK(arr.get(1) == "X");
    
    BOOST_CHECK(arr.set(-1, "Y") == false);
    BOOST_CHECK(arr.set(3, "Y") == false);
}

BOOST_AUTO_TEST_CASE(test_remove_at) {
    DynamicArray arr;
    arr.addEnd("A");
    arr.addEnd("B");
    arr.addEnd("C");
    arr.addEnd("D");
    
    BOOST_CHECK(arr.removeAt(1) == true);
    BOOST_CHECK(arr.getSize() == 3);
    BOOST_CHECK(arr.get(0) == "A");
    BOOST_CHECK(arr.get(1) == "C");
    BOOST_CHECK(arr.get(2) == "D");
    
    BOOST_CHECK(arr.removeAt(0) == true);
    BOOST_CHECK(arr.getSize() == 2);
    BOOST_CHECK(arr.get(0) == "C");
    
    BOOST_CHECK(arr.removeAt(5) == false);
    BOOST_CHECK(arr.removeAt(-1) == false);
    
    // Проверка shrink
    for (int i = 0; i < 100; i++) arr.addEnd("temp");
    int cap_before = arr.getCapacity();
    for (int i = 0; i < 90; i++) arr.removeAt(0);
    BOOST_CHECK(arr.getCapacity() < cap_before);
}

BOOST_AUTO_TEST_CASE(test_clear) {
    DynamicArray arr;
    for (int i = 0; i < 50; i++) arr.addEnd("item");
    
    arr.clear();
    BOOST_CHECK(arr.getSize() == 0);
    BOOST_CHECK(arr.getCapacity() >= 10);
    BOOST_CHECK(arr.checkIntegrity() == true);
    
    // После clear можно снова добавлять
    arr.addEnd("new");
    BOOST_CHECK(arr.getSize() == 1);
    BOOST_CHECK(arr.get(0) == "new");
}

BOOST_AUTO_TEST_CASE(test_file_operations) {
    DynamicArray arr1;
    arr1.addEnd("File");
    arr1.addEnd("Test");
    arr1.addEnd("Data");
    
    arr1.writeToFile("test_array.txt");
    
    DynamicArray arr2;
    arr2.readFromFile("test_array.txt");
    
    BOOST_CHECK(arr2.getSize() == 3);
    BOOST_CHECK(arr2.get(0) == "File");
    BOOST_CHECK(arr2.get(1) == "Test");
    BOOST_CHECK(arr2.get(2) == "Data");
    
    std::remove("test_array.txt");
    
    DynamicArray arr3;
    arr3.readFromFile("nonexistent.txt");
    BOOST_CHECK(arr3.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(test_binary_serialization) {
    DynamicArray original;
    original.addEnd("first");
    original.addEnd("second");
    original.addEnd("third");
    
    BOOST_CHECK(original.serializeToBinary("test_bin.bin") == true);
    
    DynamicArray loaded;
    BOOST_CHECK(loaded.deserializeFromBinary("test_bin.bin") == true);
    
    BOOST_CHECK(loaded.getSize() == 3);
    BOOST_CHECK(loaded.get(0) == "first");
    BOOST_CHECK(loaded.get(1) == "second");
    BOOST_CHECK(loaded.get(2) == "third");
    
    std::remove("test_bin.bin");
}

BOOST_AUTO_TEST_CASE(test_binary_empty) {
    DynamicArray original;
    BOOST_CHECK(original.serializeToBinary("empty.bin") == true);
    
    DynamicArray loaded;
    BOOST_CHECK(loaded.deserializeFromBinary("empty.bin") == true);
    BOOST_CHECK(loaded.getSize() == 0);
    
    std::remove("empty.bin");
}

BOOST_AUTO_TEST_CASE(test_binary_large) {
    DynamicArray original;
    for (int i = 0; i < 1000; i++) {
        original.addEnd("item_" + std::to_string(i));
    }
    
    BOOST_CHECK(original.serializeToBinary("large.bin") == true);
    
    DynamicArray loaded;
    BOOST_CHECK(loaded.deserializeFromBinary("large.bin") == true);
    
    BOOST_CHECK(loaded.getSize() == 1000);
    for (int i = 0; i < 1000; i++) {
        BOOST_CHECK(loaded.get(i) == "item_" + std::to_string(i));
    }
    
    std::remove("large.bin");
}
