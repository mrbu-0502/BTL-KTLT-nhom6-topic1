#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <stdexcept>
#include "Customer.h"
#include "Contract.h"
#include "InputHelper.h"
#include "Date.h"

using namespace std;

// =====================================================================
// PHẦN 1: CÀI ĐẶT THỰC THỂ PACKAGE
// =====================================================================

// Cài đặt hàm khởi tạo 
Package::Package()
    : Entity(""), Price(0), DataMB(0), VoiceMinutes(0), SMS(0), Days(30) {}

Package::Package(string ID, string name, double price, int data, int voice, int sms, int days)
    : Entity(ID), PackageName(name), Price(price), DataMB(data), VoiceMinutes(voice), SMS(sms), Days(days) {}

// Cài đặt các hàm thuần ảo 
string Package::getID() const {
    return ID;
}

string Package::toString() const {
    stringstream ss;
    ss << ID << "|" << PackageName << "|" 
       << fixed << setprecision(0) << Price << "|" 
       << DataMB << "|" << VoiceMinutes << "|" << SMS << "|" << Days;
    return ss.str();
}

void Package::fromString(const string& line) {
    stringstream ss(line);
    string PriceStr;
    string DataMBStr;
    string VoiceMinutesStr;
    string SMSStr;
    string DaysStr;

    getline(ss, ID, '|');
    getline(ss, PackageName, '|');
    getline(ss, PriceStr, '|');
    getline(ss, DataMBStr, '|');
    getline(ss, VoiceMinutesStr, '|');
    getline(ss, SMSStr, '|');
    getline(ss, DaysStr, '|');

    if (!PriceStr.empty()) {
        Price = stod(PriceStr);
    }
    if (!DataMBStr.empty()) {
        DataMB = stoi(DataMBStr);
    }
    if (!VoiceMinutesStr.empty()) {
        VoiceMinutes = stoi(VoiceMinutesStr);
    } 
    if (!SMSStr.empty()) {
        SMS = stoi(SMSStr);
    }
    if (!DaysStr.empty()) {
        Days = stoi(DaysStr);
    }
}

void Package::getInformation() {
    PackageName  = InputHelper::getString("Nhap ten goi cuoc: ", false);
    Price        = InputHelper::getDouble("Nhap gia cuoc VND: ", 0.0);
    DataMB       = InputHelper::getInt("Nhap dung luong DataMB (MB): ", 0);
    VoiceMinutes = InputHelper::getInt("Nhap thoi luong cuoc goi (phut): ", 0);
    SMS          = InputHelper::getInt("Nhap so luong SMS: ", 0);
    Days         = InputHelper::getInt("Nhap chu ky goi cuoc (ngay): ", 1);
}

void Package::display() const {
    cout << left << setw(10) << ID 
         << setw(30) << PackageName << right
         << setw(10) << (long long)Price << " D"
         << setw(10) << DataMB << " MB"
         << setw(10) << VoiceMinutes << " P"
         << setw(10) << SMS << " tin"
         << setw(10) << Days << " ngay" << endl;
}

// Cài đặt getter
string Package::getName() const {
    return PackageName;
} 
double Package::getPrice() const {
    return Price;
}
int Package::getDataMB() const {
    return DataMB;
}
int Package::getVoiceMinutes() const {
    return VoiceMinutes;
}
int Package::getSMS() const {
    return SMS;
}
int Package::getDays() const {
    return Days;
}

// Cài đặt setter
void Package::setName(const string& n) {
    if (n.empty()) {
        throw invalid_argument("Ten goi cuoc khong duoc de trong!");
    }
    PackageName = n;
}

void Package::setPrice(double p) {
    if (p < 0.0) {
        throw invalid_argument("Gia cuoc khong thể am!");
    }
    Price = p; // Đã sửa lỗi dấu trừ thành dấu gán
}

void Package::setDataMB(int d) {
    if (d < 0) {
        throw invalid_argument("Dung luong Data khong the am!");
    }
    DataMB = d;
}

void Package::setVoiceMinutes(int v) {
    if (v < 0) {
        throw invalid_argument("Thoi luong thoai khong the am!");
    }
    VoiceMinutes = v; // Đã sửa lỗi dấu trừ thành dấu gán
}

void Package::setSMS(int s) {
    if (s < 0) {
        throw invalid_argument("So luong SMS khong the am!");
    }
    SMS = s;
}

void Package::setDays(int d) {
    if (d <= 0) {
        throw invalid_argument("Chu ky ngay phai lon hon 0!");
    }
    Days = d;
}

// =====================================================================
// PHẦN 2: CÀI ĐẶT GIAO DIỆN VÀ CRUD CỦA PACKAGEMENU
// =====================================================================

