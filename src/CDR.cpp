#include "CDR.h"      
#include "InputHelper.h" 

using namespace std;

// ============================================================================
// 1. HAM KHOI TAO: Gan tat ca 6 thuoc tinh ban dau bang rong ""
// ============================================================================
CDR::CDR() {
    MaCDR = "";
    SoGoi = "";
    SoNhan = "";
    ThoiDiem = "";
    SoGiay = "";
    CuocPhi = "";
}

// ============================================================================
// 2. QUAN LY MA DINH DANH (ID)
// Doi voi ban ghi cuoc goi, maCDR (VD: "CDR01") chinh la ID duy nhat
// ============================================================================
string CDR::getID() const {
    return MaCDR; // Tra ve maCDR khi kho Repository can kiem tra ma
}

void CDR::setID(const string& value) {
    ID = value;    // Gan vao bien ID cua lop cha Entity
    MaCDR = value; // Dong bo luon vao bien maCDR
}

// ============================================================================
// 3. NHAP 6 THONG TIN CUOC GOI TU BAN PHIM
// ============================================================================
void CDR::getInformation() {
    MaCDR = InputHelper::getString("Nhap ma CDR (VD: CDR01): ");
    ID = MaCDR; // Dong bo ID ngay sau khi nhap ma

    SoGoi = InputHelper::getString("Nhap so dien thoai goi: ");
    SoNhan = InputHelper::getString("Nhap so dien thoai nhan: ");
    ThoiDiem = InputHelper::getString("Nhap thoi diem goi (VD: 01/01/2024 08:30:15): ");
    SoGiay = InputHelper::getString("Nhap thoi luong cuoc goi (giay): ");
    CuocPhi = InputHelper::getString("Nhap cuoc phi: ");
}

// ============================================================================
// 4. HIEN THI BAN GHI CUOC GOI RA MAN HINH
// ============================================================================
void CDR::display() const {
    cout << "Ma CDR: " << MaCDR 
         << " | So goi: " << SoGoi 
         << " | So nhan: " << SoNhan 
         << " | Thoi diem: " << ThoiDiem 
         << " | Thoi luong: " << SoGiay << "s"
         << " | Cuoc phi: " << CuocPhi << " VND" << endl;
}

// ============================================================================
// 5. GHEP 6 BIEN THANH 1 DONG CHUOI DE GHI FILE data/cdrs.txt
// VD ket qua: "CDR01|0981000001|0912345678|02/01/2024 08:30:15|120|0"
// ============================================================================
string CDR::toString() const {
    return MaCDR + "|" + SoGoi + "|" + SoNhan + "|" + ThoiDiem + "|" + SoGiay + "|" + CuocPhi;
}

// ============================================================================
// 6. TACH CHUOI 6 KHUC TU FILE data/cdrs.txt (Co 5 dau '|' la p1, p2, p3, p4, p5)
// ============================================================================
void CDR::fromString(const string& line) {
    // Khuc 1: Tu dau chuoi den truoc dau '|' thu 1 (p1) -> maCDR
    int p1 = line.find('|');
    MaCDR = line.substr(0, p1);
    
    // Khuc 2: Nam giua dau '|' thu 1 (p1) va thu 2 (p2) -> soGoi
    int p2 = line.find('|', p1 + 1);
    SoGoi = line.substr(p1 + 1, p2 - p1 - 1);
    
    // Khuc 3: Nam giua dau '|' thu 2 (p2) va thu 3 (p3) -> soNhan
    int p3 = line.find('|', p2 + 1);
    SoNhan = line.substr(p2 + 1, p3 - p2 - 1);
    
    // Khuc 4: Nam giua dau '|' thu 3 (p3) va thu 4 (p4) -> thoiDiem
    int p4 = line.find('|', p3 + 1);
    ThoiDiem = line.substr(p3 + 1, p4 - p3 - 1);
    
    // Khuc 5: Nam giua dau '|' thu 4 (p4) va thu 5 (p5) -> soGiay
    int p5 = line.find('|', p4 + 1);
    SoGiay = line.substr(p4 + 1, p5 - p4 - 1);
    
    // Khuc 6: Phan con lai sau dau '|' thu 5 (p5 + 1) den cuoi dong -> cuocPhi
    CuocPhi = line.substr(p5 + 1);
    
    // Cap nhat ID he thong
    ID = MaCDR; 
}
