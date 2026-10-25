// Nguyễn Văn Cường - B24DCVT053
// menu khiếu nại

#include "Contract.h"
#include "Date.h"
#include "InputHelper.h"
#include "Exceptions.h"
#include "Repository.h"
#include "Complaint.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include<string>


using namespace std;

// ============================================================================
// CONSTRUCTOR
// ============================================================================
Complaint::Complaint()
    : Entity(""), maHD(""), ngayGui(""), noiDung(""), trangThai("") {}

Complaint::Complaint(string maKN, string maHD, string ngayGui, string noiDung, string trangThai)
    : Entity(maKN), maHD(maHD), ngayGui(ngayGui), noiDung(noiDung), trangThai(trangThai) {}

// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
// Khong override getID(): ma khieu nai chinh la ID cua Entity, getID() cua
// lop co so da du tra ve dung gia tri nay.
void Complaint::getInformation() {
    setID(InputHelper::getString("Nhap ma khieu nai (vi du: KN11): ", false));
    setMaHD(InputHelper::getString("Nhap ma hop dong (vi du: HD01): ", false));
    Date d;
    d.input(InputHelper::getString("Nhap ngay gui (dd/mm/yyyy): ", false));
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
    cout << string(93, '-') << endl;
    cout << left << setw(8)  << "Ma KN"
         << setw(10) << "Ma HD"
         << setw(13) << "Ngay Gui"
         << setw(50) << "Noi Dung"
         << setw(12) << "Trang Thai" << endl;
    cout << string(93, '-') << endl;
}

// ============================================================================
// THAO TAC CRUD
// ============================================================================

//hien thi danh sach;
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
    cout << string(93, '-') << endl;
}

//them moi khieu nai;
static void themMoiComplaint(Repository<Complaint>& repo_complaint, Repository<Contract>& repo_contract ) {
    cout << "\n=== THEM MOI PHIEU KHIEU NAI ===\n";

    string maHD;
    Contract* hopdong = nullptr;
    while (true) {
        maHD = InputHelper::getString("Nhap ma hop dong (vi du: HD01, nhap 0 de thoat): ", false);
        if (maHD == "0") {
            return;
        }
        if (!InputHelper::checkidvalid(maHD, "HD")) {
            cout << "[Loi] Ma hop dong phai co dang HD + chu so (vi du HD01)! Vui long nhap lai.\n";
            continue;
        }
        //giu lai con tro hop dong de dung ben duoi, khong goi findById nhieu lan;
        hopdong = repo_contract.findById(maHD);
        if (hopdong == nullptr) {
            cout << "[Canh bao] Ma hop dong " << maHD << " chua ton tai trong he thong!\n";
            continue;
        }
        break;
    }

    Date ngaygui, ngayhopdong;
    //ngay tao hop dong doc tu file co the sai dinh dang, phai bat ngoai le ngay tai day;
    try {
        ngayhopdong.input(hopdong->getRDate());
    } catch (const exception& error) {
        cout << "[Loi] Ngay tao hop dong " << maHD << " trong du lieu khong hop le: " << error.what() << endl;
        return;
    }
    while(true){
        try{
            string ngay = InputHelper::getString("nhap ngay (DD/MM/YYYY, nhap 0 de thoat):", false);
            if (ngay == "0") {
                cout << "Da huy thao tac them moi.\n";
                return;
            }
        
            ngaygui.input(ngay);
            if(ngaygui.compare(ngayhopdong) >= 0) break;
            else {
                cout << "Ngay gui khieu nai khong the som hon ngay tao hop dong!" << endl;
            }
        }
        catch(const exception& error){
            cout << "Loi: " << error.what() << endl;
        }
    }

    string noiDung = InputHelper::getString("Nhap noi dung khieu nai (nhap 0 de thoat): ", false);
    if (noiDung == "0") {
        cout << "Da huy thao tac them moi.\n";
        return;
    }

    //phieu khieu nai moi luon o trang thai TiepNhan, khong cho nguoi dung tu nhap;
    string trangThai = "TiepNhan";

    //ma KN duoc sinh sau khi da nhap xong du lieu, tranh tang so thu tu khi nguoi dung huy;
    string maKN = InputHelper::generateAutoID(repo_complaint, "KN", "data_counter/complaint_counter.txt");
    Complaint kn(maKN, maHD, ngaygui.output(), noiDung, trangThai);
    repo_complaint.add(kn);
    cout << "Them phieu khieu nai id: " << maKN << " thanh cong!\n";
}


//tim kiem ma khieu nai;
static void timKiemComplaint(Repository<Complaint>& repo_complaint) {
    cout << "\n=== TIM KIEM KHIEU NAI THEO MA ===\n";
    string id;
    Complaint* p;
    while (true) {
        id = InputHelper::getString("Nhap ma khieu nai can tim (nhap 0 de thoat): ", false);
        if (id == "0") return;

        if (!InputHelper::checkidvalid(id, "KN")) {
            cout << "[Loi] Ma khieu nai khong hop le (dang KN + chu so, vi du KN01)!\n";
            continue;
        }
        p = repo_complaint.findById(id);
        if (p == nullptr) {
            cout << "[Thong bao] Khong tim thay khieu nai co ma: " << id << endl;
            continue;
        }

        break;
    }
    
    cout << "\nKet qua tim thay:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(93, '-') << endl;
}


