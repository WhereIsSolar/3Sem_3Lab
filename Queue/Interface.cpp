#include "Queue.h"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "queue.txt";
const std::string BINARY_FILE = "queue.bin";

Queue queue;  

void loadQueue() {
    queue.clear();
    queue.readFromFile(DATA_FILE);
}

void saveQueue() {
    queue.writeToFile(DATA_FILE);
}

void processCommand(const std::string& line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PUSH") {
        std::string val; 
        if (ss >> val) {
            queue.enqueue(val);
            saveQueue();
            std::cout << "Enqueued: " << val << std::endl;
        } else {
            std::cout << "Usage: PUSH <value>" << std::endl;
        }
        return;
    }

    if (cmd == "POP") {
        try {
            std::string val = queue.dequeue();
            saveQueue();
            std::cout << "Dequeued: " << val << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        return;
    }

    if (cmd == "PEEK") {
        try {
            std::string val = queue.peek();
            std::cout << "Front: " << val << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        return;
    }

    if (cmd == "SIZE") {
        std::cout << "Size: " << queue.getSize() 
                  << ", Capacity: " << queue.getCapacity() << std::endl;
        return;
    }

    if (cmd == "CLEAR") {
        queue.clear();
        saveQueue();
        std::cout << "Queue cleared" << std::endl;
        return;
    }

    if (cmd == "PRINT") {
        queue.print();
        return;
    }

    if (cmd == "BSAVE") {
        if (queue.serializeToBinary(BINARY_FILE)) {
            std::cout << "Queue saved to binary: " << BINARY_FILE << std::endl;
        } else {
            std::cout << "Error saving to binary" << std::endl;
        }
        return;
    }

    if (cmd == "BLOAD") {
        if (queue.deserializeFromBinary(BINARY_FILE)) {
            saveQueue();
            std::cout << "Queue loaded from binary: " << BINARY_FILE << std::endl;
        } else {
            std::cout << "Error loading from binary" << std::endl;
        }
        return;
    }

    if (cmd == "INFO") {
        std::cout << "Queue Info:" << std::endl;
        std::cout << "  Size: " << queue.getSize() << std::endl;
        std::cout << "  Capacity: " << queue.getCapacity() << std::endl;
        std::cout << "  Empty: " << (queue.isEmpty() ? "yes" : "no") << std::endl;
        std::cout << "  Front index: " << (queue.getSize() > 0 ? "0" : "N/A") << std::endl;
        std::cout << "  Integrity: " << (queue.checkIntegrity() ? "OK" : "BROKEN") << std::endl;
        return;
    }

    std::cout << "Unknown command: " << cmd << std::endl;
}

int main() {
    loadQueue();

    std::cout << "====== QUEUE CLI ======" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  PUSH <val>   - enqueue value" << std::endl;
    std::cout << "  POP          - dequeue value" << std::endl;
    std::cout << "  PEEK         - show front value" << std::endl;
    std::cout << "  SIZE         - show size and capacity" << std::endl;
    std::cout << "  PRINT        - print queue (front..back)" << std::endl;
    std::cout << "  CLEAR        - clear queue" << std::endl;
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
