#include <iostream>
#include <sstream>
#include <string>
#include "ForwardList.h"

const std::string DATA_FILE = "list.txt";
const std::string DATA_FILE_B = "listB.txt";

void loadList(ForwardList &list) {
    list.clear();
    list.readFromFile(DATA_FILE);
}

void saveList(ForwardList &list) {
    list.writeToFile(DATA_FILE);
}

void processCommand(ForwardList &list, const std::string &commandLine) {
    std::stringstream ss(commandLine);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") {
        list.printList();
        return;
    }

    if (cmd == "LCLEAR") {
        list.clear();
        saveList(list);
        std::cout << "List cleared.\n";
        return;
    }

    if (cmd == "LADDH") {
        std::string val; ss >> val;
        list.addHead(val);
        saveList(list);
        std::cout << "Added " << val << " to head.\n";
        return;
    }

    if (cmd == "LADDT") {
        std::string val; ss >> val;
        list.addTail(val);
        saveList(list);
        std::cout << "Added " << val << " to tail.\n";
        return;
    }

    if (cmd == "LADDB") {
        std::string target, val; ss >> target >> val;
        if (list.addBefore(target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " before " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }

    if (cmd == "LADDA") {
        std::string target, val; ss >> target >> val;
        if (list.addAfter(target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " after " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }

    if (cmd == "LREMH") {
        if (list.removeHead()) {
            saveList(list);
            std::cout << "Head removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }

    if (cmd == "LREMT") {
        if (list.removeTail()) {
            saveList(list);
            std::cout << "Tail removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }

    if (cmd == "LREMV") {
        std::string val; ss >> val;
        if (list.removeValue(val)) {
            saveList(list);
            std::cout << "Removed value " << val << ".\n";
        } else {
            std::cout << "Value not found.\n";
        }
        return;
    }

    if (cmd == "LREMBEFORE") {
        std::string target; ss >> target;
        if (list.removeBefore(target)) {
            saveList(list);
            std::cout << "Removed node before " << target << ".\n";
        } else {
            std::cout << "Nothing to remove before " << target << ".\n";
        }
        return;
    }

    if (cmd == "LREMAFTER") {
        std::string target; ss >> target;
        if (list.removeAfter(target)) {
            saveList(list);
            std::cout << "Removed node after " << target << ".\n";
        } else {
            std::cout << "Nothing to remove after " << target << ".\n";
        }
        return;
    }

    if (cmd == "LGET") {
        int idx; ss >> idx;
        Node* n = list.getNode(idx);
        if (n) std::cout << "Node[" << idx << "] = " << n->getValue() << "\n";
        else std::cout << "Index out of range.\n";
        return;
    }

    if (cmd == "LFIND") {
        std::string val; ss >> val;
        Node* n = list.findValue(val);
        if (n) std::cout << "Found value: " << val << "\n";
        else std::cout << "Value not found.\n";
        return;
    }

        if (cmd == "LBSAVE") {
            if (list.serializeToBinary(DATA_FILE_B)) {
                std::cout << "List saved to binary file: " << DATA_FILE_B << "\n";
            } else {
                std::cout << "Error saving to binary file\n";
            }
            return;
        }

        if (cmd == "LBLOAD") {
            if (list.deserializeFromBinary(DATA_FILE_B)) {
            saveList(list);  // Также сохраняем в текстовый файл для совместимости
            std::cout << "List loaded from binary file: " << DATA_FILE_B << "\n";
            } else {
                std::cout << "Error loading from binary file\n";
            }
            return;
        }
            

            std::cout << "Unknown command: " << cmd << "\n";
        }

        int main() {
            ForwardList list;  // Конструктор сам инициализирует
            loadList(list);

            std::cout << "ForwardList CLI. Functions:\n"
                << "  LADDH <val>               - add to head\n"
                << "  LADDT <val>               - add to tail\n"
                << "  LADDB <target> <val>      - add before target\n"
                << "  LADDA <target> <val>      - add after target\n"
                << "  LREMH                     - remove head\n"
                << "  LREMT                     - remove tail\n"
                << "  LREMV <val>               - remove by value\n"
                << "  LREMBEFORE <target>       - remove node before target\n"
                << "  LREMAFTER <target>        - remove node after target\n"
                << "  LGET <index>              - get node by index\n"
                << "  LFIND <val>               - find node by value\n"
                << "  LCLEAR                    - clear list\n"
                << "  PRINT                     - print list\n"
                << "  EXIT                      - quit\n"
                << "  LBSAVE                    - serialization in binary\n"
                << "  LBLOAD                    - deserialization in binary\n";

            std::string line;
            while (true) {
                std::cout << "> ";
                if (!getline(std::cin, line)) break;
                if (line == "EXIT") break;
                if (!line.empty()) processCommand(list, line);
            }

            return 0;  // Деструктор сам очистит память
        }
