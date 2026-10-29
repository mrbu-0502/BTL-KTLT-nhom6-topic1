// ===============================================================
// Hoc vien Cong nghe Buu chinh Vien thong
// Hoc phan: Ky thuat lap trinh
// De tai 1: Quan ly thue bao di dong
// Tac gia: Vu Quang Anh
// Use Case: Quan ly thue Bao Sim
// ===============================================================
#include "Sim.h"
#include "Date.h"
#include "Exceptions.h"
#include "InputHelper.h"
#include "Repository.h"
#include "Bill.h"
#include "CDR.h"
#include "Contract.h"
#include "Device.h"
#include "Recharge.h"
#include "SubType.h"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

static const string FILE_SIM = "data/sims.txt";

// ============================================================================
// HAM BO TRO (chi dung trong file nay)
// ============================================================================
// Tach mot dong theo dau '|' (bo ky tu '\r' thua neu file co xuong dong kieu Windows)
static vector<string> tachDong(string line) {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    vector<string> fields;
    stringstream ss(line);
    string f;
    while (getline(ss, f, '|')) fields.push_back(f);
    return fields;
}

// Repository tu tao file neu thieu; cac bang tham chieu phai ton tai san.
template <typename T>
static void napBangThamChieu(Repository<T>& repo, const string& file) {
    ifstream in(file);
    if (!in.is_open()) 
        throw FileException(file);
    repo.loadFromFile();
}

static bool imeiTonTai(const string& maIMEI) {
    Repository<Device> repo("data/devices.txt");
    napBangThamChieu(repo, "data/devices.txt");
    for (const Device& device : repo.getAll())
        if (device.getMaIMEI1() == maIMEI || device.getMaIMEI2() == maIMEI)
            return true;
    return false;
}

static bool laChuSo(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(),
                                [](unsigned char c) { return isdigit(c) != 0; });
}

static string chuThuong(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
}

static Date homNay() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    return Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

// Khong cho xoa SIM neu con bang khac tham chieu den no.
static void kiemTraRangBuocXoa(const string& soSim) {
    Repository<Bill> billRepo("data/bills.txt");
    napBangThamChieu(billRepo, "data/bills.txt");
    for (const Bill& bill : billRepo.getAll())
        if (bill.getSoSim() == soSim)
            throw ReferencedException("SIM", soSim, "Bill");

    Repository<Contract> contractRepo("data/contracts.txt");
    napBangThamChieu(contractRepo, "data/contracts.txt");
    for (const Contract& hd : contractRepo.getAll())
        if (hd.getPhoneNumber() == soSim)
            throw ReferencedException("SIM", soSim, "Contract");

    Repository<CDR> cdrRepo("data/cdrs.txt");
    napBangThamChieu(cdrRepo, "data/cdrs.txt");
    for (const CDR& cdr : cdrRepo.getAll()) {
        if (cdr.getSoGoi() == soSim)
            throw ReferencedException("SIM", soSim, "CDR (So goi)");
        if (cdr.getSoNhan() == soSim)
            throw ReferencedException("SIM", soSim, "CDR (So nhan)");
    }

    Repository<Recharge> rechargeRepo("data/recharges.txt");
    napBangThamChieu(rechargeRepo, "data/recharges.txt");
    for (const Recharge& phieu : rechargeRepo.getAll())
        if (phieu.getPhoneNumber() == soSim)
            throw ReferencedException("SIM", soSim, "Recharge");
}

// Kiem tra trung IMSI hoac IMEI, bo qua SIM dang sua.
static void kiemTraTrungIMSI_IMEI(int cot, const string& giaTri,const string& tenTruong,const string& soSimBoQua = "") {
    if (giaTri == "NONE") return;
    Repository<Sim> repo(FILE_SIM);
    napBangThamChieu(repo, FILE_SIM);
    for (const Sim& sim : repo.getAll()) {
        if (sim.getSoSim() == soSimBoQua) continue;
        const string daDung = (cot == 1) ? sim.getMaIMSI() : sim.getMaIMEI();
        if (daDung == giaTri)
            throw InvalidDataException(tenTruong + " '" + giaTri + "' da duoc su dung boi SIM " + sim.getSoSim());
    }
}

