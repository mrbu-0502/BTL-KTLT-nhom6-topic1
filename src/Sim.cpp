
#include "Sim.h"
#include "Date.h"
#include "Exceptions.h"
#include "InputHelper.h"
#include "Repository.h"

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

// Tim giaTri o cot `cot` (tinh tu 0) trong file. Thay thi tra ve true va (tuy chon) ca dong.
// Khong mo duoc file -> nem FileException.
static bool timTrongFile(const string& file, int cot, const string& giaTri,
                         vector<string>* dong = nullptr) {
    ifstream in(file);
    if (!in.is_open()) throw FileException(file);
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        vector<string> f = tachDong(line);
        if (cot < static_cast<int>(f.size()) && f[cot] == giaTri) {
            if (dong) *dong = f;
            return true;
        }
    }
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

// Doi "dd/mm/yyyy" thanh Date, dung Date::valid() cua Hiep. Sai dinh dang/khong hop le -> false
static bool phanTichNgay(const string& s, Date& ketQua) {
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
    string ds = s.substr(0, 2), ms = s.substr(3, 2), ys = s.substr(6, 4);
    if (!laChuSo(ds) || !laChuSo(ms) || !laChuSo(ys)) return false;
    Date d(stoi(ds), stoi(ms), stoi(ys));
    if (!d.valid()) return false;
    ketQua = d;
    return true;
}

static Date homNay() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    return Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

// Khong cho xoa SIM neu con bang khac tham chieu den no
static void kiemTraRangBuocXoa(const string& soSim) {
    struct Rang { const char* file; int cot; const char* ten; };
    const Rang dsRang[] = {
        {"data/bills.txt", 1, "Bill"},
        {"data/contracts.txt", 2, "Contract"},
        {"data/cdrs.txt", 1, "CDR"},
        {"data/recharges.txt", 1, "Recharge"},
    };
    for (const Rang& r : dsRang)
        if (timTrongFile(r.file, r.cot, soSim))
            throw ReferencedException("SIM", soSim, r.ten);
}

// Tra ten loai hinh / ten may tu ma (hien thi cho de doc)
static string tenLoai(const string& maLoai) {
    vector<string> f;
    if (timTrongFile("data/sub_types.txt", 0, maLoai, &f) && f.size() > 1) return f[1];
    return "(khong ro)";
}

static string tenMay(const string& maIMEI) {
    if (maIMEI == "NONE") return "(chua gan may)";
    vector<string> f;
    if (timTrongFile("data/devices.txt", 0, maIMEI, &f) && f.size() > 1) return f[1];
    return "(khong ro)";
}

// ============================================================================
// PHAN 1: LOP DU LIEU
// ============================================================================
Sim::Sim() : maIMSI(""), maLoai(""), maIMEI("NONE"), ngayKichHoat(""), trangThai("HoatDong") {}

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
    if (v.empty() || !InputHelper::checkPhoneNumber(v))
        throw InvalidDataException("So SIM phai gom 10 chu so va bat dau bang 0");
    ID = v;
}

void Sim::setMaIMSI(const string& v) { // international mobile subcriver identity - mã ở trên phôi sim
    if (v.size() != 15 || !laChuSo(v))
        throw InvalidDataException("Ma IMSI phai gom dung 15 chu so");
    maIMSI = v;
}

void Sim::setMaLoai(const string& v) { // loai sim
    if (v.empty() || v.find('|') != string::npos)
        throw InvalidDataException("Ma loai hinh khong duoc rong hoac chua ky tu '|'");
    maLoai = v;
}

void Sim::setMaIMEI(const string& v) { // mã cái điện thoại
    if (v != "NONE" && (v.size() != 15 || !laChuSo(v)))
        throw InvalidDataException("Ma IMEI phai gom 15 chu so (hoac NONE neu chua gan may)");
    maIMEI = v;
}

void Sim::setNgayKichHoat(const string& v) {
    Date d;
    if (!phanTichNgay(v, d))
        throw InvalidDataException("Ngay kich hoat phai dung dang dd/mm/yyyy va hop le: " + v);
    if (d.compare(homNay()) > 0)
        throw InvalidDataException("Ngay kich hoat khong duoc sau ngay hien tai");
    ngayKichHoat = v;
}

void Sim::setTrangThai(const string& v) {
    if (v != "HoatDong" && v != "BiKhoa")
        throw InvalidDataException("Trang thai chi duoc la HoatDong hoac BiKhoa");
    trangThai = v;
}

// ----- Ham thuan ao tu Entity -----
void Sim::getInformation() {
    // So SIM: dung dinh dang va chua ton tai trong file
    nhapLaiDenKhiHopLe([&] {
        string so = InputHelper::getString("So SIM (10 so, bat dau bang 0): ");
        setSoSim(so);
        if (tonTai(so)) throw DuplicateIdException(so);
    });
    nhapLaiDenKhiHopLe([&] { setMaIMSI(InputHelper::getString("Ma IMSI (15 so): ")); });
    nhapLaiDenKhiHopLe([&] {
        string v = InputHelper::getString("Ma loai hinh (vd SUB01): ");
        setMaLoai(v);
        if (!timTrongFile("data/sub_types.txt", 0, v)) throw ForeignKeyException("MaLoai", v);
    });
    nhapLaiDenKhiHopLe([&] {
        string v = InputHelper::getString("Ma IMEI (15 so, hoac NONE neu chua gan may): ");
        setMaIMEI(v);
        if (v != "NONE" && !timTrongFile("data/devices.txt", 0, v))
            throw ForeignKeyException("MaIMEI", v);
    });
    nhapLaiDenKhiHopLe([&] {
        setNgayKichHoat(InputHelper::getString("Ngay kich hoat (dd/mm/yyyy): "));
    });
    nhapLaiDenKhiHopLe([&] {
        setTrangThai(InputHelper::getString("Trang thai (HoatDong/BiKhoa): "));
    });
}

