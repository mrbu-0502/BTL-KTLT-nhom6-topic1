#include "Complaint.h"
#include "Repository.h"
#include "InputHelper.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;


// CONSTRUCTOR
// ============================================================================
Complaint::Complaint()
    : Entity(""), maKH(""), soSim(""), ngayGui(""), noiDung(""), trangThai("DangXuLy") {}

Complaint::Complaint(string maKN, string maKH, string soSim, string ngayGui, string noiDung, string trangThai)
    : Entity(maKN), maKH(maKH), soSim(soSim), ngayGui(ngayGui), noiDung(noiDung), trangThai(trangThai) {}

// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
string Complaint::getID() const {
    return ID;
}

void Complaint::getInformation() {
    setID(InputHelper::getString("Nhap ma khieu nai (vi du: KN11): ", false));
    setMaKH(InputHelper::getString("Nhap ma khach hang (vi du: KH01): ", false));
    setSoSim(InputHelper::getString("Nhap so thue bao/SIM (vi du: 0981000001): ", false));
    setNgayGui(InputHelper::getString("Nhap ngay gui (dd/mm/yyyy): ", false));
    setNoiDung(InputHelper::getString("Nhap noi dung khieu nai: ", false));
    setTrangThai(InputHelper::getString("Nhap trang thai (TiepNhan / DangXuLy / DaXuLy): ", false));
}

void Complaint::display() const {
    cout << left << setw(8)  << ID
         << setw(10) << maKH
         << setw(14) << soSim
         << setw(13) << ngayGui
         << setw(50) << noiDung
         << setw(12) << trangThai << endl;
}

string Complaint::toString() const {
    stringstream ss;
    ss << ID << "|" << maKH << "|" << soSim << "|" << ngayGui << "|" << noiDung << "|" << trangThai;
    return ss.str();
}

void Complaint::fromString(const string& line) {
    stringstream ss(line);
    getline(ss, ID, '|');
    getline(ss, maKH, '|');
    getline(ss, soSim, '|');
    getline(ss, ngayGui, '|');
    getline(ss, noiDung, '|');
    getline(ss, trangThai, '|');
}

// ============================================================================
// GETTER & SETTER
// ============================================================================
string Complaint::getMaKH() const {
    return maKH;
}

string Complaint::getSoSim() const {
    return soSim;
}

string Complaint::getNgayGui() const {
    return ngayGui;
}

string Complaint::getNoiDung() const {
    return noiDung;
}

string Complaint::getTrangThai() const {
    return trangThai;
}

void Complaint::setMaKH(const string& mkh) {
    if (!mkh.empty()) {
        maKH = mkh;
    }
}

void Complaint::setSoSim(const string& sim) {
    if (!sim.empty()) {
        soSim = sim;
    }
}

void Complaint::setNgayGui(const string& ngay) {
    if (!ngay.empty()) {
        ngayGui = ngay;
    }
}

void Complaint::setNoiDung(const string& nd) {
    if (!nd.empty()) {
        noiDung = nd;
    }
}

void Complaint::setTrangThai(const string& tt) {
    if (!tt.empty()) {
        trangThai = tt;
    }
}

// ============================================================================
// CAC HAM HO TRO GIAO DIEN CONSOLE (IN TIEU DE BANG)
// ============================================================================
static void inTieuDeBangComplaint() {
    cout << string(110, '-') << endl;
    cout << left << setw(8)  << "Ma KN"
         << setw(10) << "Ma KH"
         << setw(14) << "So SIM"
         << setw(13) << "Ngay Gui"
         << setw(50) << "Noi Dung"
         << setw(15) << "Trang Thai" << endl;
    cout << string(110, '-') << endl;
}

// Hàm kiểm tra ràng buộc khóa ngoại (MaKH có tồn tại trong data/customers.txt không)
static bool kiemTraTonTaiMaKH(const string& maKH) {
    ifstream file("data/customers.txt");
    if (!file.is_open()) return true; // Nếu chưa có file khách hàng thì bỏ qua kiểm tra
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id;
        getline(ss, id, '|');
        if (id == maKH) return true;
    }
    return false;
}

// ============================================================================
// THAO TAC CRUD
// ============================================================================
static void xemDanhSachComplaint(Repository<Complaint>& repo) {
    vector<Complaint> list = repo.getAll();
    if (list.empty()) {
        cout << "Danh sach khieu nai hien dang trong!\n";
        return;
    }
    cout << "\n=== DANH SACH PHIEU KHIEU NAI (" << list.size() << ") ===\n";
    inTieuDeBangComplaint();
    for (size_t i = 0; i < list.size(); i++) {
        list[i].display();
    }
    cout << string(110, '-') << endl;
}

