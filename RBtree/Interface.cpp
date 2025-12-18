#include "RBTree.h"
#include <iostream>
#include <sstream>
#include <string>

const std::string DATA_FILE = "rbtree.txt";
const std::string BINARY_FILE = "rbtree.bin";

void loadTree(RBTree& tree) {
    tree.clear();
    tree.readFromFile(DATA_FILE);
}

void saveTree(RBTree& tree) {
    tree.writeToFile(DATA_FILE);
}

void processCommand(RBTree& tree, const std::string& line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "INSERT") {
        int val; ss >> val;
        tree.insert(val);
        saveTree(tree);
        std::cout << "Inserted " << val << "\n";
        return;
    }

    if (cmd == "REMOVE") {
        int val; ss >> val;
        if (tree.remove(val)) { 
            saveTree(tree); 
            std::cout << "Removed " << val << "\n"; 
        }
        else std::cout << "Value not found\n";
        return;
    }

    if (cmd == "FIND") {
        int val; ss >> val;
        if (tree.contains(val)) std::cout << "Found " << val << "\n";
        else std::cout << "Not found\n";
        return;
    }

    if (cmd == "PRINT") {
        tree.printTree();
        return;
    }

    if (cmd == "INORDER") {
        tree.inorderPrint();
        return;
    }

    if (cmd == "CLEAR") {
        tree.clear();
        saveTree(tree);
        std::cout << "Tree cleared\n";
        return;
    }

    if (cmd == "BSAVE") {
        if (tree.serializeToBinary(BINARY_FILE)) {
            std::cout << "Tree saved to binary: " << BINARY_FILE << "\n";
        } else {
            std::cout << "Error saving binary\n";
        }
        return;
    }

    if (cmd == "BLOAD") {
        if (tree.deserializeFromBinary(BINARY_FILE)) {
            saveTree(tree);
            std::cout << "Tree loaded from binary: " << BINARY_FILE << "\n";
        } else {
            std::cout << "Error loading binary\n";
        }
        return;
    }

    if (cmd == "CHECK") {
        if (tree.checkRBProperties()) {
            std::cout << "RB properties are satisfied\n";
        } else {
            std::cout << "RB properties violated!\n";
        }
        return;
    }

    if (cmd == "INFO") {
        std::cout << "Tree is " << (tree.isEmpty() ? "empty" : "not empty") << "\n";
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    RBTree tree;
    loadTree(tree);

    std::cout << "Red-Black Tree CLI. Commands:\n"
              << "  INSERT <val>  - add element\n"
              << "  REMOVE <val>  - remove element\n"
              << "  FIND <val>    - search element\n"
              << "  PRINT         - show tree structure\n"
              << "  INORDER       - show sorted values\n"
              << "  CLEAR         - clear tree\n"
              << "  BSAVE         - save to binary\n"
              << "  BLOAD         - load from binary\n"
              << "  CHECK         - verify RB properties\n"
              << "  INFO          - show tree status\n"
              << "  EXIT          - quit (saves automatically)\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") {
            saveTree(tree);  // Добавлено сохранение перед выходом
            std::cout << "Tree saved to " << DATA_FILE << "\n";
            break;
        }
        if (!line.empty()) processCommand(tree, line);
    }

    return 0;
}
