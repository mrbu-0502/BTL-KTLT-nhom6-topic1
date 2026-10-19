#include "Bill.h"
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

// Thang/nam dang mm/yyyy
bool thangNamHopLe(const string& d) {
    if (d.size() != 7 || d[2] != '/') return false;
    string ms = d.substr(0, 2), ys = d.substr(3, 4);
    if (!laSo(ms) || !laSo(ys)) return false;
    int mon = stoi(ms), year = stoi(ys);
    return mon >= 1 && mon <= 12 && year >= 1900;
}

// Truong khong duoc rong va khong duoc chua '|' (se pha dinh dang file)
void kiemTraTruong(const string& ten, const string& v) {
    if (v.empty())
        throw InvalidDataException(ten + " khong duoc de trong");
    if (v.find('|') != string::npos)
        throw InvalidDataException(ten + " khong duoc chua ky tu '|'");
}

string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Doi chuoi sang so tien, sai dinh dang thi nem InvalidDataException
long long docTien(const string& s) {
    if (!laSo(s) || s.size() > 15)
        throw InvalidDataException("Tong tien phai la so nguyen khong am: " + s);
    return stoll(s);
}

}  // namespace

// ---------------------------------------------------------------
// PHAN 1: Lop du lieu
// ---------------------------------------------------------------
Bill::Bill()
    : maHD(""), soSim(""), thangNam(""), tongTien(0), trangThai("Unpaid") {}

Bill::Bill(const string& ma, const string& so, const string& tn,
           long long tien, const string& tt) {
    setMaHD(ma);
    setSoSim(so);
    setThangNam(tn);
    setTongTien(tien);
    setTrangThai(tt);
}

string Bill::getMaHD() const { return maHD; }
string Bill::getSoSim() const { return soSim; }
string Bill::getThangNam() const { return thangNam; }
long long Bill::getTongTien() const { return tongTien; }
string Bill::getTrangThai() const { return trangThai; }

void Bill::setMaHD(const string& v) {
    kiemTraTruong("Ma hoa don", v);
    maHD = v;
}

void Bill::setSoSim(const string& v) {
    kiemTraTruong("So SIM", v);
    soSim = v;
}

void Bill::setThangNam(const string& v) {
    if (!thangNamHopLe(v))
        throw InvalidDataException("Thang nam phai dung dang mm/yyyy: " + v);
    thangNam = v;
}

void Bill::setTongTien(long long v) {
    if (v < 0)
        throw InvalidDataException("Tong tien khong duoc am");
    tongTien = v;
}

void Bill::setTrangThai(const string& v) {
    if (v != "Unpaid" && v != "Paid")
        throw InvalidDataException("Trang thai chi duoc la Unpaid hoac Paid");
    trangThai = v;
}

string Bill::getId() const { return maHD; }

string Bill::toString() const {
    return maHD + "|" + soSim + "|" + thangNam + "|" +
           to_string(tongTien) + "|" + trangThai;
}

void Bill::fromString(const string& line) {
    stringstream ss(line);
    string ma, so, tn, tien, tt;
    if (!getline(ss, ma, '|') || !getline(ss, so, '|') ||
        !getline(ss, tn, '|') || !getline(ss, tien, '|') ||
        !getline(ss, tt))
        throw InvalidDataException("Dong hoa don sai dinh dang: " + line);
    setMaHD(trim(ma));
    setSoSim(trim(so));
    setThangNam(trim(tn));
    setTongTien(docTien(trim(tien)));
    setTrangThai(trim(tt));
}

// ---------------------------------------------------------------
// PHAN 2: Nghiep vu CRUD va menu con
// (Nhap lieu viet tam bang getline. Khi InputHelper.h xong thi thay
//  docDong/docLuaChon bang ham cua Cuong.)
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

bool timBill(const vector<Bill>& ds, const string& ma, Bill& ketQua) {
    for (const auto& b : ds) {
        if (b.getId() == ma) {
            ketQua = b;
            return true;
        }
    }
    return false;
}

