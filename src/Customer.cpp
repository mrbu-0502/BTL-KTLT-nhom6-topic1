/**
 * ==============================================================================
 * BÀI TẬP LỚN: HỆ THỐNG QUẢN LÝ THUÊ BAO DI ĐỘNG (NHÓM 06 - ĐỀ TÀI 01)
 * Học phần: Kỹ thuật lập trình
 * Người thực hiện: Bùi Gia Khánh
 * MSSV: B24DCVT191
 * Module phụ trách: Entity.h (Lớp cơ sở trừu tượng)
 * Use Case: Quản lý Khách hàng (Customer)
 * ==============================================================================
 */

#include "Customer.h"       // Header khai báo lớp Customer và CustomerMenu
#include "Contract.h"       // Khai báo lớp Contract (để kiểm tra khóa ngoại trước khi xóa khách)
#include "Date.h"           // Xử lý và kiểm tra ngày sinh hợp lệ (leap, valid, output)
#include "InputHelper.h"    // Tiện ích nhập xuất an toàn trên console
#include "Exceptions.h"     // Các lớp ngoại lệ tự định nghĩa
#include <iostream>         // Luồng xuất nhập cout, cin
#include <iomanip>          // Định dạng căn lề cột in bảng khách hàng
#include <string>           // Xử lý chuỗi string
#include <vector>           // Chứa danh sách dữ liệu trong Repository
#include <sstream>          // Tách/ghép chuỗi qua dấu '|'
#include <cctype>           // Dùng hàm isdigit() kiểm tra 12 số CCCD
#include <algorithm>        // Thuật toán sort và biểu thức lambda

using namespace std; 

// =====================================================================
// PHẦN 1: CÀI ĐẶT THỰC THỂ CUSTOMER
// =====================================================================

Customer::Customer()
    : Entity(""), FullName(""), CCCD(""), Address(""), DateOfBirth("") {}

Customer::Customer(string ID, string name, string cccd, string address, string date)
    : Entity(ID) {
    setFullName(name);
    setCCCD(cccd);
    setAddress(address);
    setDateOfBirth(date);
}

string Customer::getID() const {
    return ID;
}

string Customer::toString() const {
    stringstream ss;
    ss << ID << "|" << FullName << "|" << CCCD << "|" 
       << Address << "|" << DateOfBirth;
    return ss.str();
}

void Customer::fromString(const string& line) {
    if (line.empty()) return;
    stringstream ss(line);
    string tempID;
    getline(ss, tempID, '|');
    if (tempID == "MaKH" || tempID.empty()) {
        ID = "";
        return;
    }
    ID = tempID;
    getline(ss, FullName, '|');
    getline(ss, CCCD, '|');
    getline(ss, Address, '|');
    getline(ss, DateOfBirth, '|');
}

void Customer::getInformation() {
    ID = InputHelper::getString("Nhap ma khach hang: ", false);
    setFullName(InputHelper::getString("Nhap ten khach hang: ", false));
    setCCCD(InputHelper::getString("Nhap can cuoc cong dan cua khach hang: ", false));
    setAddress(InputHelper::getString("Nhap dia chi khach hang: ", false));
    setDateOfBirth(InputHelper::getString("Nhap ngay sinh cua khach hang: ", false));
}

void Customer::display() const {
    cout << left << setw(10) << ID 
         << setw(22) << FullName 
         << setw(14) << CCCD 
         << setw(18) << Address 
         << setw(14) << DateOfBirth << "\n";
}

string Customer::getFullName() const { return FullName; }
string Customer::getCCCD() const { return CCCD; }
string Customer::getAddress() const { return Address; }
string Customer::getDateOfBirth() const { return DateOfBirth; }

void Customer::setFullName(const string& n) {
    if (n.empty()) {
        throw InvalidDataException("Ten khach hang khong duoc de trong!");
    }
    this->FullName = n;
}

