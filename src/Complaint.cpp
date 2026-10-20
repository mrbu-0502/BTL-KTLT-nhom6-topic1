#include "Complaint.h"
#include "Date.h"
#include "Repository.h"
#include "InputHelper.h"
#include "Contract.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;


// CONSTRUCTOR
// ============================================================================
Complaint::Complaint()
    : Entity(""), maHD(""), ngayGui(""), noiDung(""), trangThai("") {}

Complaint::Complaint(string maKN, string maHD, string ngayGui, string noiDung, string trangThai)
    : Entity(maKN), maHD(maHD), ngayGui(ngayGui), noiDung(noiDung), trangThai(trangThai) {}

// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
string Complaint::getID() const {
    return ID;
}
    
void Complaint::getInformation() {
    setID(InputHelper::getString("Nhap ma khieu nai (vi du: KN11): ", false));
    setMaHD(InputHelper::getString("Nhap ma hop dong (vi du: HD01): ", false));
    Date d;
    d.input("Nhap ngay gui (dd/mm/yyyy): ");
    setNgayGui(d.output());
    setNoiDung(InputHelper::getString("Nhap noi dung khieu nai: ", false));
    setTrangThai(InputHelper::getString("Nhap trang thai (TiepNhan / DangXuLy / DaXuLy): ", false));
}

void Complaint::display() const {
    cout << left << setw(8)  << ID
         << setw(10) << maHD
         << setw(13) << ngayGui
         << setw(50) << noiDung
         << setw(12) << trangThai << endl;
}

string Complaint::toString() const {
    stringstream ss;
    ss << ID << "|" << maHD << "|" << ngayGui << "|" << noiDung << "|" << trangThai;
    return ss.str();
}

void Complaint::fromString(const string& line) {
    stringstream ss(line);
    getline(ss, ID, '|');
    getline(ss, maHD, '|');
    getline(ss, ngayGui, '|');
    getline(ss, noiDung, '|');
    getline(ss, trangThai);
}

