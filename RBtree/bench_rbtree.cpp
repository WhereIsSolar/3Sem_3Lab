#define BOOST_TEST_MODULE RBTreeBenchmark
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/chrono.hpp>
#include "RBTree.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

BOOST_AUTO_TEST_CASE(benchmark_insert_random) {
    RBTree tree;
    const int OPS = 50000;
    std::vector<int> values(OPS);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    
    for (int i = 0; i < OPS; i++) {
        values[i] = dis(gen);
    }
    
    auto start = boost::chrono::high_resolution_clock::now();
    for (int i = 0; i < OPS; i++) {
        tree.insert(values[i]);
    }
    auto end = boost::chrono::high_resolution_clock::now();
    auto dur = boost::chrono::duration_cast<boost::chrono::nanoseconds>(end - start);
    
    std::cout << "[BENCH] Insert random (" << OPS << " ops): " 
              << dur.count() / 1000000.0 << " ms" << std::endl;
    std::cout << "       Tree size: " << (tree.isEmpty() ? "empty" : "non-empty") 
              << ", RB properties: " << (tree.checkRBProperties() ? "OK" : "FAIL") << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_insert_ascending) {
    RBTree tree;
    const int OPS = 10000;
    
    auto start = boost::chrono::high_resolution_clock::now();
    for (int i = 0; i < OPS; i++) {
        tree.insert(i);
    }
    auto end = boost::chrono::high_resolution_clock::now();
    auto dur = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start);
    
    std::cout << "[BENCH] Insert ascending (" << OPS << " ops): " << dur.count() << " ms" << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_find) {
    RBTree tree;
    const int SIZE = 10000;
    const int SEARCH_OPS = 10000;
    
    for (int i = 0; i < SIZE; i++) {
        tree.insert(i * 2);
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, SIZE * 2);
    
    volatile bool dummy = false; // предотвращает оптимизацию
    
    auto start = boost::chrono::high_resolution_clock::now();
    for (int i = 0; i < SEARCH_OPS; i++) {
        dummy = tree.contains(dis(gen));
    }
    auto end = boost::chrono::high_resolution_clock::now();
    auto dur = boost::chrono::duration_cast<boost::chrono::microseconds>(end - start);
    
    (void)dummy; // подавляем предупреждение
    
    std::cout << "[BENCH] Find random (" << SEARCH_OPS << " ops): " << dur.count() << " µs" << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_remove) {
    RBTree tree;
    const int SIZE = 10000;
    
    std::vector<int> values(SIZE);
    for (int i = 0; i < SIZE; i++) {
        values[i] = i;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(values.begin(), values.end(), gen);
    
    for (int val : values) {
        tree.insert(val);
    }
    
    std::shuffle(values.begin(), values.end(), gen);
    
    auto start = boost::chrono::high_resolution_clock::now();
    for (int i = 0; i < SIZE / 2; i++) {
        tree.remove(values[i]);
    }
    auto end = boost::chrono::high_resolution_clock::now();
    auto dur = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start);
    
    std::cout << "[BENCH] Remove random (" << SIZE/2 << " ops): " << dur.count() << " ms" << std::endl;
    std::cout << "       Remaining size check: " << (tree.checkRBProperties() ? "OK" : "FAIL") << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_binary_serialization) {
    RBTree tree;
    const int SIZE = 10000;
    
    for (int i = 0; i < SIZE; i++) {
        tree.insert(i * 3);
    }
    
    auto start = boost::chrono::high_resolution_clock::now();
    bool result = tree.serializeToBinary("bench_rbtree.bin");
    auto end = boost::chrono::high_resolution_clock::now();
    auto dur = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start);
    
    BOOST_CHECK(result == true);
    std::cout << "[BENCH] Binary serialize (" << SIZE << " items): " << dur.count() << " ms" << std::endl;
    
    RBTree tree2;
    start = boost::chrono::high_resolution_clock::now();
    result = tree2.deserializeFromBinary("bench_rbtree.bin");
    end = boost::chrono::high_resolution_clock::now();
    dur = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start);
    
    BOOST_CHECK(result == true);
    std::cout << "[BENCH] Binary deserialize (" << SIZE << " items): " << dur.count() << " ms" << std::endl;
    std::cout << "       Loaded tree check: " << (tree2.checkRBProperties() ? "OK" : "FAIL") << std::endl;
    
    std::remove("bench_rbtree.bin");
}

BOOST_AUTO_TEST_CASE(benchmark_mixed_operations) {
    RBTree tree;
    const int OPS = 5000; 
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> op_dis(0, 2);
    std::uniform_int_distribution<> val_dis(1, 5000); 
    
    volatile int dummy_sum = 0;
    
    auto start = boost::chrono::high_resolution_clock::now();
    
    int insert_count = 0;
    int find_count = 0;
    int remove_count = 0;
    
    for (int i = 0; i < OPS; i++) {
        int op = op_dis(gen);
        int val = val_dis(gen);
        
        switch (op) {
            case 0:
                tree.insert(val);
                insert_count++;
                dummy_sum += val;
                break;
            case 1:
                if (tree.contains(val)) dummy_sum++;
                find_count++;
                break;
            case 2:
                if (tree.remove(val)) dummy_sum--;
                remove_count++;
                break;
        }
        
        if (i % 1000 == 0 && !tree.checkRBProperties()) {
            std::cout << "RB properties violated at iteration " << i << std::endl;
        }
    }
    
    auto end = boost::chrono::high_resolution_clock::now();
    auto dur = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start);
    
    (void)dummy_sum;
    
    std::cout << "[BENCH] Mixed operations (" << OPS << " ops): " << dur.count() << " ms" << std::endl;
    std::cout << "       Inserts: " << insert_count 
              << ", Finds: " << find_count 
              << ", Removes: " << remove_count << std::endl;
    std::cout << "       Final RB check: " << (tree.checkRBProperties() ? "OK" : "FAIL") << std::endl;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(benchmark_clear_large) {
    RBTree tree;
    const int SIZE = 10000;
    
    for (int i = 0; i < SIZE; i++) {
        tree.insert(i);
    }
    
    auto start = boost::chrono::high_resolution_clock::now();
    tree.clear();
    auto end = boost::chrono::high_resolution_clock::now();
    auto dur = boost::chrono::duration_cast<boost::chrono::milliseconds>(end - start);
    
    std::cout << "[BENCH] Clear large tree (" << SIZE << " items): " << dur.count() << " ms" << std::endl;
    BOOST_CHECK(tree.isEmpty() == true);
}