// Bước 2: Cài đặt CRUD

void PackageMenu::printHeader() {
    cout << string(85, '-') << "\n";
    cout << left  << setw(10) << "Ma Goi"
         << setw(26) << "Ten Goi Cuoc"
         << right << setw(12) << "Gia Cuoc"
         << setw(11) << "Data"
         << setw(8)  << "Thoai"
         << setw(8)  << "SMS"
         << setw(10) << "Chu Ky" << "\n";
    cout << string(85, '-') << "\n";
}

// 1: CREATE (Thêm mới gói cước)
void PackageMenu::handleCreate(Repository<Package>& repo) {
    cout << "\n---------------- THEM GOI CUOC MOI ----------------\n";
    
    // 1. Nhập và kiểm tra mã ID
    string id = InputHelper::getString("Nhap ma goi cuoc: ", false);
    if (repo.findById(id) != nullptr) {
        cout << ">> [LOI] Ma goi cuoc da ton tai tren he thong!\n";
        return;
    }

    // 2. Nhập dữ liệu mới
    string name  = InputHelper::getString("Nhap ten goi cuoc: ", false);
    double price = InputHelper::getDouble("Nhap gia cuoc VND: ", 0.0);
    int data     = InputHelper::getInt("Nhap dung luong DataMB (MB): ", 0);
    int voice    = InputHelper::getInt("Nhap thoi luong cuoc goi (phut): ", 0);
    int sms      = InputHelper::getInt("Nhap so luong SMS: ", 0);
    int days     = InputHelper::getInt("Nhap chu ky goi cuoc (ngay): ", 1);

    // 3. Đưa vào RAM và lưu file
    try {
        repo.add(Package(id, name, price, data, voice, sms, days));;
        cout << ">> [THANH CONG] Da them goi cuoc [" << id << "] vao he thong!\n";
    } 
    catch (const exception& e) {
        cout << ">> [THAT BAI] " << e.what() << "\n";
    }
}

// 2: READ ALL (Hiển thị tất cả)
void PackageMenu::handleReadAll(Repository<Package>& repo) { 
    cout << "\n---------------- DANH SACH GOI CUOC ----------------\n";
    const auto& list = repo.getAll();
    
    // 1: Kiểm tra rỗng
    if (list.empty()) {
        cout << ">> [THONG BAO] Danh sach goi cuoc hien tai dang trong!\n";
        return;
    }
    
    // 2: In tiêu đề
    printHeader();
    
    // 3: Duyệt và gọi display() của từng đối tượng
    for (const auto& pkg : list) {
        pkg.display();  
    }
    cout << string(85, '-') << "\n";
    cout << ">> Tong so luong: " << list.size() << " goi cuoc.\n";
}

// 3: SEARCH (Tìm kiếm)
void PackageMenu::handleSearch(Repository<Package>& repo) {
    cout << "\n---------------- TIM KIEM GOI CUOC ----------------\n";
    // 1: Nhập mã cần xem
    string id = InputHelper::getString("Nhap ma goi can tim: ", false);
    
    // 2: Tìm trực tiếp đối tượng mang mã đó trong vector
    auto* pkg = repo.findById(id);
    
    // 3: Xử lý kết quả
    if (pkg == nullptr) {
        cout << ">> [THONG BAO] Khong tim thay goi cuoc co ma: " << id << "\n";
        return;
    } else {
        cout << "\n>> THONG TIN CHI TIET GOI CUOC:\n";
        printHeader();
        pkg->display();
        cout << string(85, '-') << "\n";
    }
}

