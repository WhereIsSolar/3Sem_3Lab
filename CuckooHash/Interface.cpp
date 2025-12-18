#include "CuckooHash.h"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "cuckoo.txt";
const std::string BINARY_FILE = "cuckoo.bin";

void load(CuckooHashTable& ht) {
    ht.clear();
    ht.readFromFile(DATA_FILE);
}

void save(CuckooHashTable& ht) {
    ht.writeToFile(DATA_FILE);
}

void processCommand(CuckooHashTable& ht, const std::string& line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") {
        ht.print();
        return;
    }
    if (cmd == "CLEAR") {
        ht.clear();
        save(ht);
        std::cout << "Cleared\n";
        return;
    }
    if (cmd == "INSERT") {
        int key;
        std::string value, fullValue;
        ss >> key;
        std::getline(ss, fullValue);
        if (!fullValue.empty() && fullValue[0] == ' ') fullValue.erase(0, 1);
        if (ht.insert(key, fullValue)) {
            save(ht);
            std::cout << "Inserted\n";
        } else {
            std::cout << "Failed\n";
        }
        return;
    }
    if (cmd == "SEARCH") {
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
    if (cmd == "REMOVE") {
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
    CuckooHashTable ht;
    load(ht);

    std::cout << "Cuckoo Hash CLI\n"
              << "INSERT key value\n"
              << "SEARCH key\n"
              << "REMOVE key\n"
              << "CLEAR\n"
              << "PRINT\n"
              << "BSAVE / BLOAD\n"
              << "EXIT\n";

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
