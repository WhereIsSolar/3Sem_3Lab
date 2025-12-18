#include "Stack.h"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "stack.txt";
const std::string BINARY_FILE = "stack.bin";

Stack stack;  

void loadStack() {
    stack.clear();
    stack.readFromFile(DATA_FILE);
}

void saveStack() {
    stack.writeToFile(DATA_FILE);
}

void processCommand(const std::string& line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PUSH") {
        std::string val; 
        if (ss >> val) {
            stack.push(val);
            saveStack();
            std::cout << "Pushed: " << val << std::endl;
        } else {
            std::cout << "Usage: PUSH <value>" << std::endl;
        }
        return;
    }

    if (cmd == "POP") {
        try {
            std::string val = stack.pop();
            saveStack();
            std::cout << "Popped: " << val << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        return;
    }

    if (cmd == "TOP") {
        try {
            std::string val = stack.top();
            std::cout << "Top: " << val << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        return;
    }

    if (cmd == "SIZE") {
        std::cout << "Size: " << stack.getSize() 
                  << ", Capacity: " << stack.getCapacity() << std::endl;
        return;
    }

    if (cmd == "CLEAR") {
        stack.clear();
        saveStack();
        std::cout << "Stack cleared" << std::endl;
        return;
    }

    if (cmd == "PRINT") {
        stack.print();
        return;
    }

    if (cmd == "BSAVE") {
        if (stack.serializeToBinary(BINARY_FILE)) {
            std::cout << "Stack saved to binary: " << BINARY_FILE << std::endl;
        } else {
            std::cout << "Error saving to binary" << std::endl;
        }
        return;
    }

    if (cmd == "BLOAD") {
        if (stack.deserializeFromBinary(BINARY_FILE)) {
            saveStack();
            std::cout << "Stack loaded from binary: " << BINARY_FILE << std::endl;
        } else {
            std::cout << "Error loading from binary" << std::endl;
        }
        return;
    }

    if (cmd == "INFO") {
        std::cout << "Stack Info:" << std::endl;
        std::cout << "  Size: " << stack.getSize() << std::endl;
        std::cout << "  Capacity: " << stack.getCapacity() << std::endl;
        std::cout << "  Empty: " << (stack.isEmpty() ? "yes" : "no") << std::endl;
        std::cout << "  Integrity: " << (stack.checkIntegrity() ? "OK" : "BROKEN") << std::endl;
        return;
    }

    std::cout << "Unknown command: " << cmd << std::endl;
}

int main() {
    loadStack();

    std::cout << "====== STACK CLI ======" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  PUSH <val>   - push value onto stack" << std::endl;
    std::cout << "  POP          - pop value from stack" << std::endl;
    std::cout << "  TOP          - show top value" << std::endl;
    std::cout << "  SIZE         - show size and capacity" << std::endl;
    std::cout << "  PRINT        - print stack (bottom..top)" << std::endl;
    std::cout << "  CLEAR        - clear stack" << std::endl;
    std::cout << "  BSAVE        - save to binary file" << std::endl;
    std::cout << "  BLOAD        - load from binary file" << std::endl;
    std::cout << "  INFO         - show detailed info" << std::endl;
    std::cout << "  EXIT         - quit program" << std::endl;
    std::cout << "=======================" << std::endl;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(line);
    }

    return 0;
}
