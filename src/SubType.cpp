// ===============================================================
// Hoc vien Cong nghe Buu chinh Vien thong
// Hoc phan: Ky thuat lap trinh
// De tai 1: Quan ly thue bao di dong
// Tac gia: Nguyen Van Cuong
// Use Case: Loai hinh thue bao
// ===============================================================
#include "SubType.h"
#include "Repository.h"
#include "InputHelper.h"
#include "Sim.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// ============================================================================
// CONSTRUCTOR
// ============================================================================
SubType::SubType() : Entity(""), tenLoai(""), moTa(""), cuocThang(0) {}

SubType::SubType(string maLoai, string tenLoai, string moTa, long long cuocThang)
    : Entity(maLoai), tenLoai(tenLoai), moTa(moTa), cuocThang(cuocThang) {}

// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
// khong override vi da dung id cua entity lam ma loai hinh

void SubType::getInformation() {
    setID(InputHelper::getString("Nhap ma loai hinh (vi du: SUB01): ", false));
    setTenLoai(InputHelper::getString("Nhap ten loai hinh: ", false));
    setMoTa(InputHelper::getString("Nhap mo ta: ", false));
    setCuocThang(InputHelper::getLongLong("Nhap cuoc phi thang (VND): ", 0));
}

void SubType::display() const {
    cout << left << setw(10) << ID
         << setw(32) << tenLoai
         << setw(58) << moTa
         << right << setw(15) << cuocThang << endl;
}

string SubType::toString() const {
    stringstream ss;
    ss << ID << "|" << tenLoai << "|" << moTa << "|" << cuocThang;
    return ss.str();
}

