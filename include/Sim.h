#ifndef SIM_H
#define SIM_H

#include <string>
#include "Entity.h"

// SIM so thue bao
// Dinh dang file data/sims.txt:
//   SoSim|MaIMSI|MaLoai|MaIMEI|NgayKichHoat|TrangThai
class Sim : public Entity {
private:
    std::string soSim;         // khoa chinh: 10 chu so, bat dau bang 0
    std::string maIMSI;
    std::string maLoai;        // khoa ngoai -> SubType.MaLoai
    std::string maIMEI;        // khoa ngoai -> Device.MaIMEI
    std::string ngayKichHoat;  // dd/mm/yyyy
    std::string trangThai;     // Active | Locked

public:
    Sim();
    Sim(const std::string& soSim, const std::string& maIMSI,
        const std::string& maLoai, const std::string& maIMEI,
        const std::string& ngayKichHoat, const std::string& trangThai);

    // Getter
    std::string getSoSim() const;
    std::string getMaIMSI() const;
    std::string getMaLoai() const;
    std::string getMaIMEI() const;
    std::string getNgayKichHoat() const;
    std::string getTrangThai() const;

    // Setter co kiem tra hop le (sai thi nem InvalidDataException)
    void setSoSim(const std::string& v);
    void setMaIMSI(const std::string& v);
    void setMaLoai(const std::string& v);
    void setMaIMEI(const std::string& v);
    void setNgayKichHoat(const std::string& v);
    void setTrangThai(const std::string& v);

    // 3 ham thuan ao bat buoc tu Entity
    std::string getId() const override;
    std::string toString() const override;
    void fromString(const std::string& line) override;
};

// Menu con quan ly SIM, goi tu main.cpp
void menuSim();

#endif