//sua ma khieu nai;
static void suaComplaint(Repository<Complaint>& repo_complaint, Repository<Contract>& repo_contract) {
    cout << "\n=== CAP NHAT PHIEU KHIEU NAI ===\n";
    Complaint* p;
    string id;
    while(true){
        id = InputHelper::getString("Nhap ma khieu nai can sua (nhap 0 de thoat): ", false);
        if(id == "0") return;

        if (!InputHelper::checkidvalid(id, "KN")) {
            cout << "[Loi] Ma khieu nai khong hop le (dang KN + chu so, vi du KN01)!\n";
            continue;
        }
        p = repo_complaint.findById(id);
        if (p == nullptr) {
            cout << "[Loi] Khong tim thay khieu nai co ma: " << id << endl;
            continue;
        }

        break;
    }
    
    //kiem tra xem phieu khieu nai da dong chua;
    if (p->getTrangThai() == "Dong") {
        cout << "[Loi] Khieu nai da dong, khong the cap nhat!\n";
        return; 
    }

    cout << "\nThong tin hien tai:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(93, '-') << endl;

    cout << "(Luu y: Nhan Enter de giu nguyen ma HD, ngay gui, noi dung, trang thai)\n";
    string maHDMoi;
    Contract* hopdongmoi = nullptr;
    while (true) {
        maHDMoi = InputHelper::getStringOrDefault("Ma HD moi(de trong se giu nguyen): ", p->getMaHD());
        if (!InputHelper::checkidvalid(maHDMoi, "HD")) {
            cout << "[Loi] Ma hop dong phai co dang HD + chu so (vi du HD01)! Vui long nhap lai.\n";
            continue;
        }
        //kiem tra ton tai ca khi nguoi dung giu nguyen ma HD cu, vi hop dong do co the da bi xoa;
        hopdongmoi = repo_contract.findById(maHDMoi);
        if (hopdongmoi != nullptr) {
            break; // Ma hop dong hop le
        }
        cout << "[Canh bao] Ma hop dong '" << maHDMoi << "' chua ton tai trong he thong! Vui long nhap lai.\n";
    }

    Date ngayguimoi, ngayHDmoi;
    //ngay tao hop dong doc tu file co the sai dinh dang, phai bat ngoai le ngay tai day;
    try {
        ngayHDmoi.input(hopdongmoi->getRDate());
    } catch (const exception& error) {
        cout << "[Loi] Ngay tao hop dong " << maHDMoi << " trong du lieu khong hop le: " << error.what() << endl;
        return;
    }
    while(true) {
        try{
            //de trong (Enter) se giu nguyen ngay gui cu;
            string text = InputHelper::getStringOrDefault("Nhap ngay gui moi (DD/MM/YYYY, nhap 0 de huy)", p->getNgayGui());
            if (text == "0") {
                cout << "Da huy thao tac cap nhat.\n";
                return;
            }
            ngayguimoi.input(text);
            if(ngayguimoi.compare(ngayHDmoi) >= 0) break;
            else {
                cout << "Ngay gui khieu nai khong the som hon ngay tao hop dong!" << endl;
            }
        }
        catch(const exception& error){
            cout << "Loi: " << error.what() << endl;
        }
    }
    
    string noiDungMoi = InputHelper::getStringOrDefault("Noi dung moi: ", p->getNoiDung());
    
    string trangThaimoi;
    while(true){
        trangThaimoi = InputHelper::getStringOrDefault("Nhap trang thai moi (TiepNhan / DangXuLy / DaXuLy) (enter de giu nguyen, 0 de huy): ", p->getTrangThai());
        if (trangThaimoi == "0") {
            cout << "Da huy thao tac cap nhat.\n";
            return;
        }
        if(trangThaimoi == "TiepNhan" || trangThaimoi == "DangXuLy" || trangThaimoi == "DaXuLy") break;
        else cout << "Sai trang thai, trang thai chi co the nhap (TiepNhan / DangXuLy / DaXuLy)!" << endl;
    }
    Complaint updated(id, maHDMoi, ngayguimoi.output(), noiDungMoi, trangThaimoi);
    repo_complaint.update(id, updated);
    cout << "Cap nhat phieu khieu nai id: " << id << " thanh cong!\n";
}

//dong ma khieu nai
static void dongComplaint(Repository<Complaint>& repo_complaint) {
    cout << "\n=== DONG PHIEU KHIEU NAI ===\n";
    string id;
    Complaint* p;
    while(true){
        id = InputHelper::getString("Nhap ma khieu nai can dong (nhap 0 de thoat): ", false);
        if (id == "0") return;

        if (!InputHelper::checkidvalid(id, "KN")) {
            cout << "[Loi] Ma khieu nai khong hop le (dang KN + chu so, vi du KN01)!\n";
            continue;
        }

        p = repo_complaint.findById(id);

        if (p == nullptr) {
            cout << "[Thong bao] Khong tim thay khieu nai co ma: " << id << endl;
            continue;
        }

        if(p->getTrangThai() == "Dong"){
            cout << "[Loi] Khieu nai da dong, khong the dong lai!\n";
            return;
        }
            
        break;
    }
   

    cout << "\nThong tin:\n";
    inTieuDeBangComplaint();
    p->display();
    cout << string(93, '-') << endl;

    if (InputHelper::getConfirm("Ban co chac chan muon dong phieu khieu nai nay khong (y/n)? ")) {
        //sua tren ban sao roi moi ghi file: findById tra con tro vao vector cua Repository,
        //neu ghi file that bai thi du lieu trong bo nho van giu trang thai cu;
        Complaint tam = *p;
        tam.setTrangThai("Dong");
        repo_complaint.update(id, tam);
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
    try {
        repo_complaint.loadFromFile();
        repo_contract.loadFromFile();
    } catch (const exception& e) {
        cout << "[Loi] Khong nap duoc du lieu khieu nai: " << e.what() << endl;
        return;
    }
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
