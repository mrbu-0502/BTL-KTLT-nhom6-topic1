// Nguyễn Văn Cường - B24DCVT053

#pragma once
#include "Entity.h"
#include <string>

using namespace std;

// ============================================================================
// LOP THUC THE PHIEU KHIEU NAI (COMPLAINT)
// Format luu file: MaKN|MaHD|NgayGui|NoiDung|TrangThai
// ============================================================================
class Complaint : public Entity {
private:
    string maHD;
    string ngayGui;
    string noiDung;
    string trangThai;

public:
    Complaint();
    Complaint(string maKN, string maHD, string ngayGui, string noiDung, string trangThai);

    // Cac phuong thuc thuan ao ke thua tu Entity
    // Khong override getID(): ma khieu nai da duoc luu trong thuoc tinh ID cua Entity
    void getInformation() override;
    void display() const override;
    string toString() const override;
    void fromString(const string& line) override;

    // Getter va Setter
    string getMaHD() const;
    string getNgayGui() const;
    string getNoiDung() const;
    string getTrangThai() const;

    void setMaHD(const string& mahd);
    void setNgayGui(const string& ngay);
    void setNoiDung(const string& nd);
    void setTrangThai(const string& tt);
};

