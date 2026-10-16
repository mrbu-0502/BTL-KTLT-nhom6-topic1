#include "Device.h"     
#include "InputHelper.h" 

using namespace std;

// ============================================================================
// 1. HAM KHOI TAO: Dat tat ca 5 thuoc tinh ban dau la chuoi rong ""
// ============================================================================
Device::Device() {
    MaIMEI = "";
    TenMay = "";
    HangSX = "";
    NamSX = "";
    TinhTrang = "";
}

// ============================================================================
// 2. QUAN LY MA DINH DANH (ID) CUA THIET BI
// Doi voi Device, maIMEI chinh la khoa chinh (ID) de phan biet cac may voi nhau
// ============================================================================
string Device::getID() const {
    return MaIMEI; // Khi kho Repository hoi ID la gi, tra ve ngay maIMEI
}

void Device::setID(const string& value) {
    ID = value;     // Cap nhat bien ID chung cua lop cha Entity
    MaIMEI = value; // Dong bo luon vao bien maIMEI cua lop con Device
}

// ============================================================================
// 3. NHAP DU LIEU TU BAN PHIM
// Dung InputHelper::getString de hien cau thong bao va thu ket qua nguoi dung go
// ============================================================================
void Device::getInformation() {
    MaIMEI = InputHelper::getString("Nhap ma IMEI: ");
    ID = MaIMEI; // Luu luon maIMEI vua nhap vao ID he thong

    TenMay = InputHelper::getString("Nhap ten may: ");
    HangSX = InputHelper::getString("Nhap hang san xuat: ");
    NamSX = InputHelper::getString("Nhap nam san xuat: ");
    TinhTrang = InputHelper::getString("Nhap tinh trang (Moi/Cu): ");
}

// ============================================================================
// 4. HIEN THI THONG TIN THIET BI RA MAN HINH CONSOLE
// ============================================================================
void Device::display() const {
    cout << "IMEI: " << MaIMEI 
         << " | May: " << TenMay 
         << " | Hang: " << HangSX 
         << " | Nam: " << NamSX 
         << " | Tinh trang: " << TinhTrang << endl;
}

// ============================================================================
// 5. DONG GOI DU LIEU THANH 1 DONG TEXT DE GHI VAO FILE devices.txt
// Noi 5 bien lai voi nhau, dat dau '|' xen ke o giua
// VD ket qua: "868123456789001|iPhone 13 128GB|Apple|2021|Moi"
// ============================================================================
string Device::toString() const {
    return MaIMEI + "|" + TenMay + "|" + HangSX + "|" + NamSX + "|" + TinhTrang;
}

// ============================================================================
// 6. TACH DONG TEXT TU FILE devices.txt RA THANH 5 BIEN RIENG BIET
// Gia su line = "868123456789001|iPhone 13 128GB|Apple|2021|Moi"
// Quy tac: line.substr(vi_tri_bat_dau, do_dai_can_cat)
// ============================================================================
void Device::fromString(const string& line) {
    // B1: Tim vi tri dau '|' thu 1 (p1). Cat tu dau (0) voi do dai p1 -> luu vao maIMEI
    int p1 = line.find('|');
    MaIMEI = line.substr(0, p1); 
    
    // B2: Tim dau '|' thu 2 (p2) tinh tu sau p1. Cat doan o giua p1 va p2 -> luu vao tenMay
    int p2 = line.find('|', p1 + 1);
    TenMay = line.substr(p1 + 1, p2 - p1 - 1);
    
    // B3: Tim dau '|' thu 3 (p3) tinh tu sau p2. Cat doan o giua p2 va p3 -> luu vao hangSX
    int p3 = line.find('|', p2 + 1);
    HangSX = line.substr(p2 + 1, p3 - p2 - 1);
    
    // B4: Tim dau '|' thu 4 (p4) tinh tu sau p3. Cat doan o giua p3 va p4 -> luu vao namSX
    int p4 = line.find('|', p3 + 1);
    NamSX = line.substr(p3 + 1, p4 - p3 - 1);
    
    // B5: Phan cuoi cung nam sau dau '|' thu 4 (p4 + 1) den het dong -> luu vao tinhTrang
    TinhTrang = line.substr(p4 + 1);
    
    // Cap nhat ID chung cua he thong bang maIMEI vua doc duoc
    ID = MaIMEI; 
}
