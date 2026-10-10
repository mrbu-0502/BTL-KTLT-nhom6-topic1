#include "SubType.h"
#include "Repository.h"
#include "InputHelper.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// ============================================================================
// CONSTRUCTOR
// ============================================================================
SubType::SubType() : Entity(""), tenLoai(""), moTa(""), cuocThang(0.0) {}

SubType::SubType(string maLoai, string tenLoai, string moTa, double cuocThang)
    : Entity(maLoai), tenLoai(tenLoai), moTa(moTa), cuocThang(cuocThang) {}

// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
string SubType::getID() const {
    return ID;
}

void SubType::getInformation() {
    setID(InputHelper::getString("Nhap ma loai hinh (vi du: SUB01): ", false));
    setTenLoai(InputHelper::getString("Nhap ten loai hinh: ", false));
    setMoTa(InputHelper::getString("Nhap mo ta: ", false));
    setCuocThang(InputHelper::getDouble("Nhap cuoc phi thang (VND): ", 0.0));
}

void SubType::display() const {
    cout << left << setw(10) << ID
         << setw(25) << tenLoai
         << setw(45) << moTa
         << right << setw(12) << fixed << setprecision(0) << cuocThang << endl;
}

string SubType::toString() const {
    stringstream ss;
    ss << ID << "|" << tenLoai << "|" << moTa << "|" << fixed << setprecision(0) << cuocThang;
    return ss.str();
}

void SubType::fromString(const string& line) {
    stringstream ss(line);
    string cuocStr;

    getline(ss, ID, '|');
    getline(ss, tenLoai, '|');
    getline(ss, moTa, '|');
    getline(ss, cuocStr, '|');

    try {
        if (!cuocStr.empty()) {
            cuocThang = stod(cuocStr);
        } else {
            cuocThang = 0.0;
        }
    } catch (...) {
        cuocThang = 0.0;
    }
}

// ============================================================================
// GETTER & SETTER
// ============================================================================
string SubType::getTenLoai() const {
    return tenLoai;
}

string SubType::getMoTa() const {
    return moTa;
}

double SubType::getCuocThang() const {
    return cuocThang;
}

void SubType::setTenLoai(const string& ten) {
    if (!ten.empty()) {
        tenLoai = ten;
    }
}

void SubType::setMoTa(const string& mt) {
    if (!mt.empty()) {
        moTa = mt;
    }
}

void SubType::setCuocThang(double cuoc) {
    if (cuoc >= 0.0) {
        cuocThang = cuoc;
    }
}

// ============================================================================
// CAC HAM HO TRO GIAO DIEN CONSOLE (IN TIEU DE BANG)
// ============================================================================
static void inTieuDeBangSubType() {
    cout << string(95, '-') << endl;
    cout << left << setw(10) << "Ma Loai"
         << setw(25) << "Ten Loai Hinh"
         << setw(45) << "Mo Ta"
         << right << setw(12) << "Cuoc Thang (d)" << endl;
    cout << string(95, '-') << endl;
}

// ============================================================================
// THAO TAC CRUD
// ============================================================================
static void xemDanhSachSubType(Repository<SubType>& repo) {
    vector<SubType> list = repo.getAll();
    if (list.empty()) {
        cout << "Danh sach hien dang trong!\n";
        return;
    }
    cout << "\n=== DANH SACH LOAI HINH THUE BAO (" << list.size() << ") ===\n";
    inTieuDeBangSubType();
    for (size_t i = 0; i < list.size(); i++) {
        list[i].display();
    }
    cout << string(95, '-') << endl;
}

