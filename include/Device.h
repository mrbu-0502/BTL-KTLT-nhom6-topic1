/**
 * ==============================================================================
 * Người phụ trách: Kiều Đức Hiệp - B24DCVT127
 * ==============================================================================
 */
#pragma once
#include "Entity.h"
#include <string>

using namespace std;

// ============================================================================
// LOP THUC THE THIET BI (DEVICE)
// Format luu file: MaThietBi|MaIMEI1|MaIMEI2|TenMay|HangSX|NgaySanXuat|TinhTrang
// MaIMEI2 = "NONE" neu thiet bi chi dung 1 khe IMEI
// ============================================================================
class Device : public Entity {
private:
    string maIMEI1;
    string maIMEI2;
    string tenMay;
    string hangSX;
    string ngaySanXuat;
    string tinhTrang;

public:
    Device();
    Device(string maThietBi, string maIMEI1, string maIMEI2, string tenMay,
           string hangSX, string ngaySanXuat, string tinhTrang);

    // Cac phuong thuc thuan ao ke thua tu Entity
    // Khong override getID(): ma thiet bi da duoc luu trong thuoc tinh ID cua Entity
    void getInformation() override;
    void display() const override;
    string toString() const override;
    void fromString(const string& line) override;

    // Getter va Setter
    string getMaIMEI1() const;
    string getMaIMEI2() const;
    string getTenMay() const;
    string getHangSX() const;
    string getNgaySanXuat() const;
    string getTinhTrang() const;

    void setMaIMEI1(const string& imei);
    void setMaIMEI2(const string& imei);
    void setTenMay(const string& ten);
    void setHangSX(const string& hang);
    void setNgaySanXuat(const string& ngay);
    void setTinhTrang(const string& tt);
};

// Menu con quan ly thiet bi (goi tu main.cpp)
void menuThietBi();
