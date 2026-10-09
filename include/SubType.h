#pragma once
#include "Entity.h"
#include <string>

using namespace std;

// ============================================================================
// LOP THUC THE LOAI HINH THUE BAO (SUBTYPE)
// Format luu file: MaLoai|TenLoai|MoTa|CuocThang
// ============================================================================
class SubType : public Entity {
private:
    string tenLoai;
    string moTa;
    double cuocThang;

public:
    SubType();
    SubType(string maLoai, string tenLoai, string moTa, double cuocThang);

    // Cac phuong thuc thuan ao ke thua tu Entity
    string getID() const override;
    void getInformation() override;
    void display() const override;
    string toString() const override;
    void fromString(const string& line) override;

    // Getter va Setter
    string getTenLoai() const;
    string getMoTa() const;
    double getCuocThang() const;

    void setTenLoai(const string& ten);
    void setMoTa(const string& mt);
    void setCuocThang(double cuoc);
};

// ============================================================================
// HAM DIEU KHIEN MENU CON QUAN LY LOAI HINH THUE BAO
// ============================================================================
void menuLoaiHinh();