// Kiem tra ngay kich hoat moi khi cap nhat: Khong duoc muộn hon ngay HD hoac ngay Nap tien da co
static void kiemTraHopLeNgayKichHoatMoi(const string& soSim, const string& ngayKichHoatMoi) {
    Date dSim;
    dSim.input(ngayKichHoatMoi);

    Repository<Contract> contractRepo("data/contracts.txt");
    napBangThamChieu(contractRepo, "data/contracts.txt");
    for (const Contract& hd : contractRepo.getAll()) {
        if (hd.getPhoneNumber() != soSim) continue;
        Date dHD;
        dHD.input(hd.getRDate());
        if (dSim.compare(dHD) > 0)
            throw InvalidDataException("Ngay kich hoat moi (" + dSim.output()+ ") khong duoc sau ngay dang ky Hop dong " + hd.getContractID()+ " (" + dHD.output() + ")");
    }

    Repository<Recharge> rechargeRepo("data/recharges.txt");
    napBangThamChieu(rechargeRepo, "data/recharges.txt");
    for (const Recharge& phieu : rechargeRepo.getAll()) {
        if (phieu.getPhoneNumber() != soSim) continue;
        Date dNap;
        dNap.input(phieu.getRDate());
        if (dSim.compare(dNap) > 0)
            throw InvalidDataException("Ngay kich hoat moi (" + dSim.output()+ ") khong duoc sau ngay Nap tien phieu " + phieu.getRefID() + " (" + dNap.output() + ")");
    }
}

// Kiem tra neu chuyen SIM sang BiKhoa: Canh bao hoac chan neu dang co Hop dong HieuLuc
static void kiemTraKhoaSim(const string& soSim, const string& trangThaiMoi) {
    if (trangThaiMoi != "BiKhoa") return;
    Repository<Contract> repo("data/contracts.txt");
    napBangThamChieu(repo, "data/contracts.txt");
    for (const Contract& hd : repo.getAll())
        if (hd.getPhoneNumber() == soSim && hd.getStatus() == "HieuLuc")
            throw InvalidDataException("Khong the khoa SIM dang co hop dong HieuLuc (" + hd.getContractID() + ")!");
}
// Tra ten loai hinh / ten may tu ma (hien thi cho de doc)
static string tenLoai(const string& maLoai) {
    Repository<SubType> repo("data/sub_types.txt");
    napBangThamChieu(repo, "data/sub_types.txt");
    SubType* loai = repo.findById(maLoai);
    return loai ? loai->getTenLoai() : "(khong ro)";
}

static string tenMay(const string& maIMEI) {
    if (maIMEI == "NONE") return "(chua gan may)";
    Repository<Device> repo("data/devices.txt");
    napBangThamChieu(repo, "data/devices.txt");
    for (const Device& device : repo.getAll())
        if (device.getMaIMEI1() == maIMEI || device.getMaIMEI2() == maIMEI)
            return device.getTenMay();
    return "(khong ro)";
}

// ============================================================================
// CONSTRUCTOR
// ============================================================================
Sim::Sim() : maIMSI(""), maLoai(""), maIMEI("NONE"), ngayKichHoat(""), trangThai("HoatDong") {}
// ============================================================================
// GETTER/SETTER
// ============================================================================
Sim::Sim(const string& soSim, const string& imsi, const string& loai,
         const string& imei, const string& ngay, const string& tt) {
    setSoSim(soSim);
    setMaIMSI(imsi);
    setMaLoai(loai);
    setMaIMEI(imei);
    setNgayKichHoat(ngay);
    setTrangThai(tt);
}
string Sim::getSoSim() const { return ID; }
string Sim::getMaIMSI() const { return maIMSI; }
string Sim::getMaLoai() const { return maLoai; }
string Sim::getMaIMEI() const { return maIMEI; }
string Sim::getNgayKichHoat() const { return ngayKichHoat; }
string Sim::getTrangThai() const { return trangThai; }

