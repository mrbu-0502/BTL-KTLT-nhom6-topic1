#include "CDR.h"
#include "InputHelper.h"

using namespace std;

// =========================================================================
// 1. KHOI TAO BAN GHI RONG (6 bien)
// =========================================================================
CDR::CDR() {
    a = ""; // Ma CDR
    b = ""; // So dien thoai goi
    c = ""; // So dien thoai nhan
    d = ""; // Thoi diem goi (Ngay gio)
    e = ""; // Thoi luong (giay)
    f = ""; // Cuoc phi
}

// Cac ham Get/Set co ban 
string CDR::getA() const { return a; } void CDR::setA(string x) { a = x; ID = x; }
string CDR::getB() const { return b; } void CDR::setB(string x) { b = x; }
string CDR::getC() const { return c; } void CDR::setC(string x) { c = x; }
string CDR::getD() const { return d; } void CDR::setD(string x) { d = x; }
string CDR::getE() const { return e; } void CDR::setE(string x) { e = x; }
string CDR::getF() const { return f; } void CDR::setF(string x) { f = x; }

// =========================================================================
// 2. DONG BO ID HE THONG VOI MA CDR
// =========================================================================
void CDR::setID(const string& value) {
    ID = value; 
    a = value;  
}

// =========================================================================
// 3. NHAP 6 THONG TIN AN TOAN
// =========================================================================
void CDR::getInformation() {
    a = InputHelper::getString("Nhap ma CDR (VD: CDR01): ");
    ID = a; 

    b = InputHelper::getString("Nhap so dien thoai goi (VD: 0981000000): ");
    c = InputHelper::getString("Nhap so dien thoai nhan (VD: 0912345678): ");
    d = InputHelper::getString("Nhap thoi diem goi (VD: 01/01/2024 08:30:15): ");
    e = InputHelper::getString("Nhap thoi luong cuoc goi bang giay (VD: 120): ");
    f = InputHelper::getString("Nhap cuoc phi (VD: 0): ");
}

// =========================================================================
// 4. HIEN THI RA MAN HINH
// =========================================================================
void CDR::display() const {
    cout << "Ma CDR: " << a 
         << " | So goi: " << b 
         << " | So nhan: " << c 
         << " | Thoi diem: " << d 
         << " | Thoi luong: " << e << "s"
         << " | Cuoc phi: " << f << " VND" << endl;
}

// =========================================================================
// 5. GHEP 6 BIEN DE GHI FILE TXT
// =========================================================================
string CDR::toString() const {
    // Ket qua ra dung chuan: "CDR01|0981000000|0912345678|01/01/2024 08:30:15|120|0"
    return a + "|" + b + "|" + c + "|" + d + "|" + e + "|" + f;
}

// =========================================================================
// 6. TACH CHUOI 6 KHUC (Dung find va substr)
// =========================================================================
void CDR::fromString(const string& line) {
    // Tim va cat khuc 1 (Ma CDR -> a)
    int p1 = line.find('|');
    a = line.substr(0, p1);
    
    // Tim va cat khuc 2 (So goi -> b)
    int p2 = line.find('|', p1 + 1);
    b = line.substr(p1 + 1, p2 - p1 - 1);
    
    // Tim va cat khuc 3 (So nhan -> c)
    int p3 = line.find('|', p2 + 1);
    c = line.substr(p2 + 1, p3 - p2 - 1);
    
    // Tim va cat khuc 4 (Thoi diem -> d)
    int p4 = line.find('|', p3 + 1);
    d = line.substr(p3 + 1, p4 - p3 - 1);
    
    // Tim va cat khuc 5 (Thoi luong -> e)
    int p5 = line.find('|', p4 + 1);
    e = line.substr(p4 + 1, p5 - p4 - 1);
    
    // Khuc con lai cuoi cung (Cuoc phi -> f)
    f = line.substr(p5 + 1);
    
    // Gan ID he thong
    ID = a; 
}

