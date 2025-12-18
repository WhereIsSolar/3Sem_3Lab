#include "HashTable.h"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "hashtable.txt";
const std::string BINARY_FILE = "hashtable.bin";

void load(HashTable& ht) {
    ht.clear();
    ht.readFromFile(DATA_FILE);
}

void save(HashTable& ht) {
    ht.writeToFile(DATA_FILE);
}

void processCommand(HashTable& ht, const std::string& line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") {
        ht.print();
        return;
    }
    if (cmd == "HCLEAR") {
        ht.clear();
        save(ht);
        std::cout << "Table cleared\n";
        return;
    }
    if (cmd == "HINSERT") {
        int key;
        std::string value, fullValue;
        ss >> key;
        std::getline(ss, fullValue);
        if (!fullValue.empty() && fullValue[0] == ' ') fullValue.erase(0, 1);
        if (ht.insert(key, fullValue)) {
            save(ht);
            std::cout << "Inserted\n";
        } else {
            std::cout << "Insert failed\n";
        }
        return;
    }
    if (cmd == "HSEARCH") {
        int key;
        ss >> key;
        std::string res = ht.search(key);
        if (!res.empty()) {
            std::cout << "Found: " << res << "\n";
        } else {
            std::cout << "Not found\n";
        }
        return;
    }
    if (cmd == "HREMOVE") {
        int key;
        ss >> key;
        if (ht.remove(key)) {
            save(ht);
            std::cout << "Removed\n";
        } else {
            std::cout << "Not found\n";
        }
        return;
    }
    if (cmd == "HSIZE") {
        std::cout << "Size: " << ht.getSize() << "\n";
        return;
    }
    if (cmd == "HSTATS") {
        std::cout << "Size: " << ht.getSize() << "/" << ht.getCapacity()
                  << " (load: " << ht.getLoadFactor() * 100 << "%)\n";
        return;
    }
    if (cmd == "BSAVE") {
        if (ht.serializeToBinary(BINARY_FILE)) {
            std::cout << "Binary saved\n";
        } else {
            std::cout << "Save failed\n";
        }
        return;
    }
    if (cmd == "BLOAD") {
        if (ht.deserializeFromBinary(BINARY_FILE)) {
            save(ht);
            std::cout << "Binary loaded\n";
        } else {
            std::cout << "Load failed\n";
        }
        return;
    }
    std::cout << "Unknown command\n";
}

int main() {
    HashTable ht;
    load(ht);

    std::cout << "HashTable CLI. Commands:\n"
              << " HINSERT <key> <value>\n"
              << " HSEARCH <key>\n"
              << " HREMOVE <key>\n"
              << " HSIZE\n"
              << " HSTATS\n"
              << " HCLEAR\n"
              << " PRINT\n"
              << " BSAVE\n"
              << " BLOAD\n"
              << " EXIT\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") {
            save(ht);
            break;
        }
        if (!line.empty()) processCommand(ht, line);
    }

    return 0;
}