// ============================================================================
// GETTER & SETTER
// ============================================================================
string Complaint::getMaHD() const{
    return maHD;
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

void Complaint::setMaHD(const string& mahd){
    maHD = mahd;
}

void Complaint::setNgayGui(const string& ngay){
    ngayGui = ngay;
}

void Complaint::setNoiDung(const string& nd){
    noiDung= nd;
}

void Complaint::setTrangThai(const string& tt){
    trangThai = tt;
}
// ============================================================================
// CAC HAM HO TRO GIAO DIEN CONSOLE (IN TIEU DE BANG)
// ============================================================================
static void inTieuDeBangComplaint() {
    cout << string(100, '-') << endl;
    cout << left << setw(8)  << "Ma KN"
         << setw(10) << "Ma HD"
         << setw(13) << "Ngay Gui"
         << setw(50) << "Noi Dung"
         << setw(15) << "Trang Thai" << endl;
    cout << string(100, '-') << endl;
}

//tu dong sinh ma KN;
//lay phan tu cuoi cung lam max roi ma KN moi = KN max + 1;
static string sinhmaKNmoi(Repository<Complaint>& repo_complaint){
    int max = 0;
    vector<Complaint> list = repo_complaint.getAll();
    if (list.empty()) {
        return "KN01";
    }

    string maKNmax = list[list.size() - 1].getID();
    max =stoi(maKNmax.substr(2));

    stringstream ss;
    ss << "KN" << setw(2) << setfill('0') << max + 1;
    
    return ss.str();
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

static void themMoiComplaint(Repository<Complaint>& repo_complaint, Repository<Contract>& repo_contract ) {
    cout << "\n=== THEM MOI PHIEU KHIEU NAI ===\n";

    string maKN = sinhmaKNmoi(repo_complaint);

    string maHD;
    while (true) {
        maHD = InputHelper::getString("Nhap ma hop dong (vi du: HD01, nhap 0 de thoat): ", false);
        if (maHD == "0") {
            return;
        }
        if (!InputHelper::checkidvalid(maHD, "HD")) {
            cout << "[Loi] Ma hop dong phai bat dau bang 'HD'! Vui long nhap lai.\n";
            continue;
        }
        if (repo_contract.findById(maHD) == nullptr) {
            cout << "[Canh bao] Ma hop dong " << maHD << " chua ton tai trong he thong!\n";
            continue;
        }
        break;
    }

    Date ngaygui, ngayhopdong;
    ngayhopdong.input_string(repo_contract.findById(maHD)->getRDate());
    while(true){
        ngaygui.input("nhap ngay gui (DD/MM/YYYY): ");
        if(ngaygui.compare(ngayhopdong) >= 0) break;
        else {
            cout << "Ngay gui khieu nai khong the som hon ngay tao hop dong!";
        }
    }

    string noiDung = InputHelper::getString("Nhap noi dung khieu nai: ", false);

    string trangThai;
    while(true){
        trangThai = InputHelper::getString("Nhap trang thai (TiepNhan / DangXuLy / DaXuLy): ", false);
        if(trangThai == "TiepNhan" || trangThai == "DangXuLy" || trangThai == "DaXuLy") break;
        else cout << "Sai trang thai, trang thai chi co the nhap (TiepNhan / DangXuLy / DaXuLy)!" << endl;
    }

    Complaint kn(maKN, maHD, ngaygui.output(), noiDung, trangThai);
    repo_complaint.add(kn);
    cout << "Them phieu khieu nai thanh cong!\n";
}

static void timKiemComplaint(Repository<Complaint>& repo_complaint) {
    cout << "\n=== TIM KIEM KHIEU NAI THEO MA ===\n";
    string id = InputHelper::getString("Nhap ma khieu nai can tim (nhap 0 de thoat): ", false);
    if (id == "0") return;

    if (!InputHelper::checkidvalid(id, "KN")) {
        cout << "[Loi] Ma khieu nai khong hop le (phai bat dau bang 'KN')!\n";
        return;
    }

    Complaint* p = repo_complaint.findById(id);
    if (p == nullptr) {
        cout << "[Thong bao] Khong tim thay khieu nai co ma: " << id << endl;
        return;
    }
    cout << "\nKet qua tim thay:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(98, '-') << endl;
}

static void suaComplaint(Repository<Complaint>& repo_complaint, Repository<Contract>& repo_contract) {
    cout << "\n=== CAP NHAT PHIEU KHIEU NAI ===\n";
    string id = InputHelper::getString("Nhap ma khieu nai can sua (nhap 0 de thoat): ", false);
    if(id == "0") return;

    if (!InputHelper::checkidvalid(id, "KN")) {
        cout << "[Loi] Ma khieu nai khong hop le (phai bat dau bang 'KN')!\n";
        return;
    }

    Complaint* p = repo_complaint.findById(id);
    if (p == nullptr) {
        cout << "[Loi] Khong tim thay khieu nai co ma: " << id << endl;
        return;
    }

    cout << "\nThong tin hien tai:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(98, '-') << endl;

    cout << "(Luu y: Nhan Enter de giu nguyen gia tri cu)\n";
    string maHDMoi;
    while (true) {
        maHDMoi = InputHelper::getStringOrDefault("Ma HD moi(de trong se giu nguyen): ", p->getMaHD());
        if (maHDMoi == p->getMaHD()) {
            break; // Giu nguyen ma hop dong cu
        }
        if (!InputHelper::checkidvalid(maHDMoi, "HD")) {
            cout << "[Loi] Ma hop dong phai bat dau bang 'HD'! Vui long nhap lai.\n";
            continue;
        }
        if (repo_contract.findById(maHDMoi) != nullptr) {
            break; // Ma hop dong moi hop le
        }
        cout << "[Canh bao] Ma hop dong '" << maHDMoi << "' chua ton tai trong he thong! Vui long nhap lai.\n";
    }

    Date ngayguimoi, ngayHDmoi;
    ngayHDmoi.input(repo_contract.findById(maHDMoi)->getRDate());
    while(true) {
        ngayguimoi.input("nhap ngay gui moi (DD/MM/YYYY): ");
        if(ngayguimoi.compare(ngayHDmoi) >= 0) break;
        else cout << "ngay nhap khieu nai khong the som hon ngay tao hop dong!";
    }
    
    string noiDungMoi = InputHelper::getStringOrDefault("Noi dung moi: ", p->getNoiDung());
    
    string trangThaimoi;
    while(true){
        trangThaimoi = InputHelper::getString("Nhap trang thai moi (TiepNhan / DangXuLy / DaXuLy): ", false);
        if(trangThaimoi == "TiepNhan" || trangThaimoi == "DangXuLy" || trangThaimoi == "DaXuLy") break;
        else cout << "Sai trang thai, trang thai chi co the nhap (TiepNhan / DangXuLy / DaXuLy)!" << endl;
    }
    Complaint updated(id, maHDMoi, ngayguimoi.output(), noiDungMoi, trangThaimoi);
    repo_complaint.update(id, updated);
    cout << "Cap nhat phieu khieu nai thanh cong!\n";
}

static void dongComplaint(Repository<Complaint>& repo_complaint) {
    cout << "\n=== DONG PHIEU KHIEU NAI ===\n";
    string id = InputHelper::getString("Nhap ma khieu nai can dong (nhap 0 de thoat): ", false);
    if(id == "0") return;
    
    if (!InputHelper::checkidvalid(id, "KN")) {
        cout << "[Loi] Ma khieu nai khong hop le (phai bat dau bang 'KN')!\n";
        return;
    }

    Complaint* p = repo_complaint.findById(id);
    if (p == nullptr) {
        cout << "[Loi] Khong tim thay khieu nai co ma: " << id << endl;
        return;
    }

    cout << "\nThong tin:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(98, '-') << endl;

    if (InputHelper::getConfirm("Ban co chac chan muon dong phieu khieu nai nay khong (y/n)? ")) {
        p->setTrangThai("Dong");
        repo_complaint.update(id, *p);
        cout << "dong phieu khieu nai thanh cong!\n";
    } else {
        cout << "Da huy thao tac dong phieu khieu nai.\n";
    }
}

// ============================================================================
// MENU QUAN LY PHIEU KHIEU NAI
// ============================================================================
void menuKhieuNai() {
    Repository<Complaint> repo_complaint("data/complaints.txt");
    Repository<Contract> repo_contract("data/contracts.txt"); 
    repo_complaint.loadFromFile();
    repo_contract.loadFromFile();
    int chon = -1;
    do {
        cout << "\n========================================\n";
        cout << "       QUAN LY PHIEU KHIEU NAI          \n";
        cout << "========================================\n";
        cout << "1. Xem danh sach khieu nai\n";
        cout << "2. Them moi phieu khieu nai\n";
        cout << "3. Tim kiem khieu nai theo ma\n";
        cout << "4. Cap nhat khieu nai\n";
        cout << "5. Dong phieu khieu nai\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (chon) {
            case 1: xemDanhSachComplaint(repo_complaint); break;
            case 2: themMoiComplaint(repo_complaint, repo_contract); break;
            case 3: timKiemComplaint(repo_complaint); break;
            case 4: suaComplaint(repo_complaint, repo_contract); break;
            case 5: dongComplaint(repo_complaint); break;
            case 0: break;
        }
    } while (chon != 0);
}
