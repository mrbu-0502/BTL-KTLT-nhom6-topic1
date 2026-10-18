#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <stdexcept>
#include "Customer.h"
#include "InputHelper.h"
#include "Date.h"

using namespace std;

// =====================================================================
// PHẦN 1: CÀI ĐẶT THỰC THỂ CUSTOMER
// =====================================================================

Customer::Customer()
    : Entity(""), FullName(""), CCCD(""), Address(""), DateOfBirth("") {}

Customer::Customer(string ID, string name, string cccd, string address, string date)
    : Entity(ID), FullName(name), CCCD(cccd), Address(address), DateOfBirth(date) {}

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
    stringstream ss(line);
    getline(ss, ID, '|');
    getline(ss, FullName, '|');
    getline(ss, CCCD, '|');
    getline(ss, Address, '|');
    getline(ss, DateOfBirth, '|');
}

void Customer::getInformation() {
    ID          = InputHelper::getString("Nhập mã khách hàng: ", false);
    FullName    = InputHelper::getString("Nhập tên khách hàng: ", false);
    CCCD        = InputHelper::getString("Nhập căn cước công dân của khách hàng: ", false);
    Address     = InputHelper::getString("Nhập địa chỉ khách hàng: ", false);
    DateOfBirth = InputHelper::getString("Nhập ngày sinh của khách hàng: ", false);
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
        throw invalid_argument("Tên khách hàng không được để trống !");
    }
    this->FullName = n;
}

void Customer::setCCCD(const string& c) {
    if (c.empty()) {
        throw invalid_argument("CCCD khong duoc de trong!");
    }
    if (c.length() != 12) {
        throw invalid_argument("CCCD phai dung 12 chu so!");
    }
    for (int i = 0; i < c.length(); i++) {
        if (!isdigit(c[i])) {
            throw invalid_argument("Căn cước công dân không hợp lệ !");
        }
    }
    this->CCCD = c;
}

void Customer::setAddress(const string& a) {
    if (a.empty()) {
        throw invalid_argument("Dia chi khong duoc de trong !");
    }
    if (a.find('|') != string::npos) {
        throw invalid_argument("Dia chi khong duoc chua ky tu dac biet '|' !");
    }
    this->Address = a;
}

void Customer::setDateOfBirth(const string& dStr) {
    if (dStr.empty()) {
        throw invalid_argument("Ngay sinh khong duoc de trong!");
    }
    int ngay, thang, nam;
    char c1 = 0, c2 = 0;
    stringstream ss(dStr);
    if (!(ss >> ngay >> c1 >> thang >> c2 >> nam) || c1 != '/' || c2 != '/') {
        throw invalid_argument("Ngay sinh phai dung dinh dang dd/mm/yyyy!");
    }
    Date d(ngay, thang, nam);
    if (!d.valid()) {
        throw invalid_argument("Ngay sinh khong hop le hoac vuot qua nam 2026!");
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

void CustomerMenu :: handleCreate(Repository<Customer>& repo){
    cout << "\n---------------- THEM KHACH HANG MOI ----------------\n";

    // 1. Nhập và kiểm tra mã ID
    string id = InputHelper :: getString("Nhap ma khach hang: ", false);
    if(repo.findById(id) != nullptr){
        cout << ">> [LOI] Ma khach hang da ton tai tren he thong!\n";
        return;
    }

    // 2. Nhập dữ liệu mới
    string name         = InputHelper :: getString("Nhap ten khach hang moi: ", false);
    string CCCD         = InputHelper :: getString("Nhap CCCD cua khach hang moi: ", false);
    string Address      = InputHelper :: getString("Nhap dia chi cua khach hang moi: ", false);
    string DateOfBirth  = InputHelper :: getString("Nhap ngay sinh cua khach hang moi: ", false);

    // 3. Đưa vào RAM và lưu file
    try{
        repo.add(Customer(id, name, CCCD, Address, DateOfBirth));
        cout << ">> [THANH CONG] Da them khach hang [" << id << "] vao he thong!\n";
    }
    catch(const exception& e){
        cout << ">> [THAT BAI] " << e.what() << "\n";
    }
}

void CustomerMenu :: handleReadAll(Repository<Customer>& repo){
    cout << "\n---------------- DANH SACH KHACH HANG ----------------\n";
    const auto& list = repo.getAll();

    // 1: Kiểm tra rỗng
    if(list.empty()){
        cout << ">> Danh sach khach hang hien dang trong!\n";
        return;
    }

    // 2: In tiêu đề
    printHeader();

    // 3: Duyệt và gọi display() của từng đối tượng
    for(const auto& cus : list){
        cus.display();
    }
    cout << string(78, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " khach hang.\n";
}

void CustomerMenu :: handleSearch(Repository<Customer>& repo){
    cout << "\n---------------- TIM KIEM KHACH HANG ----------------\n";
    // 1: Nhập mã cần xem
    string id = InputHelper :: getString("Nhap ma khach hang can xem: ", false);

    // 2: Tìm trực tiếp đối tượng mang mã đó trong vector
    auto* cus = repo.findById(id);

    // 3: Xử lý kết quả
    if(cus == nullptr){
        cout << ">> [THONG BAO] Khong tim thay khach hang co ma: " << id << "\n";
        return;
    }
    else{
        cout << "\n>> THONG TIN CHI TIET KHACH HANG:\n";
        printHeader();
        cus -> display();
        cout << string(78, '-') << "\n";
    }
}

void CustomerMenu :: handleUpdate(Repository<Customer>& repo){
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
    for (const auto& contract : contractRepo.getAll()){
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

void CustomerMenu ::run (Repository<Customer>& repo){
    int choice;
    do {
        cout << "\n============ QUAN LY KHACH HANG ============\n";
        cout << "  1. Them khach hang moi\n";
        cout << "  2. Xem danh sach khach hang\n";
        cout << "  3. Tim kiem khach hang\n";
        cout << "  4. Cap nhat thong tin khach hang\n";
        cout << "  5. Xoa khach hang\n";
        cout << "  0. Quay lai menu chinh\n";
        cout << "==========================================\n";
        choice = InputHelper::getInt("Nhap lua chon: ", 0, 5);

        switch (choice) {
            case 1: handleCreate(repo);   break;
            case 2: handleReadAll(repo);  break;
            case 3: handleSearch(repo);   break;
            case 4: handleUpdate(repo);   break;
            case 5: handleDelete(repo);   break;
            case 0:
                cout << ">> Dang quay lai Menu chinh...\n";
                break;
            default:
                cout << "[!] Lua chon khong hop le, vui long nhap tu 0 den 5!\n";
        }
    } while (choice != 0);
}