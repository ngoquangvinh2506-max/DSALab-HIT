#define STRUCTURES_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// ==========================================
// THONG TIN DOI TUONG (OBJECTS)
// ==========================================

struct Book {
    std::string bookId;
    std::string title;
    bool isAvailable;
    int borrowCount; // Thong ke sach muon nhieu nhat

    Book(std::string id = "", std::string t = "")
        : bookId(id), title(t), isAvailable(true), borrowCount(0) {
    }
};

struct Reader {
    std::string readerId;
    std::string name;
    int borrowCount; // Thong ke doc gia tich cuc nhat

    Reader(std::string id = "", std::string n = "")
        : readerId(id), name(n), borrowCount(0) {
    }
};

// Luu thong tin phuc vu tinh nang Undo/History
struct HistoryAction {
    std::string type; // "BORROW" hoac "RETURN"
    std::string bookId;
    std::string readerId;
};

// ==========================================
// CAU TRUC DU LIEU COT LOI (DSA)
// ==========================================

// Node Cay BST (Quan ly Sach)
struct BSTNode {
    Book data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Book b) : data(b), left(nullptr), right(nullptr) {}
};

// Node Hang doi Queue (Doi muon sach)
struct QueueNode {
    std::string readerId;
    QueueNode* next;
    QueueNode(std::string id) : readerId(id), next(nullptr) {}
};

struct BorrowQueue {
    QueueNode* front;
    QueueNode* rear;
    BorrowQueue() : front(nullptr), rear(nullptr) {}
};

// Node Ngan xep Stack (Lich su muon tra)
struct StackNode {
    HistoryAction action;
    StackNode* next;
    StackNode(HistoryAction act) : action(act), next(nullptr) {}
};

struct HistoryStack {
    StackNode* top;
    HistoryStack() : top(nullptr) {}
};

// ==========================================
// NGUYEN MAU HAM (PROTOTYPES)
// ==========================================

bool insertBook(BSTNode*& root, Book newBook);
BSTNode* searchBookById(BSTNode* root, std::string id);
BSTNode* searchBookByTitle(BSTNode* root, std::string title);
BSTNode* findMin(BSTNode* root);
BSTNode* deleteBook(BSTNode*& root, std::string id);
void displayAllBooks(BSTNode* root);
void clearTree(BSTNode*& root);

void enQueue(BorrowQueue& q, std::string readerId);
std::string deQueue(BorrowQueue& q);
bool isQueueEmpty(BorrowQueue q);

void pushStack(HistoryStack& s, HistoryAction act);
HistoryAction popStack(HistoryStack& s);
bool isStackEmpty(HistoryStack s);

void borrowBookLogic(BSTNode* root, std::string bookId, std::string readerId, HistoryStack& hStack, std::vector<Reader>& readers);
void returnBookLogic(BSTNode* root, std::string bookId, HistoryStack& hStack);
void undoLastAction(BSTNode* root, HistoryStack& hStack, std::vector<Reader>& readers);
void showStatistics(BSTNode* root, const std::vector<Reader>& readers);

void saveToFile(BSTNode* root);
void loadFromFile(BSTNode*& root);
