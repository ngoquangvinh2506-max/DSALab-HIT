# 📚 Hệ Thống Quản Lý Thư Viện — C++ CLI

Ứng dụng quản lý danh mục sách trong thư viện bằng giao diện dòng lệnh (CLI). Dự án được xây dựng dựa trên cấu trúc dữ liệu **Cây Nhị Phân Tìm Kiếm (Binary Search Tree - BST)** giúp tối ưu hóa tốc độ tìm kiếm và sắp xếp dữ liệu.

---

## 📁 Cấu Trúc Thư Mục Project

Dự án tuân thủ nghiêm ngặt nguyên tắc phân tách module: khai báo trong file tiêu đề (`.h`), cài đặt logic trong file nguồn (`.cpp`), và điều khiển luồng trong (`main.cpp`).

```text
Labs_sv/2125110206_NgoQuangVinh_ThuVien/
├── src/
│   ├── main.cpp          # Chứa hàm main() và logic điều khiển Menu CLI
│   ├── structures.h      # Khai báo các struct (Book, BSTNode) và nguyên mẫu hàm
│   └── functions.cpp     # Cài đặt chi tiết giải thuật cây BST
├── tests/
│   └── test_cases.cpp    # Mã nguồn chứa các hàm kiểm thử tự động (Unit Test)
└── README.md             # Tài liệu hướng dẫn hệ thống
