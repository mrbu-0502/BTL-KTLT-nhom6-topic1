/**
 * ==============================================================================
 * Người phụ trách: Kiều Đức Hiệp - B24DCVT127
 * ==============================================================================
 */
#pragma once 
#include <iostream>
#include <string>
#include "Entity.h" 

using namespace std;

class CDR : public Entity {

// ============================================================================
// VUNG PRIVATE: Chua 6 thuoc tinh theo dung 6 cot trong file data/cdrs.txt
// ============================================================================
private:
    string MaCDR;     // Cot 1: Ma ban ghi cuoc goi (VD: CDR01)
    string SoGoi;     // Cot 2: So dien thoai goi di (VD: 0981000001)
    string SoNhan;    // Cot 3: So dien thoai nghe may (VD: 0912345678)
    string ThoiDiem;  // Cot 4: Ngay gio goi (VD: 02/01/2024 08:30:15)
    string SoGiay;    // Cot 5: Thoi luong cuoc goi tinh bang giay (VD: 120)
    string CuocPhi;   // Cot 6: So tien cuoc phi (VD: 0)

// ============================================================================
// VUNG PUBLIC: Cac ham giao tiep voi he thong ben ngoai
// ============================================================================
public:
    // 1. Ham khoi tao ban ghi rong
    CDR();

    // 2. Cac ham bat buoc ghi de (override) tu lop cha Entity
    string getID() const override;              // Lay maCDR lam ID dinh danh
    void setID(const string& value) override;   // Gan maCDR
    void getInformation() override;             // Nhap 6 thong tin tu ban phim
    void display() const override;              // Hien thi 6 thong tin ra man hinh
    string toString() const override;           // Ghep 6 bien thanh chuoi co dau '|'
    void fromString(const string& line) override; // Tach chuoi co dau '|' thanh 6 bien

    // 3. Cac ham Getter va Setter ho tro truy xuat & rang buoc khoa ngoai
    string getMaCDR() const;
    string getSoGoi() const;
    string getSoNhan() const;
    string getThoiDiem() const;
    string getSoGiay() const;
    string getCuocPhi() const;

    void setMaCDR(const string& value);
    void setSoGoi(const string& value);
    void setSoNhan(const string& value);
    void setThoiDiem(const string& value);
    void setSoGiay(const string& value);
    void setCuocPhi(const string& value);
};

// Menu con quan ly cuoc goi (goi tu main.cpp)
void menuCuocGoi();
