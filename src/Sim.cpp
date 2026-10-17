#include "Sim.h"
#include "Exceptions.h"
#include "Repository.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// ---------------------------------------------------------------
// Ham ho tro noi bo (chi dung trong file nay)
// ---------------------------------------------------------------
namespace {

bool laSo(const string& s) {
    if (s.empty()) return false;
    for (char c : s)
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

// Kiem tra ngay dd/mm/yyyy (co nam nhuan).
// Neu Date.h cua Hiep co ham kiem tra san thi co the thay vao day.
bool ngayHopLe(const string& d) {
    if (d.size() != 10 || d[2] != '/' || d[5] != '/') return false;
    string ds = d.substr(0, 2), ms = d.substr(3, 2), ys = d.substr(6, 4);
    if (!laSo(ds) || !laSo(ms) || !laSo(ys)) return false;
    int day = stoi(ds), mon = stoi(ms), year = stoi(ys);
    if (year < 1900 || mon < 1 || mon > 12 || day < 1) return false;
    static const int dm[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool nhuan = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    int maxDay = dm[mon - 1];
    if (mon == 2 && nhuan) maxDay = 29;
    return day <= maxDay;
}

// Truong khong duoc rong va khong duoc chua '|' (se pha dinh dang file)
void kiemTraTruong(const string& ten, const string& v) {
    if (v.empty())
        throw InvalidDataException(ten + " khong duoc de trong");
    if (v.find('|') != string::npos)
        throw InvalidDataException(ten + " khong duoc chua ky tu '|'");
}

string trim(const string& s) { // bỏ khoảng trắng và tab, xuống dòng
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

}  // namespace

// ---------------------------------------------------------------
// PHAN 1: Lop du lieu
// ---------------------------------------------------------------
Sim::Sim()
    : soSim(""), maIMSI(""), maLoai(""), maIMEI(""),
      ngayKichHoat(""), trangThai("Active") {}

Sim::Sim(const string& so, const string& imsi, const string& loai,
         const string& imei, const string& ngay, const string& tt) {
    setSoSim(so);
    setMaIMSI(imsi);
    setMaLoai(loai);
    setMaIMEI(imei);
    setNgayKichHoat(ngay);
    setTrangThai(tt);
}

string Sim::getSoSim() const { return soSim; }
string Sim::getMaIMSI() const { return maIMSI; }
string Sim::getMaLoai() const { return maLoai; }
string Sim::getMaIMEI() const { return maIMEI; }
string Sim::getNgayKichHoat() const { return ngayKichHoat; }
string Sim::getTrangThai() const { return trangThai; }

void Sim::setSoSim(const string& v) {
    kiemTraTruong("So SIM", v);
    if (v.size() != 10 || !laSo(v) || v[0] != '0')
        throw InvalidDataException("So SIM phai gom 10 chu so va bat dau bang 0");
    soSim = v;
}

void Sim::setMaIMSI(const string& v) {
    kiemTraTruong("Ma IMSI", v);
    maIMSI = v;
}

void Sim::setMaLoai(const string& v) {
    kiemTraTruong("Ma loai hinh", v);
    maLoai = v;
}

void Sim::setMaIMEI(const string& v) {
    kiemTraTruong("Ma IMEI", v);
    maIMEI = v;
}

void Sim::setNgayKichHoat(const string& v) {
    if (!ngayHopLe(v))
        throw InvalidDataException("Ngay kich hoat phai dung dang dd/mm/yyyy: " + v);
    ngayKichHoat = v;
}

void Sim::setTrangThai(const string& v) {
    if (v != "Active" && v != "Locked")
        throw InvalidDataException("Trang thai chi duoc la Active hoac Locked");
    trangThai = v;
}

string Sim::getId() const { return soSim; }

string Sim::toString() const {
    return soSim + "|" + maIMSI + "|" + maLoai + "|" + maIMEI + "|" +
           ngayKichHoat + "|" + trangThai;
}

void Sim::fromString(const string& line) {
    stringstream ss(line);
    string so, imsi, loai, imei, ngay, tt;
    if (!getline(ss, so, '|') || !getline(ss, imsi, '|') ||
        !getline(ss, loai, '|') || !getline(ss, imei, '|') ||
        !getline(ss, ngay, '|') || !getline(ss, tt))
        throw InvalidDataException("Dong SIM sai dinh dang: " + line);
    setSoSim(trim(so));
    setMaIMSI(trim(imsi));
    setMaLoai(trim(loai));
    setMaIMEI(trim(imei));
    setNgayKichHoat(trim(ngay));
    setTrangThai(trim(tt));
}

// ---------------------------------------------------------------
// PHAN 2: Nghiep vu CRUD va menu con
// (Nhap lieu viet tam bang getline. Neu InputHelper.h da xong thi
//  thay docDong/docLuaChon bang ham cua Cuong.)
// ---------------------------------------------------------------
namespace {

string docDong(const string& loiNhac) {
    cout << loiNhac;
    string s;
    getline(cin, s);
    return trim(s);
}

// Tra ve -1 neu nhap khong phai so, 0 neu het du lieu dau vao
int docLuaChon() {
    cout << "Chon: ";
    string s;
    if (!getline(cin, s)) return 0;
    s = trim(s);
    if (!laSo(s) || s.size() > 3) return -1;
    return stoi(s);
}

bool timSim(const vector<Sim>& ds, const string& so, Sim& ketQua) {
    for (const auto& s : ds) {
        if (s.getId() == so) {
            ketQua = s;
            return true;
        }
    }
    return false;
}

void xemDanhSach(Repository<Sim>& repo) {
    vector<Sim> ds = repo.getAll();
    if (ds.empty()) {
        cout << "(Chua co SIM nao)\n";
        return;
    }
    cout << left << setw(12) << "SoSim" << setw(18) << "MaIMSI"
         << setw(8) << "MaLoai" << setw(18) << "MaIMEI"
         << setw(14) << "NgayKichHoat" << "TrangThai\n";
    cout << string(78, '-') << "\n";
    for (const auto& s : ds) {
        cout << left << setw(12) << s.getSoSim() << setw(18) << s.getMaIMSI()
             << setw(8) << s.getMaLoai() << setw(18) << s.getMaIMEI()
             << setw(14) << s.getNgayKichHoat() << s.getTrangThai() << "\n";
    }
}

void themSim(Repository<Sim>& repo) {
    string so = docDong("So SIM (10 so, bat dau bang 0): ");
    string imsi = docDong("Ma IMSI: ");
    string loai = docDong("Ma loai hinh: ");
    string imei = docDong("Ma IMEI: ");
    string ngay = docDong("Ngay kich hoat (dd/mm/yyyy): ");
    string tt = docDong("Trang thai (Active/Locked): ");

    Sim sim(so, imsi, loai, imei, ngay, tt);  // nem InvalidDataException neu sai

    Sim tam;
    if (timSim(repo.getAll(), sim.getId(), tam))
        throw DuplicateIdException(sim.getId());

    // TODO (tuan 3): kiem tra MaLoai ton tai trong sub_types.txt va
    //                MaIMEI ton tai trong devices.txt
    repo.add(sim);
    cout << "Them SIM thanh cong.\n";
}

void capNhatSim(Repository<Sim>& repo) {
    string so = docDong("Nhap So SIM can sua: ");
    Sim sim;
    if (!timSim(repo.getAll(), so, sim)) throw NotFoundException(so);

    cout << "(De trong va nhan Enter de giu nguyen. Khong duoc sua So SIM)\n";
    string v;
    v = docDong("Ma IMSI [" + sim.getMaIMSI() + "]: ");
    if (!v.empty()) sim.setMaIMSI(v);
    v = docDong("Ma loai hinh [" + sim.getMaLoai() + "]: ");
    if (!v.empty()) sim.setMaLoai(v);
    v = docDong("Ma IMEI [" + sim.getMaIMEI() + "]: ");
    if (!v.empty()) sim.setMaIMEI(v);
    v = docDong("Ngay kich hoat [" + sim.getNgayKichHoat() + "]: ");
    if (!v.empty()) sim.setNgayKichHoat(v);
    v = docDong("Trang thai [" + sim.getTrangThai() + "]: ");
    if (!v.empty()) sim.setTrangThai(v);

    repo.update(sim);
    cout << "Cap nhat thanh cong.\n";
}

void xoaSim(Repository<Sim>& repo) {
    string so = docDong("Nhap So SIM can xoa: ");
    Sim sim;
    if (!timSim(repo.getAll(), so, sim)) throw NotFoundException(so);

    // TODO (tuan 4): neu con Bill/Contract/CDR/Recharge tham chieu den SoSim nay
    //                thi: throw ReferencedException("SIM", so, "Bill");

    string xacNhan = docDong("Ban chac chan xoa SIM " + so + "? (y/n): ");
    if (xacNhan != "y" && xacNhan != "Y") {
        cout << "Da huy xoa.\n";
        return;
    }
    repo.removeById(so);
    cout << "Xoa thanh cong.\n";
}

void chayMenu() {
    Repository<Sim> repo("data/sims.txt");
    int chon = -1;
    while (chon != 0) {
        cout << "\n===== QUAN LY SIM =====\n"
             << "1. Xem danh sach\n"
             << "2. Them moi\n"
             << "3. Cap nhat\n"
             << "4. Xoa\n"
             << "0. Quay lai\n";
        chon = docLuaChon();
        try {
            switch (chon) {
                case 1: xemDanhSach(repo); break;
                case 2: themSim(repo); break;
                case 3: capNhatSim(repo); break;
                case 4: xoaSim(repo); break;
                case 0: break;
                default: cout << "Lua chon khong hop le.\n";
            }
        } catch (const BaseException& e) {
            cout << "Loi: " << e.what() << "\n";
        } catch (const exception& e) {
            cout << "Loi he thong: " << e.what() << "\n";
        }
    }
}

}  // namespace

void menuSim() {
    try {
        chayMenu();
    } catch (const BaseException& e) {
        cout << "Loi: " << e.what() << "\n";
    }
}
