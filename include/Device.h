#pragma once 
#include <iostream>
#include <string>
#include "Entity.h"

using namespace std;

// Class Device ke thua (public) toan bo quy tac tu class cha Entity
class Device : public Entity {

// ============================================================================
// VUNG PRIVATE (BAO MAT - DONG GOI DU LIEU)
// Chua 5 thuoc tinh cua Thiet bi theo dung thu tu trong file data/devices.txt
// ============================================================================
private:
    string MaIMEI;      // Cot 1: Ma dinh danh IMEI cua may (VD: 868123456789001)
    string TenMay;      // Cot 2: Ten thiet bi (VD: iPhone 13 128GB)
    string HangSX;      // Cot 3: Hang san xuat (VD: Apple, Samsung)
    string NamSX;       // Cot 4: Nam san xuat (VD: 2021)
    string TinhTrang;   // Cot 5: Tinh trang thiet bi (VD: Moi, Cu)

// ============================================================================
// VUNG PUBLIC (CONG KHAI CAC CHUC NANG CHO HE THONG GOI TOI)
// ============================================================================
public:
    // 1. Ham khoi tao thiet bi rong ban dau
    Device();
    
    // ------------------------------------------------------------------------
    // Cac ham co tu khoa "override" o cuoi la cac ham bat buoc phai viet
    // de ghi de len khung mau cua class cha Entity
    // ------------------------------------------------------------------------

    // 2. Ham lay ma dinh danh cua thiet bi (tra ve maIMEI) cho Repository su dung
    string getID() const override;

    // 3. Ham cap nhat ma dinh danh khi he thong yeu cau
    void setID(const string& value) override;

    // 4. Ham nhap 5 thong tin thiet bi tu ban phim
    void getInformation() override;

    // 5. Ham in 5 thong tin thiet bi ra man hinh
    void display() const override;

    // 6. Ham ghep 5 thuoc tinh thanh 1 dong chu ngan cach boi dau '|' de luu file
    string toString() const override;

    // 7. Ham cat 1 dong chu tu file txt ra thanh 5 thuoc tinh
    void fromString(const string& line) override;

    // 8. Cac ham Getter va Setter ho tro rang buoc du lieu
    string getMaIMEI() const;
    string getTenMay() const;
    string getHangSX() const;
    string getNamSX() const;
    string getTinhTrang() const;

    void setMaIMEI(const string& value);
    void setTenMay(const string& value);
    void setHangSX(const string& value);
    void setNamSX(const string& value);
    void setTinhTrang(const string& value);
};

// Menu con quan ly thiet bi (goi tu main.cpp)
void menuThietBi();
