#ifndef BILL_H
#define BILL_H

#include <string>
#include "Entity.h"

// Hoa don cuoc
// Dinh dang file data/bills.txt:
//   MaHD|SoSim|ThangNam|TongTien|TrangThai
class Bill : public Entity {
private:
    std::string maHD;       // khoa chinh
    std::string soSim;      // khoa ngoai -> Sim.SoSim
    std::string thangNam;   // mm/yyyy
    long long tongTien;     // VND, >= 0
    std::string trangThai;  // Unpaid | Paid

public:
    Bill();
    Bill(const std::string& maHD, const std::string& soSim,
         const std::string& thangNam, long long tongTien,
         const std::string& trangThai);

    // Getter
    std::string getMaHD() const;
    std::string getSoSim() const;
    std::string getThangNam() const;
    long long getTongTien() const;
    std::string getTrangThai() const;

    // Setter co kiem tra hop le (sai thi nem InvalidDataException)
    void setMaHD(const std::string& v);
    void setSoSim(const std::string& v);
    void setThangNam(const std::string& v);
    void setTongTien(long long v);
    void setTrangThai(const std::string& v);

    // 3 ham thuan ao bat buoc tu Entity
    std::string getId() const override;
    std::string toString() const override;
    void fromString(const std::string& line) override;
};

// Menu con quan ly hoa don, goi tu main.cpp
void menuBill();

#endif