// Kiem tra khoa ngoai: SoSim phai ton tai trong data/sims.txt
void kiemTraSoSimTonTai(const string& soSim) {
    Repository<Sim> simRepo("data/sims.txt");
    for (const auto& s : simRepo.getAll())
        if (s.getId() == soSim) return;
    throw ForeignKeyException("SoSim", soSim);
}

void xemDanhSach(Repository<Bill>& repo) {
    vector<Bill> ds = repo.getAll();
    if (ds.empty()) {
        cout << "(Chua co hoa don nao)\n";
        return;
    }
    cout << left << setw(10) << "MaHD" << setw(14) << "SoSim"
         << setw(10) << "ThangNam" << setw(14) << "TongTien" << "TrangThai\n";
    cout << string(58, '-') << "\n";
    for (const auto& b : ds) {
        cout << left << setw(10) << b.getMaHD() << setw(14) << b.getSoSim()
             << setw(10) << b.getThangNam() << setw(14) << b.getTongTien()
             << b.getTrangThai() << "\n";
    }
}

void themBill(Repository<Bill>& repo) {
    string ma = docDong("Ma hoa don: ");
    string so = docDong("So SIM: ");
    string tn = docDong("Thang nam (mm/yyyy): ");
    string tien = docDong("Tong tien (VND): ");
    string tt = docDong("Trang thai (Unpaid/Paid): ");

    Bill bill(ma, so, tn, docTien(tien), tt);  // nem InvalidDataException neu sai

    Bill tam;
    if (timBill(repo.getAll(), bill.getId(), tam))
        throw DuplicateIdException(bill.getId());

    kiemTraSoSimTonTai(bill.getSoSim());  // rang buoc khoa ngoai
    repo.add(bill);
    cout << "Them hoa don thanh cong.\n";
}

void capNhatBill(Repository<Bill>& repo) {
    string ma = docDong("Nhap Ma hoa don can sua: ");
    Bill bill;
    if (!timBill(repo.getAll(), ma, bill)) throw NotFoundException(ma);

    cout << "(De trong va nhan Enter de giu nguyen. Khong duoc sua Ma hoa don)\n";
    string v;
    v = docDong("So SIM [" + bill.getSoSim() + "]: ");
    if (!v.empty()) {
        kiemTraSoSimTonTai(v);  // SIM moi cung phai ton tai
        bill.setSoSim(v);
    }
    v = docDong("Thang nam [" + bill.getThangNam() + "]: ");
    if (!v.empty()) bill.setThangNam(v);
    v = docDong("Tong tien [" + to_string(bill.getTongTien()) + "]: ");
    if (!v.empty()) bill.setTongTien(docTien(v));
    v = docDong("Trang thai [" + bill.getTrangThai() + "]: ");
    if (!v.empty()) bill.setTrangThai(v);

    repo.update(bill);
    cout << "Cap nhat thanh cong.\n";
}

void xoaBill(Repository<Bill>& repo) {
    string ma = docDong("Nhap Ma hoa don can xoa: ");
    Bill bill;
    if (!timBill(repo.getAll(), ma, bill)) throw NotFoundException(ma);

    // Bill khong bi bang nao khac tham chieu nen khong can kiem tra rang buoc xoa
    string xacNhan = docDong("Ban chac chan xoa hoa don " + ma + "? (y/n): ");
    if (xacNhan != "y" && xacNhan != "Y") {
        cout << "Da huy xoa.\n";
        return;
    }
    repo.removeById(ma);
    cout << "Xoa thanh cong.\n";
}

void chayMenu() {
    Repository<Bill> repo("data/bills.txt");
    int chon = -1;
    while (chon != 0) {
        cout << "\n===== QUAN LY HOA DON =====\n"
             << "1. Xem danh sach\n"
             << "2. Them moi\n"
             << "3. Cap nhat\n"
             << "4. Xoa\n"
             << "0. Quay lai\n";
        chon = docLuaChon();
        try {
            switch (chon) {
                case 1: xemDanhSach(repo); break;
                case 2: themBill(repo); break;
                case 3: capNhatBill(repo); break;
                case 4: xoaBill(repo); break;
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

void menuBill() {
    try {
        chayMenu();
    } catch (const BaseException& e) {
        cout << "Loi: " << e.what() << "\n";
    }
}