void Sim::setSoSim(const string& v) {
    string phone = v;
    InputHelper::xuly_84(phone); // Chuan hoa +84 thanh 0 dung ham co san trong InputHelper
    if (!InputHelper::checkPhoneNumber(phone))
        throw InvalidDataException("So SIM phai gom 10 chu so va bat dau bang 0");
    ID = phone;
}

void Sim::setMaIMSI(const string& v) { // international mobile subscriber identity - mã ở trên phôi sim
    if (v.size() != 15 || !laChuSo(v))
        throw InvalidDataException("Ma IMSI phai gom dung 15 chu so");
    maIMSI = v;
}

void Sim::setMaLoai(const string& v) { // loai sim
    if (!InputHelper::checkidvalid(v, "SUB"))
        throw InvalidDataException("Ma loai hinh phai bat dau bang 'SUB' (vd SUB01) va khong chua '|'");
    maLoai = v;
}

void Sim::setMaIMEI(const string& v) { // mã cái điện thoại
    if (v != "NONE" && (v.size() != 15 || !laChuSo(v)))
        throw InvalidDataException("Ma IMEI phai gom 15 chu so (hoac NONE neu chua gan may)");
    maIMEI = v;
}

void Sim::setNgayKichHoat(const string& v) {
    Date d;
    d.input(v); // Dung ham input() co san cua lop Date (nem InvalidDataException neu sai)
    if (d.compare(homNay()) > 0)
        throw InvalidDataException("Ngay kich hoat khong duoc sau ngay hien tai");
    ngayKichHoat = d.output(); // Chuan hoa ve dang DD/MM/YYYY
}

