
#pragma once
#include <iostream>
#include <string>
#include "Entity.h"

class Sim : public Entity {
private:
    std::string maIMSI;
    std::string maLoai;
    std::string maIMEI;
    std::string ngayKichHoat;
    std::string trangThai;

public:
    // Constructor mac dinh va constructor day du (kiem tra hop le qua setter)
    Sim();
    Sim(const std::string& soSim, const std::string& maIMSI, const std::string& maLoai,
        const std::string& maIMEI, const std::string& ngayKichHoat, const std::string& trangThai);

    // ----- Getter -----
    std::string getSoSim() const;
    std::string getMaIMSI() const;
    std::string getMaLoai() const;
    std::string getMaIMEI() const;
    std::string getNgayKichHoat() const;
    std::string getTrangThai() const;

    // ----- Setter co kiem tra hop le (sai thi nem InvalidDataException) -----
    void setSoSim(const std::string& v);
    void setMaIMSI(const std::string& v);
    void setMaLoai(const std::string& v);
    void setMaIMEI(const std::string& v);
    void setNgayKichHoat(const std::string& v);
    void setTrangThai(const std::string& v);

    // ----- 4 ham thuan ao tu Entity -----
    void getInformation() override;                  // nhap SIM moi tu ban phim (bat nhap lai neu sai)
    void display() const override;                   // in 1 dong
    std::string toString() const override;           // SoSim|MaIMSI|MaLoai|MaIMEI|Ngay|TrangThai
    void fromString(const std::string& line) override;

    // Sua thong tin: bo trong de giu nguyen, khong cho sua So SIM
    void capNhatThongTin();

    // Kiem tra mot so SIM da ton tai trong data/sims.txt chua (Bill, ... dung de kiem tra khoa ngoai)
    static bool tonTai(const std::string& soSim);

    // Nap chong toan tu: so sanh theo So SIM, in mot dong bang
    bool operator==(const Sim& other) const;
    bool operator<(const Sim& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Sim& s);
};

// Menu con quan ly SIM (goi tu main.cpp)
void menuSim();