// ===============================================================
// Hoc vien Cong nghe Buu chinh Vien thong
// Hoc phan: Ky thuat lap trinh
// De tai 1: Quan ly thue bao di dong
// Tac gia: Vu Quang Anh
// Use Case: Quan Bill thanh toán
// ===============================================================
#include "Bill.h"
#include "Date.h"
#include "Exceptions.h"
#include "InputHelper.h"
#include "Repository.h"
#include "Sim.h"
#include "CDR.h"
#include "SubType.h"
#include "Contract.h"
#include "Package.h"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;

static const string FILE_BILL = "data/bills.txt";
static const long long TIEN_TOI_DA = 1000000000000LL;

// ============================================================================
// HAM BO TRO
// ============================================================================
// Doi "mm/yyyy" thanh thang, nam
static bool phanTichThangNam(const string& s, int& thang, int& nam) {
    if (s.size() != 7 || s[2] != '/') return false;
    try {
        Date d;
        d.input("01/" + s);
        thang = stoi(s.substr(0, 2));
        nam = stoi(s.substr(3, 4));
        return true;
    } catch (...) {
        return false;
    }
}

// Kiem tra thang/nam hoa don (mm/yyyy) khong duoc truoc thang/nam kich hoat cua SIM
static void kiemTraThangHopLeVoiSIM(const string& soSim,const string& thangNamHD) {
    Repository<Sim> simRepo("data/sims.txt");
    simRepo.loadFromFile();

    Sim* sim = simRepo.findById(soSim);
    if (sim == nullptr)
        throw ForeignKeyException("SoSim", soSim);

    Date ngayKichHoat;
    ngayKichHoat.input(sim->getNgayKichHoat());

    // So sánh ngày đầu của hai tháng để bỏ qua ngày trong tháng.
    Date thangKichHoat;
    thangKichHoat.input("01/" + ngayKichHoat.output().substr(3));

    Date thangHoaDon;
    thangHoaDon.input("01/" + thangNamHD);

    if (thangHoaDon.compare(thangKichHoat) < 0) {
        throw InvalidDataException("Thang/nam hoa don (" + thangNamHD+ ") khong duoc truoc thang kich hoat SIM ("+ ngayKichHoat.output().substr(3) + ")!");
    }
}
static bool laChuSo(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(),
                                [](unsigned char c) { return isdigit(c) != 0; });
}
// Khoa so sanh thang/nam: nam * 100 + thang 
static int khoaThang(const string& thangNam) {
    int t = 0, n = 0;
    return phanTichThangNam(thangNam, t, n) ? n * 100 + t : 0;
}

static int khoaThangHienTai() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    return (lt->tm_year + 1900) * 100 + (lt->tm_mon + 1);
}

// Moi SIM chi co mot hoa don trong mot thang (bo qua chinh hoa don dang sua)
static void kiemTraTrungKy(Repository<Bill>& repo, const string& soSim,const string& thangNam, const string& maHDBoQua) {
    for (const Bill& b : repo.getAll())
        if (b.getSoSim() == soSim && b.getThangNam() == thangNam && b.getMaHD() != maHDBoQua)
            throw InvalidDataException("SIM " + soSim + " da co hoa don " + b.getMaHD() + " trong thang " + thangNam);
}

