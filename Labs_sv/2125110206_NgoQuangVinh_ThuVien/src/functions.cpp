#include "structures.h"
#include <map>

// Dung map de quan ly hang doi cho rieng cho tung ma sach
std::map<std::string, BorrowQueue> globalWaitingQueues;

// ==========================================
// LOGIC CAY BST
// ==========================================

bool insertBook(BSTNode*& root, Book newBook) {
    if (root == nullptr) { root = new BSTNode(newBook); return true; }
    if (newBook.bookId < root->data.bookId) return insertBook(root->left, newBook);
    if (newBook.bookId > root->data.bookId) return insertBook(root->right, newBook);
    return false;
}

BSTNode* searchBookById(BSTNode* root, std::string id) {
    if (root == nullptr || root->data.bookId == id) return root;
    if (id < root->data.bookId) return searchBookById(root->left, id);
    return searchBookById(root->right, id);
}

BSTNode* searchBookByTitle(BSTNode* root, std::string title) {
    if (root == nullptr) return nullptr;
    if (root->data.title == title) return root;
    BSTNode* res = searchBookByTitle(root->left, title);
    if (res != nullptr) return res;
    return searchBookByTitle(root->right, title);
}

BSTNode* findMin(BSTNode* root) {
    while (root && root->left != nullptr) root = root->left;
    return root;
}

BSTNode* deleteBook(BSTNode*& root, std::string id) {
    if (root == nullptr) return root;
    if (id < root->data.bookId) root->left = deleteBook(root->left, id);
    else if (id > root->data.bookId) root->right = deleteBook(root->right, id);
    else {
        if (root->left == nullptr) { BSTNode* temp = root->right; delete root; return temp; }
        else if (root->right == nullptr) { BSTNode* temp = root->left; delete root; return temp; }
        BSTNode* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteBook(root->right, temp->data.bookId);
    }
    return root;
}

void displayAllBooks(BSTNode* root) {
    if (root != nullptr) {
        displayAllBooks(root->left);
        std::cout << " [" << root->data.bookId << "] " << root->data.title
            << " | Da muon: " << root->data.borrowCount << " lan"
            << " | Trang thai: " << (root->data.isAvailable ? "San sang" : "Dang duoc muon") << "\n";
        displayAllBooks(root->right);
    }
}

// ==========================================
// LOGIC HANG DOI (QUEUE)
// ==========================================

void enQueue(BorrowQueue& q, std::string readerId) {
    QueueNode* temp = new QueueNode(readerId);
    if (q.rear == nullptr) { q.front = q.rear = temp; return; }
    q.rear->next = temp; q.rear = temp;
}

std::string deQueue(BorrowQueue& q) {
    if (q.front == nullptr) return "";
    QueueNode* temp = q.front;
    std::string id = temp->readerId;
    q.front = q.front->next;
    if (q.front == nullptr) q.rear = nullptr;
    delete temp;
    return id;
}

bool isQueueEmpty(BorrowQueue q) { return q.front == nullptr; }

// ==========================================
// LOGIC NGAN XEP (STACK)
// ==========================================

void pushStack(HistoryStack& s, HistoryAction act) {
    StackNode* temp = new StackNode(act);
    temp->next = s.top; s.top = temp;
}

HistoryAction popStack(HistoryStack& s) {
    if (s.top == nullptr) return { "", "", "" };
    StackNode* temp = s.top;
    HistoryAction act = temp->action;
    s.top = s.top->next;
    delete temp;
    return act;
}

bool isStackEmpty(HistoryStack s) { return s.top == nullptr; }

// ==========================================
// nghiep vu muon tra & UNDO
// ==========================================

void borrowBookLogic(BSTNode* root, std::string bookId, std::string readerId, HistoryStack& hStack, std::vector<Reader>& readers) {
    BSTNode* bookNode = searchBookById(root, bookId);
    if (!bookNode) { std::cout << "[Loi] Khong ton tai ma sach nay!\n"; return; }

    bool readerExists = false;
    for (auto& r : readers) {
        if (r.readerId == readerId) { r.borrowCount++; readerExists = true; break; }
    }
    if (!readerExists) { readers.push_back(Reader(readerId, "Doc gia " + readerId)); readers.back().borrowCount++; }

    if (bookNode->data.isAvailable) {
        bookNode->data.isAvailable = false;
        bookNode->data.borrowCount++;
        pushStack(hStack, { "BORROW", bookId, readerId });
        std::cout << " Muon sach thanh cong!\n";
    }
    else {
        enQueue(globalWaitingQueues[bookId], readerId);
        std::cout << " Sach da co nguoi muon. Da dua Doc gia [" << readerId << "] vao Hang doi cho!\n";
    }
}

