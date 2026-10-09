#pragma once
#include "Entity.h"
#include <string>

using namespace std;

// ============================================================================
// LOP THUC THE PHIEU KHIEU NAI (COMPLAINT)
// Format luu file: MaKN|MaKH|SoSim|NgayGui|NoiDung|TrangThai
// ============================================================================
class Complaint : public Entity {
private:
    string maKH;
    string soSim;
    string ngayGui;
    string noiDung;
    string trangThai;

public:
    Complaint();
    Complaint(string maKN, string maKH, string soSim, string ngayGui, string noiDung, string trangThai);

    // Cac phuong thuc thuan ao ke thua tu Entity
    string getID() const override;
    void getInformation() override;
    void display() const override;
    string toString() const override;
    void fromString(const string& line) override;

    // Getter va Setter
    string getMaKH() const;
    string getSoSim() const;
    string getNgayGui() const;
    string getNoiDung() const;
    string getTrangThai() const;

    void setMaKH(const string& mkh);
    void setSoSim(const string& sim);
    void setNgayGui(const string& ngay);
    void setNoiDung(const string& nd);
    void setTrangThai(const string& tt);
};

// ============================================================================
// HAM DIEU KHIEN MENU CON QUAN LY KHIEU NAI
// ============================================================================
void menuKhieuNai();