static void kiemTraSimTonTai(const string& soSim) {
    if (!Sim::tonTai(soSim)) throw ForeignKeyException("SoSim", soSim);
}
// Hàm bổ trợ tự động tính tổng tiền hóa đơn trong Bill
long long tinhTongTienHoaDon(const string& soSim, const string& thangNam) {
    long long tongTien = 0;

    // 1. Cong cuoc loai hinh thue bao
    string maLoai = "";
    Repository<Sim> simRepo("data/sims.txt");
    simRepo.loadFromFile();
    Sim* sim = simRepo.findById(soSim);
    if (sim != nullptr) {
        maLoai = sim->getMaLoai();
        Repository<SubType> typeRepo("data/sub_types.txt");
        typeRepo.loadFromFile();
        SubType* type = typeRepo.findById(maLoai);
        if (type != nullptr) 
            tongTien += type->getCuocThang();
    }

    // 2. Cong cuoc goi dich vu (Tu Contract -> Package)
    Repository<Contract> contractRepo("data/contracts.txt");
    contractRepo.loadFromFile();
    for (const auto& hd : contractRepo.getAll()) {
        if (hd.getPhoneNumber() == soSim && hd.getStatus() == "HieuLuc") {
            Repository<Package> pkgRepo("data/packages.txt");
            pkgRepo.loadFromFile();
            Package* pkg = pkgRepo.findById(hd.getPhonePlan());
            if (pkg != nullptr) 
                tongTien += pkg->getPrice();
            break;
        }
    }

    // 3. Cong cuoc goi phat sinh trong thang
    Repository<CDR> cdrRepo("data/cdrs.txt");
    cdrRepo.loadFromFile();
    for (const auto& cdr : cdrRepo.getAll()) {
        if (cdr.getSoGoi() == soSim) {
            string thoiDiem = cdr.getThoiDiem(); 
            string thangNamCDR = thoiDiem.substr(3, 7); 
            if (thangNamCDR == thangNam) {
                tongTien += stoll(cdr.getCuocPhi()); 
            }
        }
    }
    return tongTien;
}
// ============================================================================
// CONSTRUCTOR
// ============================================================================
Bill::Bill() : soSim(""), thangNam(""), tongTien(0), trangThai("ChuaThanhToan") {}
Bill::Bill(const string& maHD, const string& so, const string& tn,
           long long tien, const string& tt) {
    setMaHD(maHD);
    setSoSim(so);
    setThangNam(tn);
    setTongTien(tien);
    setTrangThai(tt);
}
// ============================================================================
// GETTER/SETTER
// ============================================================================
string Bill::getMaHD() const { return ID; }
string Bill::getSoSim() const { return soSim; }
string Bill::getThangNam() const { return thangNam; }
long long Bill::getTongTien() const { return tongTien; }
string Bill::getTrangThai() const { return trangThai; }

void Bill::setMaHD(const string& v) {
    if (!InputHelper::checkidvalid(v, "BILL")) {
        throw InvalidDataException("Ma hoa don phai co dang BILL + so (vd BILL01): " + v);
    }
    ID = v;
}

void Bill::setSoSim(const string& v) {
    string phone = v;
    InputHelper::xuly_84(phone); // Chuan hoa +84 thanh 0 dung ham co san trong InputHelper
    if (!InputHelper::checkPhoneNumber(phone))
        throw InvalidDataException("So SIM phai gom 10 chu so va bat dau bang 0");
    soSim = phone;
}

void Bill::setThangNam(const string& v) {
    int t = 0, n = 0;
    if (!phanTichThangNam(v, t, n))
        throw InvalidDataException("Thang/nam phai dung dang mm/yyyy va hop le: " + v);
    if (n * 100 + t > khoaThangHienTai())
        throw InvalidDataException("Thang/nam khong duoc sau thang hien tai");
    thangNam = v;
}

void Bill::setTongTien(long long v) {
    if (v < 0 || v > TIEN_TOI_DA)
        throw InvalidDataException("Tong tien phai tu 0 den 1.000.000.000.000 VND");
    tongTien = v;
}