void returnBookLogic(BSTNode* root, std::string bookId, HistoryStack& hStack) {
    BSTNode* bookNode = searchBookById(root, bookId);
    if (!bookNode) { std::cout << "[Loi] Khong tim thay sach!\n"; return; }

    pushStack(hStack, { "RETURN", bookId, "" });

    if (!isQueueEmpty(globalWaitingQueues[bookId])) {
        std::string nextReader = deQueue(globalWaitingQueues[bookId]);
        bookNode->data.borrowCount++;
        std::cout << " Tra thanh cong! Sach duoc chuyen ngay cho Doc gia [" << nextReader << "] trong hang doi.\n";
    }
    else {
        bookNode->data.isAvailable = true;
        std::cout << " Tra sach thanh cong! Trang thai sach hien tai: San sang.\n";
    }
}

void undoLastAction(BSTNode* root, HistoryStack& hStack, std::vector<Reader>& readers) {
    if (isStackEmpty(hStack)) { std::cout << "[Loi] Khong co thao tac nao de Undo!\n"; return; }
    HistoryAction lastAct = popStack(hStack);
    BSTNode* bookNode = searchBookById(root, lastAct.bookId);

    if (bookNode) {
        if (lastAct.type == "BORROW") {
            bookNode->data.isAvailable = true;
            bookNode->data.borrowCount--;
            for (auto& r : readers) { if (r.readerId == lastAct.readerId) r.borrowCount--; }
            std::cout << " Undo thanh cong: Da huy luot muon sach [" << lastAct.bookId << "].\n";
        }
        else if (lastAct.type == "RETURN") {
            bookNode->data.isAvailable = false;
            std::cout << " Undo thanh cong: Da khoi phuc trang thai Dang muon cho sach [" << lastAct.bookId << "].\n";
        }
    }
}

// ==========================================
// THONG KE & FILE IO
// ==========================================

void findTopBookRecursive(BSTNode* root, BSTNode*& topBook) {
    if (root == nullptr) return;
    if (topBook == nullptr || root->data.borrowCount > topBook->data.borrowCount) { topBook = root; }
    findTopBookRecursive(root->left, topBook);
    findTopBookRecursive(root->right, topBook);
}

void showStatistics(BSTNode* root, const std::vector<Reader>& readers) {
    std::cout << "\n--- BAO CAO THONG KE THU VIEN ---\n";
    BSTNode* topBook = nullptr;
    findTopBookRecursive(root, topBook);
    if (topBook) std::cout << " Sach duoc muon nhieu nhat: [" << topBook->data.bookId << "] " << topBook->data.title << " (" << topBook->data.borrowCount << " luot)\n";
    else std::cout << " Sach duoc muon nhieu nhat: Chua co du lieu\n";

    const Reader* topReader = nullptr;
    for (const auto& r : readers) {
        if (topReader == nullptr || r.borrowCount > topReader->borrowCount) topReader = &r;
    }
    if (topReader) std::cout << " Doc gia tich cuc nhat: [" << topReader->readerId << "] voi " << topReader->borrowCount << " lan muon sach.\n";
    else std::cout << " Doc gia tich cuc nhat: Chua co du lieu\n";
}

void writeTreeToFile(BSTNode* root, std::ofstream& out) {
    if (root == nullptr) return;
    out << root->data.bookId << "," << root->data.title << "," << root->data.isAvailable << "," << root->data.borrowCount << "\n";
    writeTreeToFile(root->left, out);
    writeTreeToFile(root->right, out);
}

void saveToFile(BSTNode* root) {
    std::ofstream out("library_data.txt");
    if (!out) return;
    writeTreeToFile(root, out);
    out.close();
    std::cout << " Da luu du lieu vao 'library_data.txt'!\n";
}

void loadFromFile(BSTNode*& root) {
    std::ifstream in("library_data.txt");
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, title, availStr, countStr;
        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, availStr, ',');
        std::getline(ss, countStr, ',');

        Book b(id, title);
        b.isAvailable = (availStr == "1");
        b.borrowCount = std::stoi(countStr);
        insertBook(root, b);
    }
    in.close();
}

void clearTree(BSTNode*& root) {
    if (root != nullptr) { clearTree(root->left); clearTree(root->right); delete root; root = nullptr; }
}
