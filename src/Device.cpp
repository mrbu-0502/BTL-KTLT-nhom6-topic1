#include "Device.h"      // Goi ban ve Device vao day
#include "InputHelper.h" // Goi cong cu nhap lieu chong troi lenh

using namespace std;

// =========================================================================
// 1. KHOI TAO THIET BI RONG (CONSTRUCTOR)
// =========================================================================
Device::Device() {
    a = ""; // Ma IMEI ban dau de trong
    b = ""; // Ten may ban dau de trong
    c = ""; // Hang san xuat ban dau de trong
    d = ""; // Nam san xuat ban dau de trong
    e = ""; // Tinh trang may ban dau de trong
}

// =========================================================================
// 2. DONG BO ID HE THONG VOI MA IMEI
// =========================================================================
void Device::setID(const string& value) {
    ID = value; // Gan ID chung do he thong quan ly yeu cau
    a = value;  // Doi voi thiet bi, ma IMEI (a) cung chinh la ID
}

// =========================================================================
// 3. NHAP THONG TIN TU BAN PHIM (5 THONG TIN)
// =========================================================================
void Device::getInformation() {
    // Dung InputHelper de mang hinh dung lai cho minh nhap tung dong
    a = InputHelper::getString("Nhap ma IMEI: ");
    ID = a; // Nhap xong IMEI thi cap nhat luon vao ID cua he thong

    b = InputHelper::getString("Nhap ten may: ");
    c = InputHelper::getString("Nhap hang san xuat (VD: Apple, Samsung): ");
    d = InputHelper::getString("Nhap nam san xuat (VD: 2021): ");
    e = InputHelper::getString("Nhap tinh trang (VD: Moi/Cu): ");
}

// =========================================================================
// 4. IN THONG TIN RA MAN HINH DE NGUOI DUNG XEM
// =========================================================================
void Device::display() const {
    // In lan luot 5 bien ra, ngan cach nhau boi dau "|" cho dep mat
    cout << "IMEI: " << a 
         << " | May: " << b 
         << " | Hang: " << c 
         << " | Nam: " << d 
         << " | Tinh trang: " << e << endl;
}

// =========================================================================
// 5. GHEP CHUOI DE LUU KHO (GHI RA FILE TXT)
// =========================================================================
string Device::toString() const {
    // Ghep 5 bien lai, nhet dau "|" vao giua cac bien.
    // Ket qua ra se dung chuan: "861234|iPhone 13|Apple|2021|Moi"
    return a + "|" + b + "|" + c + "|" + d + "|" + e;
}

// =========================================================================
// 6. CAT CHUOI TU FILE TXT DE NAP VAO MAY TINH (QUAN TRONG NHAT)
// =========================================================================
void Device::fromString(const string& line) {
    // Gia su tham so line dang la dong chu: "861234|iPhone|Apple|2021|Moi"
    
    // --------------------------------------------------
    // B1: Tim dau '|' thu 1 (ky hieu la p1)
    int p1 = line.find('|');
    // Cat tu dau (vi tri 0), cat mot doan dai bang dung p1. Gan vao a
    a = line.substr(0, p1); 
    
    // --------------------------------------------------
    // B2: Tim dau '|' thu 2 (ky hieu la p2). Bat dau tim tu sau p1 (p1 + 1)
    int p2 = line.find('|', p1 + 1);
    // Cat doan nam giua p1 va p2. Do dai doan chu = (p2 - p1 - 1). Gan vao b
    b = line.substr(p1 + 1, p2 - p1 - 1);
    
    // --------------------------------------------------
    // B3: Tim dau '|' thu 3 (ky hieu la p3). Bat dau tim tu sau p2 (p2 + 1)
    int p3 = line.find('|', p2 + 1);
    // Cat doan nam giua p2 va p3. Do dai doan chu = (p3 - p2 - 1). Gan vao c
    c = line.substr(p2 + 1, p3 - p2 - 1);
    
    // --------------------------------------------------
    // B4: Tim dau '|' thu 4 (ky hieu la p4). Bat dau tim tu sau p3 (p3 + 1)
    int p4 = line.find('|', p3 + 1);
    // Cat doan nam giua p3 va p4. Do dai doan chu = (p4 - p3 - 1). Gan vao d
    d = line.substr(p3 + 1, p4 - p3 - 1);
    
    // --------------------------------------------------
    // B5: Lay phan cuoi cung. Khong can tim diem ket thuc nua!
    // Cat tu sau p4 (p4 + 1) cho den tan day cuoi cung cua cau chu. Gan vao e
    e = line.substr(p4 + 1);
    
    // --------------------------------------------------
    // Cuoi cung: Lay ma IMEI vua tach duoc gan vao ID
    ID = a; 
}
