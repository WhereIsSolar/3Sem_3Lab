#include "DoubleLinked.h"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "dlist.txt";
const std::string DATA_FILE_B = "dlistB.txt";

void loadList(DoubleLinkedList &list) {
    list.clear();
    list.readFromFile(DATA_FILE);
}

void saveList(DoubleLinkedList &list) {
    list.writeToFile(DATA_FILE);
}

void processCommand(DoubleLinkedList &list, const std::string &line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") { list.printList(); return; }
    if (cmd == "DCLEAR") {
        list.clear();
        saveList(list);
        std::cout << "List cleared.\n";
        return;
    }

    if (cmd == "DADDH") {
        std::string val; ss >> val;
        list.addHead(val);
        saveList(list);
        std::cout << "Added " << val << " to head.\n";
        return;
    }
    
    if (cmd == "DADDT") {
        std::string val; ss >> val;
        list.addTail(val);
        saveList(list);
        std::cout << "Added " << val << " to tail.\n";
        return;
    }
    
    if (cmd == "DADDB") {
        std::string target, val; ss >> target >> val;
        if (list.addBefore(target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " before " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }
    
    if (cmd == "DADDA") {
        std::string target, val; ss >> target >> val;
        if (list.addAfter(target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " after " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }

    if (cmd == "DREMH") {
        if (list.removeHead()) {
            saveList(list);
            std::cout << "Head removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }
    
    if (cmd == "DREMT") {
        if (list.removeTail()) {
            saveList(list);
            std::cout << "Tail removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }
    
    if (cmd == "DREMV") {
        std::string val; ss >> val;
        if (list.removeValue(val)) {
            saveList(list);
            std::cout << "Removed value " << val << ".\n";
        } else {
            std::cout << "Value not found.\n";
        }
        return;
    }

    if (cmd == "DREMBA") {
        std::string target; ss >> target;
        if (list.removeBefore(target)) {
            saveList(list);
            std::cout << "Removed node before " << target << ".\n";
        } else {
            std::cout << "Nothing to remove before " << target << ".\n";
        }
        return;
    }
    
    if (cmd == "DREMAF") {
        std::string target; ss >> target;
        if (list.removeAfter(target)) {
            saveList(list);
            std::cout << "Removed node after " << target << ".\n";
        } else {
            std::cout << "Nothing to remove after " << target << ".\n";
        }
        return;
    }

    if (cmd == "DGET") {
        int idx; ss >> idx;
        DNode* n = list.getNode(idx);
        if (n)
            std::cout << "Node[" << idx << "] = " << n->getValue() << "\n";
        else
            std::cout << "Index out of range.\n";
        return;
    }
    
    if (cmd == "DFIND") {
        std::string val; ss >> val;
        DNode* n = list.findValue(val);
        if (n)
            std::cout << "Found value: " << val << "\n";
        else
            std::cout << "Value not found.\n";
        return;
    }
    
    // Бинарные операции
    if (cmd == "BSAVE") {
        if (list.serializeToBinary(DATA_FILE_B)) {
            std::cout << "List saved to binary: " << DATA_FILE_B << "\n";
        } else {
            std::cout << "Error saving binary\n";
        }
        return;
    }
    
    if (cmd == "BLOAD") {
        if (list.deserializeFromBinary(DATA_FILE_B)) {
            saveList(list);
            std::cout << "List loaded from binary: " << DATA_FILE_B << "\n";
        } else {
            std::cout << "Error loading binary\n";
        }
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    DoubleLinkedList list;
    loadList(list);

    std::cout << "DoublyLinkedList CLI. Functions:\n"
              << "  DADDH <val>           - add to head\n"
              << "  DADDT <val>           - add to tail\n"
              << "  DADDB <target> <val>  - add before target\n"
              << "  DADDA <target> <val>  - add after target\n"
              << "  DREMH                 - remove head\n"
              << "  DREMT                 - remove tail\n"
              << "  DREMV <val>           - remove by value\n"
              << "  DREMBA <target>       - remove before target\n"
              << "  DREMAF <target>       - remove after target\n"
              << "  DGET <index>          - get node by index\n"
              << "  DFIND <val>           - find node by value\n"
              << "  DCLEAR                - clear list\n"
              << "  PRINT                 - print list\n"
              << "  BSAVE                 - save to binary\n"
              << "  BLOAD                 - load from binary\n"
              << "  EXIT                  - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(list, line);
    }
    
    return 0;
}