// 4: UPDATE (Cập nhật)
void PackageMenu::handleUpdate(Repository<Package>& repo) {
    cout << "\n---------------- CAP NHAT THONG TIN GOI CUOC ----------------\n";
    // 1: Nhập Id và kiểm tra có gói cước cần cập nhật không 
    string id = InputHelper::getString("Nhap ma goi cuoc can sua: ", false);
    auto* pkg = repo.findById(id);
    if (pkg == nullptr) {
        cout << ">> [THONG BAO] Khong tim thay goi cuoc co ma: " << id << "\n";
        return; 
    }
    
    // 2: In để hiện thị thông tin hiện tại
    cout << "\n>> Thong tin hien tai cua goi cuoc:\n";
    printHeader();
    pkg->display();
    cout << string(85, '-') << "\n";
    cout << "(Luu y: Nhan Enter de trong neu muon GIU NGUYEN thong tin cu. Ma goi cuoc khong duoc sua)\n\n";
    
    // 3: Nhập dữ liệu mới dưới dạng chuỗi để kiểm tra rỗng
    string newName  = InputHelper::getString("Nhap ten goi cuoc moi: ", true);
    string priceStr = InputHelper::getString("Nhap gia cuoc moi (VND): ", true);
    string dataStr  = InputHelper::getString("Nhap dung luong DataMB moi (MB): ", true);
    string voiceStr = InputHelper::getString("Nhap thoi luong cuoc goi moi (phut): ", true);
    string smsStr   = InputHelper::getString("Nhap so luong SMS moi: ", true);
    string daysStr  = InputHelper::getString("Nhap chu ky goi cuoc moi (ngay): ", true);
    
    // 4 & 5: Kiểm tra qua Setter và Ghi lại file
    try {
        if (!newName.empty()) {
            pkg->setName(newName);
        }
        if (!priceStr.empty()) {
            pkg->setPrice(stod(priceStr));
        }
        if (!dataStr.empty()) {
            pkg->setDataMB(stoi(dataStr));
        }
        if (!voiceStr.empty()) {
            pkg->setVoiceMinutes(stoi(voiceStr));
        }
        if (!smsStr.empty()) {
            pkg->setSMS(stoi(smsStr));
        }
        if (!daysStr.empty()) {
            pkg->setDays(stoi(daysStr));
        }
        repo.saveToFile();
        cout << "\n>> [THANH CONG] Da cap nhat thong tin goi cuoc [" << id << "] thanh cong!\n";
    }
    catch (const exception& e) {
        cout << "\n>> [THAT BAI] Khong the cap nhat: " << e.what() << "\n";
    }
}

// 5: DELETE (Xóa)
void PackageMenu::handleDelete(Repository<Package>& repo) {
    cout << "\n---------------- XOA GOI CUOC ----------------\n";
    // 1: Nhập Id và kiểm tra có gói cước xóa không
    string id = InputHelper::getString("Nhap ma goi cuoc can xoa: ", false);
    auto* pkg = repo.findById(id);
    if (pkg == nullptr) {
        cout << ">> [THONG BAO] Khong tim thay goi cuoc co ma: " << id << "\n";
        return; 
    }
    
    // 2: In để hiển thị thông tin hiện tại
    cout << "\n>> Thong tin hien tai cua goi cuoc:\n";
    printHeader();
    pkg->display();
    cout << string(85, '-') << "\n";
    
    // 3: Kiểm tra ràng buộc toàn vẹn khóa ngoại trước khi xóa (liên quan tới contracts.txt)
    Repository<Contract> contractRepo("data/contracts.txt"); // Đường dẫn đến file hợp đồng của nhóm
    try {
        contractRepo.loadFromFile();
    } catch (...) {
        // Nếu file hợp đồng chưa có dữ liệu hoặc chưa tạo thì bỏ qua
    }

    bool isLinked = false;
    for (const auto& contract : contractRepo.getAll()) {
        if (contract.getPhonePlan() == id) { // Dùng getPhonePlan() theo đúng Contract.h
            isLinked = true;
            break;
        }
    }

    if (isLinked) {
        cout << ">> [Tu choi xoa]: Goi cuoc [" << id 
             << "] dang ton tai trong hop dong khach hang, khong duoc phep xoa!\n";
        return;
    }
    // 4: Hỏi xác nhận có cần xóa không và lưu file
    string confirm = InputHelper::getString("Ban co chac chan muon xoa? (y/n): ", false);
    if (confirm == "y" || confirm == "Y") {
        repo.remove(id); // Thu hồi vùng nhớ và xóa phần tử khỏi vector
        cout << ">> [THANH CONG] Xoa thanh cong goi cuoc [" << id << "]!\n";
    } else {
        cout << ">> [THONG BAO] Thao tac xoa da duoc huy bo an toan.\n";
    }
}

// =====================================================================
// PHẦN 3: MENU CONSOLE ĐIỀU KHIỂN
// =====================================================================

void PackageMenu::run(Repository<Package>& repo) {
    int choice;
    do {
        cout << "\n============ QUAN LY GOI CUOC ============\n";
        cout << "  1. Them goi cuoc moi\n";
        cout << "  2. Xem danh sach goi cuoc\n";
        cout << "  3. Tim kiem goi cuoc\n";
        cout << "  4. Cap nhat thong tin goi cuoc\n";
        cout << "  5. Xoa goi cuoc\n";
        cout << "  0. Quay lai menu chinh\n";
        cout << "==========================================\n";
        choice = InputHelper::getInt("Nhap lua chon: ", 0);

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
                cout << "[!] Lua chon khong hop le, vui long nhap tu 0 den 6!\n";
        }
    } while (choice != 0);
}