void Bill::setTrangThai(const string& v) {
    if (v != "DaThanhToan" && v != "ChuaThanhToan")
        throw InvalidDataException("Trang thai chi duoc la DaThanhToan hoac ChuaThanhToan");
    trangThai = v;
}
// ============================================================================
// KE THUA TU ENTITY
// ============================================================================
void Bill::getInformation() {
    Repository<Bill> repo(FILE_BILL);
    repo.loadFromFile();   // de kiem tra trung ma va trung ky hoa don

    string ma = InputHelper::generateAutoID(repo, "BILL", "data_counter/bill_counter.txt");
    setMaHD(ma);
    cout << "Ma hoa don (tu dong sinh): " << ma << '\n';

    // 2. Nhap So SIM
    while (true) {
        try {
            string so = InputHelper::getString("So SIM: ");
            setSoSim(so);
            kiemTraSimTonTai(soSim);
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 3. Nhap Thang/nam
    while (true) {
        try {
            string tn = InputHelper::getString("Thang/nam (mm/yyyy): ");
            setThangNam(tn);
            kiemTraTrungKy(repo, soSim, tn, "");
            kiemTraThangHopLeVoiSIM(soSim, tn);
                break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
                cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    long long tongDuTinh = tinhTongTienHoaDon(soSim, thangNam);
    setTongTien(tongDuTinh);
    cout << "Tong tien (He thong tu dong tinh): " << tongDuTinh << " VND\n";

    // 5. Nhap Trang thai
    while (true) {
        try {
            setTrangThai(InputHelper::getString("Trang thai (DaThanhToan/ChuaThanhToan): "));
            break;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }
}

void Bill::capNhatThongTin() {
    Repository<Bill> repo(FILE_BILL);
    repo.loadFromFile();
    Bill moi = *this;

    cout << "(Nhan Enter de giu nguyen gia tri hien tai. Khong duoc sua Ma hoa don)\n";

    // 1. Sua So SIM
    while (true) {
        try {
            string so = InputHelper::getStringOrDefault("So SIM ", moi.getSoSim());
            Bill thu = moi;
            thu.setSoSim(so);
            kiemTraSimTonTai(thu.getSoSim());
            moi = thu;
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // 2. Sua Thang/nam
    while (true) {
        try {
            string tn = InputHelper::getStringOrDefault("Thang/nam (mm/yyyy) ", moi.getThangNam());
            Bill thu = moi;
            thu.setThangNam(tn);
            kiemTraTrungKy(repo, thu.getSoSim(), thu.getThangNam(), thu.getMaHD());
            kiemTraThangHopLeVoiSIM(thu.getSoSim(), thu.getThangNam());
            moi = thu;
            break;
        } catch (const FileException&) {
            throw;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }

    // Tong tien chi duoc tinh tu du lieu SIM, goi cuoc va CDR.
    moi.setTongTien(tinhTongTienHoaDon(moi.getSoSim(), moi.getThangNam()));
    cout << "Tong tien (He thong tu dong tinh): " << moi.getTongTien() << " VND\n";

    // 3. Sua Trang thai
    while (true) {
        try {
            moi.setTrangThai(InputHelper::getStringOrDefault("Trang thai (DaThanhToan/ChuaThanhToan) ", moi.getTrangThai()));
            break;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }
    *this = moi;
}

void Bill::display() const { cout << *this << '\n'; }

string Bill::toString() const {
    return ID + "|" + soSim + "|" + thangNam + "|" + to_string(tongTien) + "|" + trangThai;
}

void Bill::fromString(const string& line) {
    string l = line;
    if (!l.empty() && l.back() == '\r') l.pop_back();
    vector<string> f;
    stringstream ss(l);
    string t;
    while (getline(ss, t, '|')) f.push_back(t);
    if (f.size() != 5 || !laChuSo(f[3]) || f[3].size() > 13)
        throw InvalidDataException("Dong hoa don sai dinh dang: " + line);
    setMaHD(f[0]);
    setSoSim(f[1]);
    setThangNam(f[2]);
    setTongTien(stoll(f[3]));
    setTrangThai(f[4]);
}

bool Bill::operator==(const Bill& other) const { return ID == other.ID; }

bool Bill::operator<(const Bill& other) const {
    if (ID.size() != other.ID.size()) return ID.size() < other.ID.size();   // BILL9 < BILL10
    return ID < other.ID;
}

ostream& operator<<(ostream& os, const Bill& b) {
    os << left << setw(10) << b.ID << " | " << setw(12) << b.soSim << " | "
       << setw(9) << b.thangNam << " | " << right << setw(14) << b.tongTien << " | "
       << left << b.trangThai;
    return os;
}

// ============================================================================
// IN TIEU DE
// ============================================================================
static void inTieuDe() {
    cout << left << setw(10) << "Ma HD" << " | " << setw(12) << "So SIM" << " | "
         << setw(9) << "Thang/nam" << " | " << right << setw(14) << "Tong tien" << " | "
         << left << "Trang thai\n" << string(70, '-') << '\n';
}
// ============================================================================
// IN DANH SACH
// ============================================================================
static void inDanhSach(const vector<Bill>& ds) {
    if (ds.empty()) {
        cout << "(Khong co hoa don nao)\n";
        return;
    }
    inTieuDe();
    long long tong = 0;
    for (const Bill& b : ds) {
        cout << b << '\n';
        tong += b.getTongTien();
    }
    cout << string(70, '-') << "\nTong so: " << ds.size() << " hoa don, tong tien: " << tong << " VND\n";
}

static void inChiTiet(const Bill& b) {
    cout << "Ma hoa don : " << b.getMaHD() << '\n'
         << "So SIM     : " << b.getSoSim() << '\n'
         << "Thang/nam  : " << b.getThangNam() << '\n'
         << "Tong tien  : " << b.getTongTien() << " VND\n"
         << "Trang thai : " << b.getTrangThai() << '\n';
}

// Nhap mot thang/nam hop le (bat nhap lai neu sai), tra ve khoa nam*100+thang
static int docKhoaThang(const string& loiNhac) {
    while (true) {
        try {
            string s = InputHelper::getString(loiNhac);
            int t = 0, n = 0;
            if (!phanTichThangNam(s, t, n))
                throw InvalidDataException("Thang/nam phai dung dang mm/yyyy va hop le");
            return n * 100 + t;
        } catch (const BaseException& e) {
            cout << "[Loi] " << e.what() << ". Vui long nhap lai.\n";
        }
    }
}
// ============================================================================
// THEM HOA DON
// ============================================================================
static void themBill(Repository<Bill>& repo) {
    Bill b;
    b.getInformation();
    repo.add(b);
    cout << "Them hoa don thanh cong.\n";
}
// ============================================================================
// TIM KIEM
// ============================================================================
static void timKiem(Repository<Bill>& repo) {
    cout << "\n--- TIM KIEM HOA DON ---\n"
         << "1. Theo ma hoa don (chinh xac, xem chi tiet)\n"
         << "2. Theo so SIM (chua chuoi, gan dung)\n"
         << "3. Theo trang thai thanh toan\n"
         << "4. Theo khoang thang/nam\n"
         << "0. Quay lai\n";
    int chon = InputHelper::getInt("Chon: ", 0, 4);
    vector<Bill> ketQua;

    if (chon == 1) {
        string ma = InputHelper::getString("Nhap ma hoa don: ");
        Bill* p = repo.findById(ma);
        if (p == nullptr) throw NotFoundException(ma);
        inChiTiet(*p);
        return;
    } else if (chon == 2) {
        string q = InputHelper::getString("Nhap chuoi can tim: ");
        InputHelper::xuly_84(q);
        for (const Bill& b : repo.getAll())
            if (b.getSoSim().find(q) != string::npos) ketQua.push_back(b);
    } else if (chon == 3) {
        int c = InputHelper::getInt("1. DaThanhToan  2. ChuaThanhToan: ", 1, 2);
        string tt = (c == 1) ? "DaThanhToan" : "ChuaThanhToan";
        for (const Bill& b : repo.getAll())
            if (b.getTrangThai() == tt) ketQua.push_back(b);
    } else if (chon == 4) {
        int tu = docKhoaThang("Tu thang/nam (mm/yyyy): ");
        int den = docKhoaThang("Den thang/nam (mm/yyyy): ");
        for (const Bill& b : repo.getAll()) {
            int k = khoaThang(b.getThangNam());
            if (k >= tu && k <= den) ketQua.push_back(b);
        }
    } else {
        return;
    }
    inDanhSach(ketQua);
}
// ============================================================================
// SAP XEP
// ============================================================================
static void sapXep(Repository<Bill>& repo) {
    cout << "\n--- SAP XEP HOA DON ---\n"
         << "1. Ma hoa don tang dan\n"
         << "2. Tong tien tang dan\n"
         << "3. Tong tien giam dan\n"
         << "4. Thang/nam cu -> moi\n"
         << "0. Quay lai\n";
    int chon = InputHelper::getInt("Chon: ", 0, 4);
    vector<Bill> ds = repo.getAll();   // sap xep ban sao, khong doi thu tu trong file

    if (chon == 1) {
        sort(ds.begin(), ds.end());                                     // operator<
    } else if (chon == 2) {
        sort(ds.begin(), ds.end(), [](const Bill& a, const Bill& b) { return a.getTongTien() < b.getTongTien(); });
    } else if (chon == 3) {
        sort(ds.begin(), ds.end(), [](const Bill& a, const Bill& b) { return a.getTongTien() > b.getTongTien(); });
    } else if (chon == 4) {
        sort(ds.begin(), ds.end(), [](const Bill& a, const Bill& b) {
            return khoaThang(a.getThangNam()) < khoaThang(b.getThangNam());
        });
    } else {
        return;
    }
    inDanhSach(ds);
}
// ============================================================================
// CAP NHAT HOA DON
// ============================================================================
static void capNhatBill(Repository<Bill>& repo) {
    string ma = InputHelper::getString("Nhap ma hoa don can sua: ");
    Bill* p = repo.findById(ma);
    if (p == nullptr) throw NotFoundException(ma);
    inChiTiet(*p);
    Bill moi = *p;              // sua tren ban sao, xong moi ghi vao repo
    moi.capNhatThongTin();
    repo.update(ma, moi);
    cout << "Cap nhat hoa don thanh cong.\n";
}

static void xoaBill(Repository<Bill>& repo) {
    while (true) {
        string ma = InputHelper::getString("Nhap ma hoa don can xoa (0 de quay lai): ");
        if (ma == "0") return;
        try {
            Bill* p = repo.findById(ma);
            if (p == nullptr) {
                cout << "Khong tim thay hoa don " << ma << ". Vui long nhap lai.\n";
                continue;
            }
            inTieuDe();
            inChiTiet(*p);
            if (p->getTrangThai() == "ChuaThanhToan") {
                cout << "[Loi nghiep vu] Khong the xoa hoa don dang 'ChuaThanhToan'! Vui long thanh toan hoac cap nhat trang thai truoc.\n";
                return;
            }
            if (!InputHelper::getConfirm("Ban chac chan muon xoa hoa don nay? (y/n): ")) {
                cout << "Da huy xoa.\n";
                return;
            }
            repo.remove(ma);
            cout << "Xoa hoa don thanh cong.\n";
            return;
        } catch (const exception& e) {
            cout << "[Loi] " << e.what() << '\n';
        }
    }
}

void menuHoaDon() {
    Repository<Bill> repo(FILE_BILL);
    try {
        repo.loadFromFile();
    } catch (const exception& e) {
        cout << "[Loi] Khong nap duoc du lieu hoa don: " << e.what() << '\n';
        return;
    }

    int chon = -1;
    do {
        cout << "\n=========== QUAN LY HOA DON ===========\n"
             << "  1. Them hoa don moi\n"
             << "  2. Xem danh sach hoa don\n"
             << "  3. Tim kiem hoa don\n"
             << "  4. Sap xep danh sach\n"
             << "  5. Cap nhat thong tin hoa don\n"
             << "  6. Xoa hoa don\n"
             << "  0. Quay lai menu chinh\n"
             << "=======================================\n";
        chon = InputHelper::getInt("Nhap lua chon: ", 0, 6);
        try {
            switch (chon) {
                case 1: themBill(repo); break;
                case 2: inDanhSach(repo.getAll()); break;
                case 3: timKiem(repo); break;
                case 4: sapXep(repo); break;
                case 5: capNhatBill(repo); break;
                case 6: xoaBill(repo); break;
                default: break;
            }
        } catch (const exception& e) {
            cout << "[Loi] " << e.what() << '\n';
        }
    } while (chon != 0);
}
