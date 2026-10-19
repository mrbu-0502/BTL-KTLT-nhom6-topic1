/**
 * ==============================================================================
 * USE CASE 8 - QUAN LY HOA DON CUOC
 * Tac gia: Quang Anh (MSSV: B24DCVT...)   <-- ghi ho ten day du + MSSV cua ban
 * Dinh dang file data/bills.txt:
 *   MaHoaDon|SoSim|ThangNam|TongTien|TrangThai
 * Rang buoc:
 *   MaHoaDon  : BILL + cac chu so (vd BILL01), duy nhat, khong duoc sua (luu trong Entity::ID)
 *   SoSim     : phai ton tai trong data/sims.txt (khoa ngoai -> Sim)
 *   ThangNam  : mm/yyyy hop le, khong sau thang hien tai; moi SIM chi co 1 hoa don / thang
 *   TongTien  : so nguyen VND, tu 0 den 1.000.000.000.000
 *   TrangThai : DaThanhToan | ChuaThanhToan
 * Khong bang nao khac tham chieu den Bill nen xoa chi can xac nhan (y/n).
 * ==============================================================================
 */
#pragma once
#include <iostream>
#include <string>
#include "Entity.h"

class Bill : public Entity {
private:
    std::string soSim;
    std::string thangNam;     // mm/yyyy
    long long tongTien;       // VND
    std::string trangThai;

public:
    // Constructor mac dinh va constructor day du (kiem tra hop le qua setter)
    Bill();
    Bill(const std::string& maHD, const std::string& soSim, const std::string& thangNam,
         long long tongTien, const std::string& trangThai);

    // ----- Getter -----
    std::string getMaHD() const;
    std::string getSoSim() const;
    std::string getThangNam() const;
    long long getTongTien() const;
    std::string getTrangThai() const;

    // ----- Setter co kiem tra hop le (sai thi nem InvalidDataException) -----
    void setMaHD(const std::string& v);
    void setSoSim(const std::string& v);
    void setThangNam(const std::string& v);
    void setTongTien(long long v);
    void setTrangThai(const std::string& v);

    // ----- 4 ham thuan ao tu Entity -----
    void getInformation() override;                  // nhap hoa don moi (bat nhap lai neu sai)
    void display() const override;                   // in 1 dong
    std::string toString() const override;           // MaHD|SoSim|ThangNam|TongTien|TrangThai
    void fromString(const std::string& line) override;

    // Sua thong tin: bo trong de giu nguyen, khong cho sua Ma hoa don
    void capNhatThongTin();

    // Nap chong toan tu: so sanh theo Ma hoa don, in mot dong bang
    bool operator==(const Bill& other) const;
    bool operator<(const Bill& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Bill& b);
};

// Menu con quan ly hoa don (goi tu main.cpp)
void menuHoaDon();