#include "../src/structures.h"
#include <cassert>
#include <iostream>
#include <vector>

// Ham gia lap de xoa sach khoi vector readers (phuc vu test)
void clearReaders(std::vector<Reader>& readers) {
    readers.clear();
}

void test_chuc_nang_nang_cao() {
    BSTNode* root = nullptr;
    HistoryStack hStack;
    std::vector<Reader> readers;

    // 1. Test BST: Them sach va kiem tra thu tu
    insertBook(root, Book("B02", "Cau Truc Du Lieu"));
    insertBook(root, Book("B01", "Giai Thuat Toan Tap"));
    insertBook(root, Book("B03", "Lap Trinh C++"));

    assert(root->data.bookId == "B02");
    assert(root->left->data.bookId == "B01");
    assert(root->right->data.bookId == "B03");
    std::cout << "=> Test 1: Them sach vao BST va Sap xep tu dong -> PASSED\n";

    // 2. Test Tim kiem theo Ten sach
    BSTNode* searchRes = searchBookByTitle(root, "Lap Trinh C++");
    assert(searchRes != nullptr);
    assert(searchRes->data.bookId == "B03");
    std::cout << "=> Test 2: Tim kiem sach theo Ten -> PASSED\n";

    // 3. Test Muon sach va Stack (Undo)
    // Doc gia R01 muon sach B01
    borrowBookLogic(root, "B01", "R01", hStack, readers);
    BSTNode* b01Node = searchBookById(root, "B01");
    assert(b01Node->data.isAvailable == false); // Sach khong con san sang

    // Thuc hien Undo thao tac vua muon
    undoLastAction(root, hStack, readers);
    assert(b01Node->data.isAvailable == true); // Sach phai san sang tro lai
    std::cout << "=> Test 3: Muon sach va Hoan tac (Undo Stack) -> PASSED\n";

    // 4. Test Hang doi cho (Queue) khi sach dang bi muon
    borrowBookLogic(root, "B02", "R01", hStack, readers); // R01 muon B02 thanh conng
    borrowBookLogic(root, "B02", "R02", hStack, readers); // B02 het, R02 phai vao Queue cho

    // R01 tra sach B02 -> He thong phai tu dong chuyen cho nguoi cho tiep theo (R02)
    returnBookLogic(root, "B02", hStack); 
    
    // Vi B02 duoc chuyen ngay cho R02 nen trang thai cua no van phai la false (Dang duoc muon)
    BSTNode* b02Node = searchBookById(root, "B02");
    assert(b02Node->data.isAvailable == false); 
    std::cout << "=> Test 4: Hang doi cho (Queue Waiting List) -> PASSED\n";

    // Giai phong bo nho sau khi test xong
    clearTree(root);
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "     CHUONG TRINH KIEM THU TU DONG       \n";
    std::cout << "=========================================\n";
    
    test_chuc_nang_nang_cao();
    
    std::cout << "-----------------------------------------\n";
    std::cout << " CHUC MUNG: TAT CA TEST CASE DA VUOT QUA!\n";
    std::cout << "=========================================\n";
    return 0;
}