void SubType::fromString(const string& line) {
    stringstream ss(line);
    string cuocStr;

    getline(ss, ID, '|');
    getline(ss, tenLoai, '|');
    getline(ss, moTa, '|');
    getline(ss, cuocStr);

    if (!cuocStr.empty()) {
        cuocThang = stoll(cuocStr);
    } else {
        cuocThang = 0;
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

long long SubType::getCuocThang() const {
    return cuocThang;
}

void SubType::setTenLoai(const string& ten) {
    tenLoai = ten;
}

void SubType::setMoTa(const string& mt) {
    moTa = mt;
}

void SubType::setCuocThang(long long cuoc) {
    if (cuoc >= 0) {
        cuocThang = cuoc;
    }
}

// ============================================================================
// CAC HAM HO TRO GIAO DIEN CONSOLE (IN TIEU DE BANG)
// ============================================================================
static void inTieuDeBangSubType() {
    cout << string(115, '-') << endl;
    cout << left << setw(10) << "Ma Loai"
         << setw(32) << "Ten Loai Hinh"
         << setw(58) << "Mo Ta"
         << right << setw(15) << "Cuoc Thang (d)" << endl;
    cout << string(115, '-') << endl;
}

// ============================================================================
// THAO TAC CRUD
// ============================================================================

//xem danh sach dang co;
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
    cout << string(115, '-') << endl;
}

//them moi subtype;
static void themMoiSubType(Repository<SubType>& repo) {
    cout << "\n=== THEM MOI LOAI HINH THUE BAO ===\n";

    string ten = InputHelper::getString("Nhap ten loai hinh (nhap 0 de thoat): ", false);
    if (ten == "0"){
        cout << "Da huy thao tac them moi.\n";
        return;
    }

    string moTa = InputHelper::getString("Nhap mo ta(nhap 0 de thoat): ", false);
    if (moTa == "0"){
        cout << "Da huy thao tac them moi.\n";
        return;
    }

    long long cuoc = InputHelper::getLongLong("Nhap cuoc phi thang (VND, nhap -1 de thoat): ", -1);
    if (cuoc == -1) {
        cout << "Da huy thao tac them moi.\n";
        return;
    }

    //ma loai hinh duoc sinh sau khi da nhap xong du lieu, tranh tang so thu tu khi nguoi dung huy;
    string id = InputHelper::generateAutoID(repo, "SUB", "data_counter/subtype_counter.txt");
    cout << "Ma loai hinh: " << id << endl;

    SubType subtypenew(id, ten, moTa, cuoc);
    repo.add(subtypenew);
    cout << "Them loai hinh thue bao thanh cong!\n";
}

//tim kiem subtype;
static void timKiemSubType(Repository<SubType>& repo) {
    cout << "\n=== TIM KIEM LOAI HINH THEO MA ===\n";

    string id;
    SubType* p;
    while(true){
        id = InputHelper::getString("Nhap ma loai hinh can tim (nhap 0 de thoat): ", false);
        if(id == "0") return;

        if (!InputHelper::checkidvalid(id, "SUB")) {
            cout << "[Loi] Ma loai hinh khong hop le (dang SUB + chu so, vi du SUB01)!\n";
            continue;
        }
        
        p = repo.findById(id);
        if (p == nullptr) {
            cout << "[Thong bao] Khong tim thay loai hinh co ma: " << id << endl;
            continue;
        }

        break;
    }
    
    cout << "\nKet qua tim thay:\n";
    inTieuDeBangSubType();
    p->display();
    cout << string(115, '-') << endl;
}

//sua subtype
static void suaSubType(Repository<SubType>& repo) {
    cout << "\n=== CAP NHAT LOAI HINH THUE BAO ===\n";
    string id;
    SubType* p;
    while(true){
        id = InputHelper::getString("Nhap ma loai hinh can sua (nhap 0 de thoat): ", false);
        if (id == "0") return;

        if (!InputHelper::checkidvalid(id, "SUB")) {
            cout << "[Loi] Ma loai hinh khong hop le (dang SUB + chu so, vi du SUB01)!\n";
            continue;
        }
        p = repo.findById(id);
        if (p == nullptr) {
            cout << "[Loi] Khong tim thay loai hinh co ma: " << id << endl;
            continue;
        }

        break;
    }

    cout << "\nThong tin hien tai:\n";
    inTieuDeBangSubType();
    p->display();
    cout << string(115, '-') << endl;

    cout << "(Luu y: Nhan Enter de giu nguyen gia tri cu)\n";
    string tenMoi = InputHelper::getStringOrDefault("Ten loai hinh moi: ", p->getTenLoai());
    string moTaMoi = InputHelper::getStringOrDefault("Mo ta moi: ", p->getMoTa());
    long long cuocMoi = InputHelper::getLongLongOrDefault("Cuoc thang moi: ", p->getCuocThang());

    SubType updated(id, tenMoi, moTaMoi, cuocMoi);
    repo.update(id, updated);
    cout << "Cap nhat loai hinh thue bao " << id << " thanh cong!\n";
}


//xoa subtype
static void xoaSubType(Repository<SubType>& repo, Repository<Sim>& repo_sim) {
    cout << "\n=== XOA LOAI HINH THUE BAO ===\n";
    string id;
    SubType* p;
    while(true){
        id = InputHelper::getString("Nhap ma loai hinh can xoa (nhap 0 de thoat): ", false);
        if (id == "0") return;
        
        if (!InputHelper::checkidvalid(id, "SUB")) {
            cout << "[Loi] Ma loai hinh khong hop le (dang SUB + chu so, vi du SUB01)!\n";
            continue;
        }
        p = repo.findById(id);
        if (p == nullptr) {
            cout << "[Loi] Khong tim thay loai hinh co ma: " << id << endl;
            continue;
        }
        
        break;
    }

    vector<Sim> listSim = repo_sim.getAll();
    for (size_t i = 0; i < listSim.size(); i++) {
        if (listSim[i].getMaLoai() == id) {
            cout << "[Loi] Loai hinh " << id << " khong the xoa vi van con sim dang dung!\n";
            return;
        }
    }
    
    cout << "\nThong tin can xoa:\n";
    inTieuDeBangSubType();
    p->display();
    cout << string(115, '-') << endl;

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
    Repository<Sim> repo_sim("data/sims.txt");
    try {
        repo.loadFromFile();
        repo_sim.loadFromFile();
    } catch (const exception& e) {
        cout << "[Loi] Khong nap duoc du lieu loai hinh thue bao: " << e.what() << endl;
        return;
    }


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
            case 5: xoaSubType(repo, repo_sim);         break;
            case 0: break;
        }
    } while (chon != 0);
}