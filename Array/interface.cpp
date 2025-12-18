#include <iostream>
#include <sstream>
#include "Array.h"

const std::string DATA_FILE = "array.txt";
const std::string BINARY_FILE = "array.bin";

void loadArray(DynamicArray &arr) {
    arr.clear();
    arr.readFromFile(DATA_FILE);
}

void saveArray(DynamicArray &arr) {
    arr.writeToFile(DATA_FILE);
}

void processCommand(DynamicArray &arr, const std::string &commandLine) {
    std::stringstream ss(commandLine);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") {
        arr.print();
        return;
    }

    if (cmd == "ACLEAR") {
        arr.clear();
        saveArray(arr);
        std::cout << "Array cleared.\n";
        return;
    }

    if (cmd == "AADDEND") {
        std::string val; ss >> val;
        arr.addEnd(val);
        saveArray(arr);
        std::cout << "Added " << val << " to end.\n";
        return;
    }

    if (cmd == "AADD") {
        int index; std::string val; ss >> index >> val;
        if (arr.addAt(index, val)) {
            saveArray(arr);
            std::cout << "Inserted " << val << " at index " << index << ".\n";
        } else {
            std::cout << "Index out of range.\n";
        }
        return;
    }

    if (cmd == "AGET") {
        int index; ss >> index;
        try {
            std::cout << "Array[" << index << "] = " << arr.get(index) << "\n";
        } catch (...) {
            std::cout << "Index out of range.\n";
        }
        return;
    }

    if (cmd == "ASET") {
        int index; std::string val; ss >> index >> val;
        if (arr.set(index, val)) {
            saveArray(arr);
            std::cout << "Set index " << index << " = " << val << "\n";
        } else {
            std::cout << "Index out of range.\n";
        }
        return;
    }

    if (cmd == "AREM") {
        int index; ss >> index;
        if (arr.removeAt(index)) {
            saveArray(arr);
            std::cout << "Removed element at index " << index << ".\n";
        } else {
            std::cout << "Index out of range.\n";
        }
        return;
    }

    if (cmd == "BSAVE") {
        if (arr.serializeToBinary(BINARY_FILE)) {
            std::cout << "Array saved to binary: " << BINARY_FILE << "\n";
        } else {
            std::cout << "Error saving binary\n";
        }
        return;
    }

    if (cmd == "BLOAD") {
        if (arr.deserializeFromBinary(BINARY_FILE)) {
            saveArray(arr);
            std::cout << "Array loaded from binary: " << BINARY_FILE << "\n";
        } else {
            std::cout << "Error loading binary\n";
        }
        return;
    }

    if (cmd == "INFO") {
        std::cout << "Size: " << arr.getSize() << ", Capacity: " << arr.getCapacity() << "\n";
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    DynamicArray arr;
    loadArray(arr);

    std::cout << "DynamicArray CLI. Functions:\n"
              << "  AADDEND <val>       - add to end\n"
              << "  AADD <index> <val>  - add at index\n"
              << "  AGET <index>        - get value at index\n"
              << "  ASET <index> <val>  - set value at index\n"
              << "  AREM <index>        - remove element at index\n"
              << "  ACLEAR              - clear array\n"
              << "  PRINT               - print array\n"
              << "  BSAVE               - save to binary\n"
              << "  BLOAD               - load from binary\n"
              << "  INFO                - show size/capacity\n"
              << "  EXIT                - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(arr, line);
    }

    return 0;
}
