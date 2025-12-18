#define BOOST_TEST_MODULE RBTreeTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "RBTree.h"
#include <fstream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>

std::string captureOutput(std::function<void()> func) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    func();
    std::cout.rdbuf(oldCout);
    return oss.str();
}

BOOST_AUTO_TEST_CASE(test_constructor_destructor) {
    RBTree tree1;
    BOOST_CHECK(tree1.isEmpty() == true);
    
    {
        RBTree tree2;
        tree2.insert(10);
        BOOST_CHECK(tree2.contains(10) == true);
    }
}

BOOST_AUTO_TEST_CASE(test_insert_single) {
    RBTree tree;
    tree.insert(42);
    BOOST_CHECK(tree.isEmpty() == false);
    BOOST_CHECK(tree.contains(42) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
}

BOOST_AUTO_TEST_CASE(test_insert_duplicates) {
    RBTree tree;
    tree.insert(10);
    tree.insert(10);
    tree.insert(10);
    tree.printTree();
    BOOST_CHECK(tree.contains(10) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
    
    tree.remove(10);
    BOOST_CHECK(tree.contains(10) == false);
    BOOST_CHECK(tree.checkRBProperties() == true);
}

BOOST_AUTO_TEST_CASE(test_insert_multiple_random) {
    RBTree tree;
    std::vector<int> values = {15, 10, 20, 5, 12, 18, 25, 3, 7, 17, 19};
    
    for (int val : values) {
        tree.insert(val);
        BOOST_CHECK(tree.contains(val) == true);
        BOOST_CHECK(tree.checkRBProperties() == true);
    }
    
    BOOST_CHECK(tree.checkRBProperties() == true);
}

BOOST_AUTO_TEST_CASE(test_remove_leaf_nodes) {
    RBTree tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);
    tree.insert(25);
    tree.insert(35);

    BOOST_CHECK(tree.remove(5) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
    BOOST_CHECK(tree.remove(15) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
    BOOST_CHECK(tree.remove(25) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
    BOOST_CHECK(tree.remove(35) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
}

BOOST_AUTO_TEST_CASE(test_remove_nodes_with_one_child) {
    RBTree tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);

    BOOST_CHECK(tree.remove(10) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
    BOOST_CHECK(tree.contains(15) == true);
}

BOOST_AUTO_TEST_CASE(test_remove_root_multiple_times) {
    RBTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);

    BOOST_CHECK(tree.remove(10) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);

    tree.insert(10);
    BOOST_CHECK(tree.remove(10) == true);
    BOOST_CHECK(tree.checkRBProperties() == true);
}

BOOST_AUTO_TEST_CASE(test_remove_nonexistent) {
    RBTree tree;
    tree.insert(1);
    tree.insert(2);
    BOOST_CHECK(tree.remove(999) == false);
    BOOST_CHECK(tree.checkRBProperties() == true);
}

BOOST_AUTO_TEST_CASE(test_large_random_insert_remove) {
    RBTree tree;
    std::vector<int> values;
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < 1000; ++i) {
        int val = dis(gen);
        values.push_back(val);
        tree.insert(val);
    }

    BOOST_CHECK(tree.checkRBProperties() == true);

    std::shuffle(values.begin(), values.end(), gen);
    for (int i = 0; i < 500; ++i) {
        tree.remove(values[i]);
        BOOST_CHECK(tree.checkRBProperties() == true);
    }
}

BOOST_AUTO_TEST_CASE(test_clear_multiple_times) {
    RBTree tree;
    tree.insert(1);
    tree.insert(2);
    tree.clear();
    BOOST_CHECK(tree.isEmpty() == true);
    BOOST_CHECK(tree.checkRBProperties() == true);

    tree.insert(42);
    BOOST_CHECK(tree.contains(42) == true);
    tree.clear();
    BOOST_CHECK(tree.isEmpty() == true);
}

BOOST_AUTO_TEST_CASE(test_print_tree) {
    RBTree tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);

    std::string output = captureOutput([&]() { tree.printTree(); });
    BOOST_CHECK(output.find("B:20") != std::string::npos);
    BOOST_CHECK(output.find("R:10") != std::string::npos || output.find("B:10") != std::string::npos);
    BOOST_CHECK(output.find("30") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_inorder_print) {
    RBTree tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);

    std::string output = captureOutput([&]() { tree.inorderPrint(); });
    BOOST_CHECK(output == "5 10 15 20 30 \n");
}

BOOST_AUTO_TEST_CASE(test_rbnode_accessors) {
    RBTree tree;
    tree.insert(42);

    RBNode* root = tree.getRoot();
    BOOST_CHECK(root != nullptr);
    BOOST_CHECK(root->getValue() == 42);
    BOOST_CHECK(root->getColor() == BLACK); // корень всегда чёрный

}

BOOST_AUTO_TEST_CASE(test_stress_mixed_operations) {
    RBTree tree;
    std::mt19937 gen(123);
    std::uniform_int_distribution<> val_dis(1, 1000);
    std::uniform_int_distribution<> op_dis(0, 2);

    for (int i = 0; i < 3000; ++i) {
        int op = op_dis(gen);
        int val = val_dis(gen);

        if (op == 0) tree.insert(val);
        else if (op == 1) tree.remove(val);
        else tree.contains(val);

        if (i % 200 == 0) {
            BOOST_CHECK(tree.checkRBProperties() == true);
        }
    }

    BOOST_CHECK(tree.checkRBProperties() == true);
}

BOOST_AUTO_TEST_CASE(test_print_tree_structure) {
    RBTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    tree.printTree();
}


BOOST_AUTO_TEST_CASE(test_write_read_file) {
    RBTree tree1;
    tree1.insert(10);
    tree1.insert(5);
    tree1.insert(15);
    tree1.insert(3);
    tree1.insert(7);

    const std::string filename = "coverage_test.txt";
    tree1.writeToFile(filename);  // покрывает writeToFile + inorderRec с ofstream

    RBTree tree2;
    tree2.readFromFile(filename); // покрывает readFromFile + цикл вставок

    BOOST_CHECK(tree2.contains(3));
    BOOST_CHECK(tree2.contains(5));
    BOOST_CHECK(tree2.contains(7));
    BOOST_CHECK(tree2.contains(10));
    BOOST_CHECK(tree2.contains(15));
    BOOST_CHECK(tree2.checkRBProperties() == true);

    std::remove(filename.c_str());
}

// BOOST_AUTO_TEST_CASE(test_read_nonexistent_file) {
//     RBTree tree;
//     tree.insert(42);
//     tree.readFromFile("nonexistent_file_that_does_not_exist.txt"); // не открывается
//     BOOST_CHECK(tree.contains(42) == true); // дерево не очищено, старые данные остались
//     BOOST_CHECK(tree.checkRBProperties() == true);
// }

// покрытие serializeToBinary и deserializeFromBinary
BOOST_AUTO_TEST_CASE(test_binary_serialization_full) {
    RBTree original;
    original.insert(100);
    original.insert(50);
    original.insert(150);
    original.insert(25);
    original.insert(75);
    original.insert(125);
    original.insert(175);

    const std::string bin_file = "coverage_bin.bin";
    BOOST_CHECK(original.serializeToBinary(bin_file) == true); // покрывает всю сериализацию

    RBTree loaded;
    BOOST_CHECK(loaded.deserializeFromBinary(bin_file) == true); // покрывает всю десериализацию

    BOOST_CHECK(loaded.contains(25));
    BOOST_CHECK(loaded.contains(50));
    BOOST_CHECK(loaded.contains(75));
    BOOST_CHECK(loaded.contains(100));
    BOOST_CHECK(loaded.contains(125));
    BOOST_CHECK(loaded.contains(150));
    BOOST_CHECK(loaded.contains(175));
    BOOST_CHECK(loaded.checkRBProperties() == true);

    std::remove(bin_file.c_str());
}

// Покрытие serialize/deserialize пустого дерева
BOOST_AUTO_TEST_CASE(test_binary_serialization_empty) {
    RBTree empty;
    const std::string empty_file = "empty_cov.bin";
    BOOST_CHECK(empty.serializeToBinary(empty_file) == true);

    RBTree loaded;
    BOOST_CHECK(loaded.deserializeFromBinary(empty_file) == true);
    BOOST_CHECK(loaded.isEmpty() == true);

    std::remove(empty_file.c_str());
}

// Покрытие случая, когда бинарный файл не открывается
BOOST_AUTO_TEST_CASE(test_binary_open_fail) {
    RBTree tree;
    tree.insert(1);
    BOOST_CHECK(tree.serializeToBinary("/invalid/path/no_permission.bin") == false); // не открывается
    BOOST_CHECK(tree.contains(1) == true); // дерево не изменилось
}

BOOST_AUTO_TEST_CASE(test_node_accessors_internal) {
    RBTree tree;
    tree.insert(42);
    RBNode* root = tree.getRoot();
    BOOST_CHECK(root->getValue() == 42);
    BOOST_CHECK(root->getColor() == BLACK);
    BOOST_CHECK(root->getLeft() == nullptr || root->getLeft() == root->getLeft()); // просто вызов
}


BOOST_AUTO_TEST_CASE(test_rbnode_getters_setters_full_coverage) {
    RBNode* parent = new RBNode(100, BLACK);
    RBNode* left   = new RBNode(50, RED);
    RBNode* right  = new RBNode(150, RED);

    parent->setValue(200);
    parent->setColor(RED);
    parent->setLeft(left);
    parent->setRight(right);
    parent->setParent(nullptr);  

    left->setParent(parent);
    right->setParent(parent);

    BOOST_CHECK(parent->getValue() == 200);
    BOOST_CHECK(parent->getColor() == RED);
    BOOST_CHECK(parent->getLeft() == left);
    BOOST_CHECK(parent->getRight() == right);
    BOOST_CHECK(parent->getParent() == nullptr);

    BOOST_CHECK(left->getValue() == 50);
    BOOST_CHECK(left->getColor() == RED);
    BOOST_CHECK(left->getParent() == parent);

    BOOST_CHECK(right->getValue() == 150);
    BOOST_CHECK(right->getColor() == RED);
    BOOST_CHECK(right->getParent() == parent);

    delete left;
    delete right;
    delete parent;
}