static void themMoiComplaint(Repository<Complaint>& repo) {
    cout << "\n=== THEM MOI PHIEU KHIEU NAI ===\n";

    string maKN = InputHelper::getString("Nhap ma khieu nai (vi du: KN11): ", false);
    if (repo.findById(maKN) != nullptr) {
        cout << "[Loi] Ma khieu nai '" << maKN << "' da ton tai!\n";
        return;
    }

    string maKH = InputHelper::getString("Nhap ma khach hang (vi du: KH01): ", false);
    if (!kiemTraTonTaiMaKH(maKH)) {
        cout << "[Canh bao] Ma khach hang '" << maKH << "' chua ton tai trong he thong!\n";
        if (!InputHelper::getConfirm("Ban co van muon tiep tuc luu phieu khieu nai nay khong (y/n)? ")) {
            cout << "Da huy thao tac them.\n";
            return;
        }
    }

    string soSim = InputHelper::getString("Nhap so SIM lien quan: ", false);
    string ngayGui = InputHelper::getString("Nhap ngay gui (dd/mm/yyyy): ", false);
    string noiDung = InputHelper::getString("Nhap noi dung khieu nai: ", false);
    string trangThai = InputHelper::getString("Nhap trang thai (TiepNhan / DangXuLy / DaXuLy): ", false);

    Complaint kn(maKN, maKH, soSim, ngayGui, noiDung, trangThai);
    repo.add(kn);
    cout << "Them phieu khieu nai thanh cong!\n";
}

static void timKiemComplaint(Repository<Complaint>& repo) {
    cout << "\n=== TIM KIEM KHIEU NAI THEO MA ===\n";
    string id = InputHelper::getString("Nhap ma khieu nai can tim: ", false);
    Complaint* p = repo.findById(id);
    if (p == nullptr) {
        cout << "[Thong bao] Khong tim thay khieu nai co ma: " << id << endl;
        return;
    }
    cout << "\nKet qua tim thay:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(98, '-') << endl;
}

static void suaComplaint(Repository<Complaint>& repo) {
    cout << "\n=== CAP NHAT PHIEU KHIEU NAI ===\n";
    string id = InputHelper::getString("Nhap ma khieu nai can sua: ", false);
    Complaint* p = repo.findById(id);
    if (p == nullptr) {
        cout << "[Loi] Khong tim thay khieu nai co ma: " << id << endl;
        return;
    }

    cout << "\nThong tin hien tai:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(98, '-') << endl;

    cout << "(Luu y: Nhan Enter de giu nguyen gia tri cu)\n";
    string maKHMoi = InputHelper::getStringOrDefault("Ma KH moi: ", p->getMaKH());
    string soSimMoi = InputHelper::getStringOrDefault("So SIM moi: ", p->getSoSim());
    string ngayGuiMoi = InputHelper::getStringOrDefault("Ngay gui moi: ", p->getNgayGui());
    string noiDungMoi = InputHelper::getStringOrDefault("Noi dung moi: ", p->getNoiDung());
    string trangThaiMoi = InputHelper::getStringOrDefault("Trang thai moi: ", p->getTrangThai());

    Complaint updated(id, maKHMoi, soSimMoi, ngayGuiMoi, noiDungMoi, trangThaiMoi);
    repo.update(id, updated);
    cout << "Cap nhat phieu khieu nai thanh cong!\n";
}

static void xoaComplaint(Repository<Complaint>& repo) {
    cout << "\n=== XOA PHIEU KHIEU NAI ===\n";
    string id = InputHelper::getString("Nhap ma khieu nai can xoa: ", false);
    Complaint* p = repo.findById(id);
    if (p == nullptr) {
        cout << "[Loi] Khong tim thay khieu nai co ma: " << id << endl;
        return;
    }

    cout << "\nThong tin can xoa:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(98, '-') << endl;

    if (InputHelper::getConfirm("Ban co chac chan muon xoa phieu khieu nai nay (y/n)? ")) {
        repo.remove(id);
        cout << "Xoa phieu khieu nai thanh cong!\n";
    } else {
        cout << "Da huy thao tac xoa.\n";
    }
}

// ============================================================================
// MENU QUAN LY PHIEU KHIEU NAI
// ============================================================================
void menuKhieuNai() {
    Repository<Complaint> repo("data/complaints.txt");
    repo.loadFromFile();

    int chon = -1;
    do {
        cout << "\n========================================\n";
        cout << "       QUAN LY PHIEU KHIEU NAI          \n";
        cout << "========================================\n";
        cout << "1. Xem danh sach khieu nai\n";
        cout << "2. Them moi phieu khieu nai\n";
        cout << "3. Tim kiem khieu nai theo ma\n";
        cout << "4. Cap nhat khieu nai\n";
        cout << "5. Xoa phieu khieu nai\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (chon) {
            case 1: xemDanhSachComplaint(repo); break;
            case 2: themMoiComplaint(repo);     break;
            case 3: timKiemComplaint(repo);     break;
            case 4: suaComplaint(repo);         break;
            case 5: xoaComplaint(repo);         break;
            case 0: break;
        }
    } while (chon != 0);
}
