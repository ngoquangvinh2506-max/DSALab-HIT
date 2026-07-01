#include "structures.h"
#include <iostream>

void printMenu() {
    std::cout << "\n==================================================\n";
    std::cout << "     HE THONG QUAN LY THU VIEN NANG CAO C++       \n";
    std::cout << "==================================================\n";
    std::cout << "1. Them sach moi vao he thong (BST)\n";
    std::cout << "2. Xoa sach khoi thu vien\n";
    std::cout << "3. Tim kiem sach theo Ma hoac Ten sach\n";
    std::cout << "4. Hien thi toan bo danh muc kho sach\n";
    std::cout << "5. Dang ky muon sach (Tu dong vao Queue neu ban)\n";
    std::cout << "6. Tra sach (Uu tien chuyen giao cho hang doi)\n";
    std::cout << "7. Hoan tac thao tac muon/tra vua lam (Undo Stack)\n";
    std::cout << "8. Xem so lieu thong ke (Sach hot / Doc gia tich cuc)\n";
    std::cout << "9. Luu du lieu vao file .txt\n";
    std::cout << "10. Thoat chuong trinh\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Nhap lua chon cua ban (1-10): ";
}

int main() {
    BSTNode* bookRoot = nullptr;
    HistoryStack actionStack;
    std::vector<Reader> readerList;

    loadFromFile(bookRoot);

    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cout << " [Loi] Dinh dang khong hop le!\n";
            std::cin.clear(); std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 10) {
            saveToFile(bookRoot);
            clearTree(bookRoot);
            std::cout << " He thong da tat an toan. Tam biet!\n";
            break;
        }

        switch (choice) {
        case 1: {
            std::string id, title;
            std::cout << "Nhap ma sach: "; std::cin >> id;
            std::cin.ignore();
            std::cout << "Nhap ten sach: "; std::getline(std::cin, title);
            if (insertBook(bookRoot, Book(id, title))) std::cout << " Them thanh cong!\n";
            else std::cout << " [Loi] Trung ma sach!\n";
            break;
        }
        case 2: {
            std::string id;
            std::cout << "Nhap ma sach can xoa: "; std::cin >> id;
            bookRoot = deleteBook(bookRoot, id);
            std::cout << " Da thuc hien lenh xoa.\n";
            break;
        }
        case 3: {
            int mode;
            std::cout << "1. Tim theo ID | 2. Tim theo Ten: "; std::cin >> mode;
            if (mode == 1) {
                std::string id; std::cout << "Nhap ID: "; std::cin >> id;
                BSTNode* r = searchBookById(bookRoot, id);
                if (r) std::cout << " Tim thay: " << r->data.title << "\n";
                else std::cout << " Khong tim thay!\n";
            }
            else {
                std::string title; std::cin.ignore();
                std::cout << "Nhap Ten: "; std::getline(std::cin, title);
                BSTNode* r = searchBookByTitle(bookRoot, title);
                if (r) std::cout << " Tim thay ma: " << r->data.bookId << "\n";
                else std::cout << " Khong tim thay!\n";
            }
            break;
        }
        case 4:
            std::cout << "\n--- KHO SACH HIEN CO ---\n";
            displayAllBooks(bookRoot);
            break;
        case 5: {
            std::string bId, rId;
            std::cout << "Nhap ma sach muon muon: "; std::cin >> bId;
            std::cout << "Nhap ma doc gia: "; std::cin >> rId;
            borrowBookLogic(bookRoot, bId, rId, actionStack, readerList);
            break;
        }
        case 6: {
            std::string bId;
            std::cout << "Nhap ma sach tra: "; std::cin >> bId;
            returnBookLogic(bookRoot, bId, actionStack);
            break;
        }
        case 7:
            undoLastAction(bookRoot, actionStack, readerList);
            break;
        case 8:
            showStatistics(bookRoot, readerList);
            break;
        case 9:
            saveToFile(bookRoot);
            break;
        default:
            std::cout << " Lua chon khong hop le.\n";
        }
    }
    return 0;
}