void Customer::setCCCD(const string& c) {
    if (c.empty()) {
        throw InvalidDataException("CCCD khong duoc de trong!");
    }
    if (c.length() != 12) {
        throw InvalidDataException("CCCD phai dung 12 chu so!");
    }
    for (char ch : c) {
        if (!isdigit(ch)) {
            throw InvalidDataException("Can cuoc cong dan khong hop le (chi gom cac chu so)!");
        }
    }
    this->CCCD = c;
}

void Customer::setAddress(const string& a) {
    if (a.empty()) {
        throw InvalidDataException("Dia chi khong duoc de trong!");
    }
    if (a.find('|') != string::npos) {
        throw InvalidDataException("Dia chi khong duoc chua ky tu dac biet '|'!");
    }
    this->Address = a;
}

void Customer::setDateOfBirth(const string& dStr) {
    if (dStr.empty()) {
        throw InvalidDataException("Ngay sinh khong duoc de trong!");
    }
    int ngay, thang, nam;
    char c1 = 0, c2 = 0;
    stringstream ss(dStr);
    if (!(ss >> ngay >> c1 >> thang >> c2 >> nam) || c1 != '/' || c2 != '/') {
        throw InvalidDataException("Ngay sinh phai dung dinh dang dd/mm/yyyy!");
    }
    Date d(ngay, thang, nam);
    if (!d.valid()) {
        throw InvalidDataException("Ngay sinh khong hop le hoac vuot qua nam 2026!");
    }
    this->DateOfBirth = d.output();
}

// =====================================================================
// PHẦN 3: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA CUSTOMERMENU
// =====================================================================

void CustomerMenu::printHeader() {
    cout << string(78, '-') << "\n";
    cout << left  << setw(10) << "Ma KH"
         << setw(22) << "HO TEN"
         << setw(14) << "CCCD"
         << setw(18) << "DIA CHI"
         << setw(14) << "NGAY SINH" << "\n";
    cout << string(78, '-') << "\n";
}

void CustomerMenu::handleCreate(Repository<Customer>& repo) {
    cout << "\n---------------- THEM KHACH HANG MOI ----------------\n";

    // 1. Nhập và kiểm tra mã ID
    string id = InputHelper::getString("Nhap ma khach hang: ", false);
    if (repo.findById(id) != nullptr) {
        cout << ">> [LOI] Ma khach hang da ton tai tren he thong!\n";
        return;
    }

    // 2. Nhập dữ liệu mới
    string name        = InputHelper::getString("Nhap ten khach hang moi: ", false);
    string CCCD        = InputHelper::getString("Nhap CCCD cua khach hang moi (12 chu so): ", false);
    string Address     = InputHelper::getString("Nhap dia chi cua khach hang moi: ", false);
    string DateOfBirth = InputHelper::getString("Nhap ngay sinh cua khach hang moi (dd/mm/yyyy): ", false);

    // 3. Đưa vào RAM và lưu file (kiểm tra tính hợp lệ qua setter trong constructor)
    try {
        Customer newCus(id, name, CCCD, Address, DateOfBirth);
        repo.add(newCus);
        cout << ">> [THANH CONG] Da them khach hang [" << id << "] vao he thong!\n";
    }
    catch (const exception& e) {
        cout << ">> [THAT BAI] " << e.what() << "\n";
    }
}