void Sim::capNhatThongTin() {
    cout << "(Nhan Enter de giu nguyen gia tri hien tai. Khong duoc sua So SIM)\n";
    nhapLaiDenKhiHopLe([&] {
        setMaIMSI(InputHelper::getStringOrDefault("Ma IMSI ", maIMSI));
    });
    nhapLaiDenKhiHopLe([&] {
        string v = InputHelper::getStringOrDefault("Ma loai hinh ", maLoai);
        Sim().setMaLoai(v);   // kiem tra truoc, chua doi gia tri hien tai
        if (!timTrongFile("data/sub_types.txt", 0, v)) throw ForeignKeyException("MaLoai", v);
        setMaLoai(v);
    });
    nhapLaiDenKhiHopLe([&] {
        string v = InputHelper::getStringOrDefault("Ma IMEI ", maIMEI);
        Sim().setMaIMEI(v);   // kiem tra truoc, chua doi gia tri hien tai
        if (v != "NONE" && !timTrongFile("data/devices.txt", 0, v))
            throw ForeignKeyException("MaIMEI", v);
        setMaIMEI(v);
    });
    nhapLaiDenKhiHopLe([&] {
        setNgayKichHoat(InputHelper::getStringOrDefault("Ngay kich hoat ", ngayKichHoat));
    });
    nhapLaiDenKhiHopLe([&] {
        setTrangThai(InputHelper::getStringOrDefault("Trang thai (HoatDong/BiKhoa) ", trangThai));
    });
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

bool Sim::tonTai(const string& soSim) { return timTrongFile(FILE_SIM, 0, soSim); }

bool Sim::operator==(const Sim& other) const { return ID == other.ID; }
bool Sim::operator<(const Sim& other) const { return ID < other.ID; }

ostream& operator<<(ostream& os, const Sim& s) {
    os << left << setw(12) << s.ID << " | " << setw(15) << s.maIMSI << " | "
       << setw(7) << s.maLoai << " | " << setw(15) << s.maIMEI << " | "
       << setw(12) << s.ngayKichHoat << " | " << setw(10) << s.trangThai;
    return os;
}

// ============================================================================
// PHAN 2: NGHIEP VU CRUD VA MENU CON
// ============================================================================
static void inTieuDe() {
    cout << left << setw(12) << "So SIM" << " | " << setw(15) << "Ma IMSI" << " | "
         << setw(7) << "Ma loai" << " | " << setw(15) << "Ma IMEI" << " | "
         << setw(12) << "Ngay KH" << " | " << setw(10) << "Trang thai" << " | Ten loai hinh\n"
         << string(95, '-') << '\n';
}

static void inDanhSach(const vector<Sim>& ds) {
    if (ds.empty()) {
        cout << "(Khong co SIM nao)\n";
        return;
    }
    inTieuDe();
    for (const Sim& s : ds) cout << s << " | " << tenLoai(s.getMaLoai()) << '\n';
    cout << string(95, '-') << "\nTong so: " << ds.size() << " SIM\n";
}

static void inChiTiet(const Sim& s) {
    cout << "So SIM        : " << s.getSoSim() << '\n'
         << "Ma IMSI       : " << s.getMaIMSI() << '\n'
         << "Loai hinh     : " << s.getMaLoai() << " - " << tenLoai(s.getMaLoai()) << '\n'
         << "Thiet bi IMEI : " << s.getMaIMEI() << " - " << tenMay(s.getMaIMEI()) << '\n'
         << "Ngay kich hoat: " << s.getNgayKichHoat() << '\n'
         << "Trang thai    : " << s.getTrangThai() << '\n';
}

static void themSim(Repository<Sim>& repo) {
    Sim s;
    s.getInformation();
    repo.add(s);
    cout << "Them SIM thanh cong.\n";
}

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
        tu.input("Tu ngay (dd/mm/yyyy): ");
        den.input("Den ngay (dd/mm/yyyy): ");
        for (const Sim& s : repo.getAll()) {
            Date d;
            if (phanTichNgay(s.getNgayKichHoat(), d) && d.compare(tu) >= 0 && d.compare(den) <= 0)
                ketQua.push_back(s);
        }
    } else {
        return;
    }
    inDanhSach(ketQua);
}

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
        phanTichNgay(a.getNgayKichHoat(), da);
        phanTichNgay(b.getNgayKichHoat(), db);
        return da.compare(db) < 0;
    };
    if (chon == 1) sort(ds.begin(), ds.end());                       // operator<
    else if (chon == 2) sort(ds.rbegin(), ds.rend());
    else if (chon == 3) sort(ds.begin(), ds.end(), truocVeNgay);
    else if (chon == 4) sort(ds.rbegin(), ds.rend(), truocVeNgay);
    else return;
    inDanhSach(ds);
}

static void capNhatSim(Repository<Sim>& repo) {
    string so = InputHelper::getString("Nhap so SIM can sua: ");
    Sim* p = repo.findById(so);
    if (p == nullptr) throw NotFoundException(so);
    inChiTiet(*p);
    Sim moi = *p;               // sua tren ban sao, xong moi ghi vao repo
    moi.capNhatThongTin();
    repo.update(so, moi);
    cout << "Cap nhat SIM thanh cong.\n";
}

static void xoaSim(Repository<Sim>& repo) {
    string so = InputHelper::getString("Nhap so SIM can xoa: ");
    Sim* p = repo.findById(so);
    if (p == nullptr) throw NotFoundException(so);
    inChiTiet(*p);
    kiemTraRangBuocXoa(so);     // con Bill/Contract/CDR/Recharge tham chieu -> nem ReferencedException
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