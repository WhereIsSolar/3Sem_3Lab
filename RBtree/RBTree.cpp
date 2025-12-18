#include "RBTree.h"
#include <iostream>
#include <fstream>
#include <functional>

RBNode::RBNode(int val, Color col, RBNode* l, RBNode* r, RBNode* p)
    : value(val), color(col), left(l), right(r), parent(p) {}

int RBNode::getValue() const { return value; }
Color RBNode::getColor() const { return color; }
RBNode* RBNode::getLeft() const { return left; }
RBNode* RBNode::getRight() const { return right; }
RBNode* RBNode::getParent() const { return parent; }

void RBNode::setValue(int val) { value = val; }
void RBNode::setColor(Color col) { color = col; }
void RBNode::setLeft(RBNode* node) { left = node; }
void RBNode::setRight(RBNode* node) { right = node; }
void RBNode::setParent(RBNode* node) { parent = node; }

RBTree::RBTree() {
    NIL = new RBNode(0, BLACK);
    NIL->left = NIL->right = NIL->parent = NIL;
    root = NIL;
}

RBTree::~RBTree() { 
    clear(); 
    delete NIL; 
}

RBNode* RBTree::getRoot() const { return root; }

void RBTree::clear() {
    clearRec(root);
    root = NIL;
}

void RBTree::clearRec(RBNode* node) {
    if (node == NIL) return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}

bool RBTree::isEmpty() const {
    return root == NIL;
}

void RBTree::insert(int val) {
    RBNode* z = new RBNode(val, RED, NIL, NIL);
    RBNode* y = NIL;
    RBNode* x = root;

    if (findNode(val)){
        // std::cout << "duplicate" << std::endl;
        return;
    }

    while (x != NIL) {
        y = x;
        if (z->value < x->value) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (y == NIL) root = z;
    else if (z->value < y->value) y->left = z;
    else y->right = z;

    insertFixup(z);
}

void RBTree::leftRotate(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    y->parent = x->parent;

    if (x->parent == NIL) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void RBTree::rightRotate(RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right != NIL) x->right->parent = y;
    x->parent = y->parent;

    if (y->parent == NIL) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void RBTree::insertFixup(RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

RBNode* RBTree::findNode(int val) const {
    RBNode* cur = root;
    while (cur != NIL) {
        if (cur->value == val) return cur;
        if (val < cur->value) cur = cur->left;
        else cur = cur->right;
    }
    return nullptr;
}

bool RBTree::contains(int val) const {
    return findNode(val) != nullptr;
}

RBNode* RBTree::minimum(RBNode* node) const {
    while (node->left != NIL) node = node->left;
    return node;
}

void RBTree::transplant(RBNode* u, RBNode* v) {
    if (u->parent == NIL) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;

    v->parent = u->parent;
}

void RBTree::deleteFixup(RBNode* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            RBNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

bool RBTree::remove(int val) {
    RBNode* z = findNode(val);
    if (!z) return false;

    RBNode* y = z;
    Color yOriginalColor = y->color;
    RBNode* x;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;
    if (yOriginalColor == BLACK) {
        deleteFixup(x);
    }
    return true;
}

void RBTree::printRec(RBNode* node, int indent) const {
    if (node == NIL) return;
    printRec(node->right, indent + 4);
    std::cout << std::string(indent, ' ');
    std::cout << (node->color == RED ? "R:" : "B:") << node->value << "\n";
    printRec(node->left, indent + 4);
}

void RBTree::printTree() const {
    std::cout << "Tree structure (R:red, B:black):\n";
    printRec(root);
}

void RBTree::inorderRec(RBNode* node) const {
    if (node == NIL) return;
    inorderRec(node->left);
    std::cout << node->value << " ";
    inorderRec(node->right);
}

void RBTree::inorderPrint() const {
    inorderRec(root);
    std::cout << std::endl;
}

void RBTree::readFromFile(const std::string& filename) {
    clear();
    std::ifstream in(filename);
    if (!in.is_open()) return;

    int val;
    while (in >> val) insert(val);
    in.close();
}

void RBTree::inorderRec(RBNode* node, std::ofstream& out) const {
    if (node == NIL) return;
    inorderRec(node->left, out);
    out << node->value << " ";
    inorderRec(node->right, out);
}

void RBTree::writeToFile(const std::string& filename) {
    std::ofstream out(filename);
    inorderRec(root, out);
    out.close();
}

bool RBTree::serializeToBinary(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) return false;

    std::function<void(RBNode*)> writeBFS = [&](RBNode* node) {
        if (node == NIL) return;

        int val = node->value;
        out.write(reinterpret_cast<const char*>(&val), sizeof(val));

        writeBFS(node->left);
        writeBFS(node->right);
    };

    int count = 0;
    std::function<void(RBNode*)> countNodes = [&](RBNode* node) {
        if (node == NIL) return;
        ++count;
        countNodes(node->left);
        countNodes(node->right);
    };
    countNodes(root);

    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    writeBFS(root);

    out.close();
    return true;
}

bool RBTree::deserializeFromBinary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) return false;

    clear();

    int count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (int i = 0; i < count; ++i) {
        int val = 0;
        in.read(reinterpret_cast<char*>(&val), sizeof(val));
        insert(val);
    }

    in.close();
    return true;
}

bool RBTree::checkRBProperties() const {
    if (root == NIL) return true;

    if (root->color != BLACK) return false;

    int blackHeight = -1;

    std::function<bool(RBNode*, int, bool)> checkNode = [&](RBNode* node, int currentBlack, bool parentRed) -> bool {
        if (node == NIL) {
            if (blackHeight == -1) {
                blackHeight = currentBlack;
                return true;
            }
            return blackHeight == currentBlack;
        }

        if (node->color == RED && parentRed) return false;

        int nextBlack = currentBlack + (node->color == BLACK ? 1 : 0);

        return checkNode(node->left, nextBlack, node->color == RED) &&
               checkNode(node->right, nextBlack, node->color == RED);
    };

    return checkNode(root, 0, false);
}