void CustomerMenu::handleReadAll(Repository<Customer>& repo) {
    cout << "\n---------------- DANH SACH KHACH HANG ----------------\n";
    const auto& list = repo.getAll();

    // 1: Kiểm tra rỗng
    if (list.empty()) {
        cout << ">> Danh sach khach hang hien dang trong!\n";
        return;
    }

    // 2: In tiêu đề
    printHeader();

    // 3: Duyệt và gọi display() của từng đối tượng
    for (const auto& cus : list) {
        cus.display();
    }
    cout << string(78, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " khach hang.\n";
}

void CustomerMenu::handleSearch(Repository<Customer>& repo) {
    cout << "\n---------------- TIM KIEM KHACH HANG ----------------\n";
    // 1: Nhập mã cần xem
    string id = InputHelper::getString("Nhap ma khach hang can xem: ", false);

    // 2: Tìm trực tiếp đối tượng mang mã đó trong vector
    auto* cus = repo.findById(id);

    // 3: Xử lý kết quả
    if (cus == nullptr) {
        cout << ">> [THONG BAO] Khong tim thay khach hang co ma: " << id << "\n";
        return;
    }
    else {
        cout << "\n>> THONG TIN CHI TIET KHACH HANG:\n";
        printHeader();
        cus->display();
        cout << string(78, '-') << "\n";
    }
}

// ============================================================================
// CHỨC NĂNG 4: SẮP XẾP DANH SÁCH KHÁCH HÀNG (SORT)
// - Mục đích: Cho phép người dùng sắp xếp và hiển thị danh sách khách hàng theo
//   nhiều tiêu chí thực tế (Tên, Mã KH, Ngày sinh).
// - Kỹ thuật áp dụng:
//   + Sử dụng thuật toán std::sort từ thư viện chuẩn <algorithm> (tiêu chuẩn STL).
//   + Biểu thức Lambda C++11 linh hoạt làm comparator tùy biến:
//     * Xử lý trích xuất TÊN từ HỌ VÀ TÊN chuẩn theo quy tắc xưng tên tiếng Việt.
//     * Chuyển đổi định dạng ngày dd/mm/yyyy -> yyyymmdd để so sánh thời gian chính xác.
//   + Sắp xếp trên bản sao (vector<Customer> list = repo.getAll()), bảo toàn nguyên vẹn
//     thứ tự lưu trữ trong cơ sở dữ liệu / file text.
// ============================================================================
void CustomerMenu::handleSort(Repository<Customer>& repo) {
    cout << "\n---------------- SAP XEP KHACH HANG ----------------\n";

    // Bước 1: Lấy bản sao danh sách khách hàng từ Repository
    vector<Customer> list = repo.getAll();

    // Kiểm tra danh sách rỗng để tránh xử lý thừa và báo cho người dùng
    if (list.empty()) {
        cout << ">> Danh sach khach hang hien dang trong!\n";
        return;
    }

    // Bước 2: Hiển thị các tiêu chí sắp xếp cho người dùng lựa chọn
    cout << "  1. Ho ten (A -> Z)\n";
    cout << "  2. Ho ten (Z -> A)\n";
    cout << "  3. Ma khach hang tang dan\n";
    cout << "  4. Ngay sinh (Cu -> Moi nhat)\n";
    cout << "  0. Quay lai\n";
    int subChoice = InputHelper::getInt("Nhap tieu chi sap xep (0-4): ", 0, 4);

    // Người dùng chọn 0 -> Thoát khỏi chức năng sắp xếp
    if (subChoice == 0) return;

    // Lambda helper: Trích xuất phần "Tên" từ chuỗi "Họ và Tên"
    // Ví dụ: "Nguyen Van An" -> cắt lấy "An" để sắp xếp theo chữ cái của Tên
    auto layTen = [](const string& hoTen) {
        size_t pos = hoTen.rfind(' '); // Tìm vị trí dấu cách cuối cùng
        if (pos != string::npos) return hoTen.substr(pos + 1); // Lấy chuỗi sau dấu cách
        return hoTen; // Nếu không có dấu cách, giữ nguyên họ tên
    };

    // Bước 3: Áp dụng thuật toán std::sort tương ứng với tiêu chí được chọn
    switch (subChoice) {
        case 1:
            // Tiêu chí 1: Họ tên tăng dần từ A -> Z (Ưu tiên so sánh Tên)
            sort(list.begin(), list.end(), [&](const Customer& a, const Customer& b) {
                string tenA = layTen(a.getFullName());
                string tenB = layTen(b.getFullName());
                // Nếu Tên khác nhau -> So sánh theo Tên
                if (tenA != tenB) return tenA < tenB;
                // Nếu Tên trùng nhau -> So sánh toàn bộ Họ và Tên để phân định
                return a.getFullName() < b.getFullName();
            });
            cout << "\n>> DANH SACH KHACH HANG (HO TEN A -> Z):\n";
            break;

        case 2:
            // Tiêu chí 2: Họ tên giảm dần từ Z -> A (Đảo ngược chiều so sánh)
            sort(list.begin(), list.end(), [&](const Customer& a, const Customer& b) {
                string tenA = layTen(a.getFullName());
                string tenB = layTen(b.getFullName());
                if (tenA != tenB) return tenA > tenB;
                return a.getFullName() > b.getFullName();
            });
            cout << "\n>> DANH SACH KHACH HANG (HO TEN Z -> A):\n";
            break;

        case 3:
            // Tiêu chí 3: Mã khách hàng tăng dần (so sánh chuỗi std::string)
            sort(list.begin(), list.end(), [](const Customer& a, const Customer& b) {
                return a.getID() < b.getID();
            });
            cout << "\n>> DANH SACH KHACH HANG (MA KH TANG DAN):\n";
            break;

        case 4:
            // Tiêu chí 4: Ngày sinh từ cũ nhất đến mới nhất (Người lớn tuổi xếp trước)
            // Kỹ thuật: Định dạng ngày gốc là dd/mm/yyyy.
            // Để so sánh theo đúng thứ tự thời gian, chuyển đổi về định dạng yyyymmdd.
            // Ví dụ: 25/12/1990 -> 19901225; 01/01/2005 -> 20050101 (19901225 < 20050101).
            sort(list.begin(), list.end(), [](const Customer& a, const Customer& b) {
                string dA = a.getDateOfBirth();
                string dB = b.getDateOfBirth();
                auto convertDate = [](const string& d) {
                    if (d.length() >= 10) {
                        return d.substr(6, 4) + d.substr(3, 2) + d.substr(0, 2);
                    }
                    return d;
                };
                return convertDate(dA) < convertDate(dB);
            });
            cout << "\n>> DANH SACH KHACH HANG (NGAY SINH CU -> MOI):\n";
            break;
    }

    // Bước 4: In tiêu đề bảng và duyệt danh sách khách hàng đã sắp xếp để hiển thị
    printHeader();
    for (const auto& cus : list) {
        cus.display();
    }
    cout << string(78, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " khach hang.\n";
}

void CustomerMenu::handleUpdate(Repository<Customer>& repo) {
    cout << "\n---------------- CAP NHAT KHACH HANG ----------------\n";
    // 1: Nhập Id và kiểm tra có khách hàng cần cập nhật không 
    string id = InputHelper::getString("Nhap ma khach hang can sua: ", false);
    auto* cus = repo.findById(id);
    if (cus == nullptr) {
        cout << ">> [THONG BAO] Khong tim thay khach hang co ma: " << id << "\n";
        return; 
    }
    
    // 2: In để hiện thị thông tin hiện tại
    cout << "\n>> Thong tin hien tai cua khach hang:\n";
    printHeader();
    cus->display();
    cout << string(78, '-') << "\n";
    cout << "(Luu y: Nhan Enter de trong neu muon GIU NGUYEN thong tin cu. Ma khach hang khong duoc sua)\n\n";
    
    // 3: Nhập dữ liệu mới dưới dạng chuỗi để kiểm tra rỗng
    string newName        = InputHelper::getString("Nhap ten khach hang moi: ", true);
    string newCCCD        = InputHelper::getString("Nhap CCCD cua khach hang moi: ", true);
    string newAddress     = InputHelper::getString("Nhap dia chi cua khach hang moi: ", true);
    string newDateOfBirth = InputHelper::getString("Nhap ngay sinh moi: ", true);

    // 4 & 5: Kiểm tra qua Setter và Ghi lại file
    try {
        if (!newName.empty())        cus->setFullName(newName);
        if (!newCCCD.empty())        cus->setCCCD(newCCCD);
        if (!newAddress.empty())     cus->setAddress(newAddress);
        if (!newDateOfBirth.empty()) cus->setDateOfBirth(newDateOfBirth);

        repo.saveToFile();
        cout << "\n>> [THANH CONG] Da cap nhat thong tin khach hang [" << id << "] thanh cong!\n";
    }
    catch (const exception& e) {
        cout << "\n>> [THAT BAI] Khong the cap nhat: " << e.what() << "\n";
    }
}

void CustomerMenu::handleDelete(Repository<Customer>& repo) {
    cout << "\n---------------- XOA KHACH HANG ----------------\n";
    
    // Bước 1: Nhập mã và tìm kiếm khách hàng trong RAM
    string id = InputHelper::getString("Nhap ma khach hang can xoa: ", false);
    Customer* cus = repo.findById(id);
    
    if (cus == nullptr) {
        cout << ">> [Loi]: Khong tim thay khach hang co ma: " << id << "\n";
        return;
    }

    // Bước 2: Hiển thị thông tin khách hàng để người dùng kiểm tra lại
    cout << "\n>> Thong tin khach hang duoc chon de xoa:\n";
    printHeader();
    cus->display();
    cout << string(78, '-') << "\n";

    // Bước 3: Kiểm tra ràng buộc toàn vẹn khóa ngoại (đọc từ contracts.txt)
    Repository<Contract> contractRepo("data/contracts.txt");
    try {
        contractRepo.loadFromFile();
    } catch (...) {}

    bool isLinked = false;
    for (const auto& contract : contractRepo.getAll()) {
        if (contract.getCustomerID() == id) { // Dùng getCustomerID() theo Contract.h
            isLinked = true;
            break;
        }
    } 
    if (isLinked) {
        cout << ">> [Tu choi xoa]: Khach hang [" << id 
             << "] dang dung ten trong hop dong, khong duoc phep xoa!\n";
        return;
    }
    
    // 4: Hỏi xác nhận có cần xóa không và lưu file
    string confirm = InputHelper::getString("Ban co chac chan muon xoa? (y/n): ", false);
    if (confirm == "y" || confirm == "Y") {
        repo.remove(id); // Thu hồi vùng nhớ và xóa phần tử khỏi vector
        cout << ">> [THANH CONG] Xoa thanh cong khach hang [" << id << "]!\n";
    } else {
        cout << ">> [THONG BAO] Thao tac xoa da duoc huy bo an toan.\n";
    }
}

// =====================================================================
// PHẦN 4: MENU CONSOLE ĐIỀU KHIỂN
// =====================================================================

void CustomerMenu::run() {
    // Tự khởi tạo Repository và nạp dữ liệu từ file khi mở menu
    Repository<Customer> repo("data/customers.txt");
    try {
        repo.loadFromFile();
    } catch (...) {}

    int choice;
    do {
        cout << "\n============ QUAN LY KHACH HANG ============\n";
        cout << "  1. Them khach hang moi\n";
        cout << "  2. Xem danh sach khach hang\n";
        cout << "  3. Tim kiem khach hang\n";
        cout << "  4. Sap xep danh sach khach hang\n";
        cout << "  5. Cap nhat thong tin khach hang\n";
        cout << "  6. Xoa khach hang\n";
        cout << "  0. Quay lai menu chinh\n";
        cout << "==========================================\n";
        choice = InputHelper::getInt("Nhap lua chon: ", 0, 6);

        switch (choice) {
            case 1: handleCreate(repo);   break;
            case 2: handleReadAll(repo);  break;
            case 3: handleSearch(repo);   break;
            case 4: handleSort(repo);     break;
            case 5: handleUpdate(repo);   break;
            case 6: handleDelete(repo);   break;
            case 0:
                cout << ">> Dang quay lai Menu chinh...\n";
                break;
            default:
                cout << "[!] Lua chon khong hop le, vui long nhap tu 0 den 6!\n";
        }
    } while (choice != 0);
}

// Hàm để main.cpp gọi trực tiếp
void menuKhachHang() {
    CustomerMenu::run();
}