static void themMoiSubType(Repository<SubType>& repo) {
    cout << "\n=== THEM MOI LOAI HINH THUE BAO ===\n";
    string id = InputHelper::getString("Nhap ma loai hinh (vi du: SUB11): ", false);
    if (repo.findById(id) != nullptr) {
        cout << "[Loi] Ma loai hinh '" << id << "' da ton tai!\n";
        return;
    }

    string ten = InputHelper::getString("Nhap ten loai hinh: ", false);
    string moTa = InputHelper::getString("Nhap mo ta: ", false);
    double cuoc = InputHelper::getDouble("Nhap cuoc phi thang (VND): ", 0.0);

    SubType st(id, ten, moTa, cuoc);
    repo.add(st);
    cout << "Them loai hinh thue bao thanh cong!\n";
}

static void timKiemSubType(Repository<SubType>& repo) {
    cout << "\n=== TIM KIEM LOAI HINH THEO MA ===\n";
    string id = InputHelper::getString("Nhap ma loai hinh can tim: ", false);
    SubType* p = repo.findById(id);
    if (p == nullptr) {
        cout << "[Thong bao] Khong tim thay loai hinh co ma: " << id << endl;
        return;
    }
    cout << "\nKet qua tim thay:\n";
    inTieuDeBangSubType();
    p->display();
    cout << string(95, '-') << endl;
}

static void suaSubType(Repository<SubType>& repo) {
    cout << "\n=== CAP NHAT LOAI HINH THUE BAO ===\n";
    string id = InputHelper::getString("Nhap ma loai hinh can sua: ", false);
    SubType* p = repo.findById(id);
    if (p == nullptr) {
        cout << "[Loi] Khong tim thay loai hinh co ma: " << id << endl;
        return;
    }

    cout << "\nThong tin hien tai:\n";
    inTieuDeBangSubType();
    p->display();
    cout << string(95, '-') << endl;

    cout << "(Luu y: Nhan Enter de giu nguyen gia tri cu)\n";
    string tenMoi = InputHelper::getStringOrDefault("Ten loai hinh moi: ", p->getTenLoai());
    string moTaMoi = InputHelper::getStringOrDefault("Mo ta moi: ", p->getMoTa());
    double cuocMoi = InputHelper::getDoubleOrDefault("Cuoc thang moi: ", p->getCuocThang());

    SubType updated(id, tenMoi, moTaMoi, cuocMoi);
    repo.update(id, updated);
    cout << "Cap nhat loai hinh thue bao thanh cong!\n";
}

static void xoaSubType(Repository<SubType>& repo) {
    cout << "\n=== XOA LOAI HINH THUE BAO ===\n";
    string id = InputHelper::getString("Nhap ma loai hinh can xoa: ", false);
    SubType* p = repo.findById(id);
    if (p == nullptr) {
        cout << "[Loi] Khong tim thay loai hinh co ma: " << id << endl;
        return;
    }

    cout << "\nThong tin can xoa:\n";
    inTieuDeBangSubType();
    p->display();
    cout << string(95, '-') << endl;

    if (InputHelper::getConfirm("Ban co chac chan muon xoa loai hinh nay (y/n)? ")) {
        repo.remove(id);
        cout << "Xoa loai hinh thanh cong!\n";
    } else {
        cout << "Da huy thao tac xoa.\n";
    }
}

// ============================================================================
// MENU QUAN LY LOAI HINH THUE BAO
// ============================================================================
void menuLoaiHinh() {
    Repository<SubType> repo("data/sub_types.txt");
    repo.loadFromFile();

    int chon = -1;
    do {
        cout << "\n========================================\n";
        cout << "     QUAN LY LOAI HINH THUE BAO         \n";
        cout << "========================================\n";
        cout << "1. Xem danh sach loai hinh\n";
        cout << "2. Them moi loai hinh\n";
        cout << "3. Tim kiem loai hinh theo ma\n";
        cout << "4. Cap nhat loai hinh\n";
        cout << "5. Xoa loai hinh\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (chon) {
            case 1: xemDanhSachSubType(repo); break;
            case 2: themMoiSubType(repo);     break;
            case 3: timKiemSubType(repo);     break;
            case 4: suaSubType(repo);         break;
            case 5: xoaSubType(repo);         break;
            case 0: break;
        }
    } while (chon != 0);
}
