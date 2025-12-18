#pragma once
#include <string>
#include <fstream>

enum Color { RED, BLACK };

class RBNode {
private:
    int value;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

public:
    RBNode(int val = 0, Color col = RED, RBNode* l = nullptr, RBNode* r = nullptr, RBNode* p = nullptr);
    
    int getValue() const;
    Color getColor() const;
    RBNode* getLeft() const;
    RBNode* getRight() const;
    RBNode* getParent() const;
    
    void setValue(int val);
    void setColor(Color col);
    void setLeft(RBNode* node);
    void setRight(RBNode* node);
    void setParent(RBNode* node);
    
    friend class RBTree;
};

class RBTree {
private:
    RBNode* root;
    RBNode* NIL; 

    void leftRotate(RBNode* x);
    void rightRotate(RBNode* y);
    void insertFixup(RBNode* z);
    void transplant(RBNode* u, RBNode* v);
    void deleteFixup(RBNode* x);
    void clearRec(RBNode* node);
    void inorderRec(RBNode* node, std::ofstream& out) const;
    void inorderRec(RBNode* node) const;
    void printRec(RBNode* node, int indent = 0) const;
    RBNode* findNode(int val) const;
    RBNode* minimum(RBNode* node) const;

public:
    RBTree();
    ~RBTree();
    
    void insert(int val);
    bool remove(int val);
    bool contains(int val) const;
    
    void printTree() const;
    void inorderPrint() const;
    
    void readFromFile(const std::string& filename);
    void writeToFile(const std::string& filename);
    
    bool serializeToBinary(const std::string& filename) const;
    bool deserializeFromBinary(const std::string& filename);
    
    void clear();
    bool isEmpty() const;
    
    // Вспомогательные методы для тестов
    RBNode* getRoot() const;
    bool checkRBProperties() const;
};