void Sim::setTrangThai(const string& v) {
    if (v != "HoatDong" && v != "BiKhoa")
        throw InvalidDataException("Trang thai chi duoc la HoatDong hoac BiKhoa");
    trangThai = v;
}
// ============================================================================
// KE THUA TU ENTITY
// ============================================================================
void Sim::getInformation() {
    // 1. Nhap So SIM: 10 so, bat dau bang 0, chua ton tai tren he thong
    while (true) {
        try {
            string so = InputHelper::getString("So SIM (10 so, bat dau bang 0): ");
            setSoSim(so);
            if (tonTai(ID)) throw DuplicateIdException(ID);
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 2. Nhap Ma IMSI: 15 chu so
    while (true) {
        try {
            string imsi = InputHelper::getString("Ma IMSI (15 so): ");
            setMaIMSI(imsi);
            kiemTraTrungIMSI_IMEI(1, imsi, "Ma IMSI");
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 3. Nhap Ma loai hinh: ton tai trong sub_types.txt
    while (true) {
        try {
            string v = InputHelper::getString("Ma loai hinh (vd SUB01): ");
            setMaLoai(v);
            Repository<SubType> repo("data/sub_types.txt");
            napBangThamChieu(repo, "data/sub_types.txt");
            if (repo.findById(v) == nullptr) throw ForeignKeyException("MaLoai", v);
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 4. Nhap Ma IMEI: 15 chu so hoac NONE, ton tai trong devices.txt
    while (true) {
        try {
            string v = InputHelper::getString("Ma IMEI (15 so, hoac NONE neu chua gan may): ");
            setMaIMEI(v);
            if (v != "NONE" && !imeiTonTai(v))
                throw ForeignKeyException("MaIMEI", v);
            kiemTraTrungIMSI_IMEI(3, v, "Ma IMEI");
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 5. Nhap Ngay kich hoat: dd/mm/yyyy hop le, khong sau ngay hien tai
    while (true) {
        try {
            setNgayKichHoat(InputHelper::getString("Ngay kich hoat (dd/mm/yyyy): "));
            break;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    //6. Trang thai ban dau luon la HoatDong
    setTrangThai("HoatDong");
}

void Sim::capNhatThongTin() {
    Sim moi = *this;
    cout << "(Nhan Enter de giu nguyen gia tri hien tai. Khong duoc sua So SIM)\n";

    // 1. Sua Ma IMSI (kiem tra trung voi SIM khac)
    while (true) {
        try {
            string v = InputHelper::getStringOrDefault("Ma IMSI ", moi.getMaIMSI());
            Sim thu = moi;
            thu.setMaIMSI(v);
            kiemTraTrungIMSI_IMEI(1, thu.getMaIMSI(), "Ma IMSI", ID);
            moi = thu;
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 2. Sua Ma loai hinh
    while (true) {
        try {
            string v = InputHelper::getStringOrDefault("Ma loai hinh ", moi.getMaLoai());
            Sim thu = moi;
            thu.setMaLoai(v);
            Repository<SubType> repo("data/sub_types.txt");
            napBangThamChieu(repo, "data/sub_types.txt");
            if (repo.findById(thu.getMaLoai()) == nullptr)
                throw ForeignKeyException("MaLoai", thu.getMaLoai());
            moi = thu;
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 3. Sua Ma IMEI (kiem tra ton tai trong devices.txt va chua gan cho SIM khac)
    while (true) {
        try {
            string v = InputHelper::getStringOrDefault("Ma IMEI ", moi.getMaIMEI());
            Sim thu = moi;
            thu.setMaIMEI(v);
            if (v != "NONE" && !imeiTonTai(v))
                throw ForeignKeyException("MaIMEI", v);
            kiemTraTrungIMSI_IMEI(3, thu.getMaIMEI(), "Ma IMEI", ID);
            moi = thu;
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 4. Sua Ngay kich hoat (Khong duoc muon hon ngay Hop dong / Nap tien da co)
    while (true) {
        try {
            string v = InputHelper::getStringOrDefault("Ngay kich hoat ", moi.getNgayKichHoat());
            Sim thu = moi;
            thu.setNgayKichHoat(v);
            kiemTraHopLeNgayKichHoatMoi(ID, thu.getNgayKichHoat());
            moi = thu;
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 5. Sua Trang thai
    while (true) {
        try {
            string v = InputHelper::getStringOrDefault("Trang thai (HoatDong/BiKhoa) ", moi.getTrangThai());
            Sim thu = moi;
            thu.setTrangThai(v);
            kiemTraKhoaSim(ID, thu.getTrangThai());
            moi = thu;
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }
    *this = moi;
}

void Sim::display() const { cout << *this << '\n'; }

string Sim::toString() const {
    return ID + "|" + maIMSI + "|" + maLoai + "|" + maIMEI + "|" + ngayKichHoat + "|" + trangThai;
}

void Sim::fromString(const string& line) {
    vector<string> f = tachDong(line);
    if (f.size() != 6) throw InvalidDataException("Dong SIM sai dinh dang: " + line);
    setSoSim(f[0]);
    setMaIMSI(f[1]);
    setMaLoai(f[2]);
    setMaIMEI(f[3]);
    setNgayKichHoat(f[4]);
    setTrangThai(f[5]);
}

bool Sim::tonTai(const string& soSim) {
    Repository<Sim> repo(FILE_SIM);
    napBangThamChieu(repo, FILE_SIM);
    return repo.findById(soSim) != nullptr;
}

bool Sim::operator==(const Sim& other) const { return ID == other.ID; }
bool Sim::operator<(const Sim& other) const { return ID < other.ID; }

ostream& operator<<(ostream& os, const Sim& s) {
    os << left << setw(12) << s.ID << " | " << setw(15) << s.maIMSI << " | "
       << setw(7) << s.maLoai << " | " << setw(15) << s.maIMEI << " | "
       << setw(12) << s.ngayKichHoat << " | " << setw(10) << s.trangThai;
    return os;
}
// ============================================================================
// IN TIEU DE 
// ============================================================================
static void inTieuDe() {
    cout << left << setw(12) << "So SIM" << " | " << setw(15) << "Ma IMSI" << " | "
         << setw(7) << "Ma loai" << " | " << setw(15) << "Ma IMEI" << " | "
         << setw(12) << "Ngay KH" << " | " << setw(10) << "Trang thai" << " | Ten loai hinh\n"
         << string(95, '-') << '\n';
}
// ============================================================================
// IN DANH SACH
// ============================================================================
static void inDanhSach(const vector<Sim>& ds) {
    if (ds.empty()) {
        cout << "(Khong co SIM nao)\n";
        return;
    }
    inTieuDe();
    for (const Sim& s : ds) cout << s << " | " << tenLoai(s.getMaLoai()) << '\n';
    cout << string(95, '-') << "\nTong so: " << ds.size() << " SIM\n";
}
// ============================================================================
// IN CHI TIET
// ============================================================================
static void inChiTiet(const Sim& s) {
    cout << "So SIM        : " << s.getSoSim() << '\n'
         << "Ma IMSI       : " << s.getMaIMSI() << '\n'
         << "Loai hinh     : " << s.getMaLoai() << " - " << tenLoai(s.getMaLoai()) << '\n'
         << "Thiet bi IMEI : " << s.getMaIMEI() << " - " << tenMay(s.getMaIMEI()) << '\n'
         << "Ngay kich hoat: " << s.getNgayKichHoat() << '\n'
         << "Trang thai    : " << s.getTrangThai() << '\n';
}
// ============================================================================
// THEM SIM
// ============================================================================
static void themSim(Repository<Sim>& repo) {
    Sim s;
    s.getInformation();
    repo.add(s);
    cout << "Them SIM thanh cong.\n";
    inChiTiet(s);  
}
// ============================================================================
// TIM KIEM
// ============================================================================
static void timKiem(Repository<Sim>& repo) {
    cout << "\n--- TIM KIEM SIM ---\n"
         << "1. Theo so SIM (chinh xac, xem chi tiet)\n"
         << "2. Theo so SIM (chua chuoi, gan dung)\n"
         << "3. Theo ten loai hinh (gan dung, khong phan biet hoa thuong)\n"
         << "4. Theo khoang ngay kich hoat\n"
         << "0. Quay lai\n";
    int chon = InputHelper::getInt("Chon: ", 0, 4);
    vector<Sim> ketQua;

    if (chon == 1) {
        string so = InputHelper::getString("Nhap so SIM: ");
        InputHelper::xuly_84(so);
        Sim* p = repo.findById(so);
        if (p == nullptr) throw NotFoundException(so);
        inChiTiet(*p);
        return;
    } else if (chon == 2) {
        string q = InputHelper::getString("Nhap chuoi can tim: ");
        for (const Sim& s : repo.getAll())
            if (s.getSoSim().find(q) != string::npos) ketQua.push_back(s);
    } else if (chon == 3) {
        string q = chuThuong(InputHelper::getString("Nhap ten loai hinh can tim: "));
        for (const Sim& s : repo.getAll())
            if (chuThuong(tenLoai(s.getMaLoai())).find(q) != string::npos) ketQua.push_back(s);
    } else if (chon == 4) {
        Date tu, den;
        while (true) {
            try {
                string s = InputHelper::getString("Tu ngay (dd/mm/yyyy): ");
                tu.input(s);
                break;
            } catch (const BaseException& e) {
                cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
            }
        }
        while (true) {
            try {
                string s = InputHelper::getString("Den ngay (dd/mm/yyyy): ");
                den.input(s);
                break;
            } catch (const BaseException& e) {
                cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
            }
        }
        for (const Sim& s : repo.getAll()) {
            Date d;
            d.input(s.getNgayKichHoat());
            if (d.compare(tu) >= 0 && d.compare(den) <= 0)
                ketQua.push_back(s);
        }
    } else {
        return;
    }
    inDanhSach(ketQua);
}
// ============================================================================
// SAP XEP
// ============================================================================
static void sapXep(Repository<Sim>& repo) {
    cout << "\n--- SAP XEP SIM ---\n"
         << "1. So SIM tang dan\n"
         << "2. So SIM giam dan\n"
         << "3. Ngay kich hoat cu -> moi\n"
         << "4. Ngay kich hoat moi -> cu\n"
         << "0. Quay lai\n";
    int chon = InputHelper::getInt("Chon: ", 0, 4);
    vector<Sim> ds = repo.getAll();   // sap xep ban sao, khong doi thu tu trong file

    auto truocVeNgay = [](const Sim& a, const Sim& b) {
        Date da, db;
        da.input(a.getNgayKichHoat());
        db.input(b.getNgayKichHoat());
        return da.compare(db) < 0;
    };
    if (chon == 1) sort(ds.begin(), ds.end());                       
    else if (chon == 2) sort(ds.rbegin(), ds.rend());
    else if (chon == 3) sort(ds.begin(), ds.end(), truocVeNgay);
    else if (chon == 4) sort(ds.rbegin(), ds.rend(), truocVeNgay);
    else return;
    inDanhSach(ds);
}
// ============================================================================
// CAP NHAT SIM
// ============================================================================
static void capNhatSim(Repository<Sim>& repo) {
    string so = InputHelper::getString("Nhap so SIM can sua: ");
    InputHelper::xuly_84(so);
    Sim* p = repo.findById(so);
    if (p == nullptr) throw NotFoundException(so);
    inChiTiet(*p);
    Sim moi = *p;               // sua tren ban sao, xong moi ghi vao repo
    moi.capNhatThongTin();
    repo.update(so, moi);
    cout << "Cap nhat SIM thanh cong.\n";
}
// ============================================================================
// XOA SIM 
// ============================================================================
static void xoaSim(Repository<Sim>& repo) {
    string so = InputHelper::getString("Nhap so SIM can xoa: ");
    InputHelper::xuly_84(so);
    Sim* p = repo.findById(so);
    if (p == nullptr) throw NotFoundException(so);
    inChiTiet(*p);
    kiemTraRangBuocXoa(so);
    if (!InputHelper::getConfirm("Ban chac chan muon xoa SIM nay? (y/n): ")) {
        cout << "Da huy xoa.\n";
        return;
    }
    repo.remove(so);
    cout << "Xoa SIM thanh cong.\n";
}

void menuSim() {
    Repository<Sim> repo(FILE_SIM);
    try {
        repo.loadFromFile();
    } catch (const exception& e) {
        cout << "[Loi] Khong nap duoc du lieu SIM: " << e.what() << '\n';
        return;
    }

    int chon = -1;
    do {
        cout << "\n============ QUAN LY SIM ============\n"
             << "  1. Them SIM moi\n"
             << "  2. Xem danh sach SIM\n"
             << "  3. Tim kiem SIM\n"
             << "  4. Sap xep danh sach\n"
             << "  5. Cap nhat thong tin SIM\n"
             << "  6. Xoa SIM\n"
             << "  0. Quay lai menu chinh\n"
             << "=====================================\n";
        chon = InputHelper::getInt("Nhap lua chon: ", 0, 6);
        try {
            switch (chon) {
                case 1: themSim(repo); break;
                case 2: inDanhSach(repo.getAll()); break;
                case 3: timKiem(repo); break;
                case 4: sapXep(repo); break;
                case 5: capNhatSim(repo); break;
                case 6: xoaSim(repo); break;
                default: break;
            }
        } catch (const exception& e) {
            cout << "[Loi] " << e.what() << '\n';
        }
    } while (chon != 0);
}
