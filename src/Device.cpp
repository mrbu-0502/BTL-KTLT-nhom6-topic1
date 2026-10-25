// Kiều Đức Hiệp
// menu thiết bị

#include "Device.h"
#include "Sim.h"
#include "Date.h"
#include "Repository.h"
#include "InputHelper.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// CÁC HÀM KIỂM TRA DỮ LIỆU DÙNG CHUNG
// ============================================================================

static bool laChuCai(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool laChuSo(char c) {
    return c >= '0' && c <= '9';
}

// Chuyển toàn bộ chuỗi về chữ thường (dùng để so sánh không phân biệt hoa thường);
static string chuyenThuong(const string& s) {
    string ketQua = s;
    for (int i = 0; i < (int)ketQua.size(); i++) {
        if (ketQua[i] >= 'A' && ketQua[i] <= 'Z') {
            ketQua[i] = ketQua[i] + 32;
        }
    }
    return ketQua;
}

// Hàm bổ trợ: kiểm tra chuỗi số bằng thuật toán Luhn (dùng cho chữ số kiểm tra
// cuối của IMEI). Duyệt từ phải sang trái, nhân đôi các vị trí cách chữ số
// kiểm tra 1 bước; số hợp lệ khi tổng các chữ số chia hết cho 10.
static bool kiemTraLuhn(const string& so) {
    if (so.empty()) return false;

    int tong = 0;
    bool nhanDoi = false;
    for (int i = (int)so.size() - 1; i >= 0; i--) {
        if (!laChuSo(so[i])) return false;

        int chuSo = so[i] - '0';
        if (nhanDoi) {
            chuSo = chuSo * 2;
            if (chuSo > 9) chuSo = chuSo - 9;
        }
        tong = tong + chuSo;
        nhanDoi = !nhanDoi;
    }
    return tong % 10 == 0;
}

// IMEI hợp lệ: đúng 15 chữ số và vượt qua chữ số kiểm tra theo thuật toán Luhn
// (các chữ số cạnh nhau được phép giống nhau);
static bool hopLeIMEI(const string& s) {
    if (s.size() != 15) return false;

    for (int i = 0; i < (int)s.size(); i++) {
        if (!laChuSo(s[i])) return false;
    }

    return kiemTraLuhn(s);
}

// Kiểm tra chéo với dữ liệu: IMEI này đã được thiết bị khác dùng ở khe 1
// hoặc khe 2 chưa (bỏ qua giá trị NONE vì nhiều máy có thể cùng NONE);
static bool imeiDaTonTai(Repository<Device>& repo, const string& imei) {
    vector<Device> list = repo.getAll();
    int soLuong = (int)list.size();
    for (int i = 0; i < soLuong; i++) {
        if (list[i].getMaIMEI1() == imei) return true;
        if (imei != "NONE" && list[i].getMaIMEI2() == imei) return true;
    }
    return false;
}

// Tên máy: 2-40 ký tự, bắt đầu bằng chữ cái, chỉ gồm chữ cái, chữ số, khoảng
// trắng và các ký tự - + . / ( ). Không có 2 khoảng trắng liền nhau và không
// lặp 4 ký tự giống nhau liên tiếp (chặn dữ liệu gõ lặp vô nghĩa như "iphoneeee").
static bool hopLeTenMay(const string& s) {
    if (s.size() < 2 || s.size() > 40) return false;
    if (!laChuCai(s[0])) return false;

    int soLanLap = 1;
    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        bool kyTuHopLe = laChuCai(c) || laChuSo(c) || c == ' ' || c == '-' ||
                         c == '+' || c == '.' || c == '/' || c == '(' || c == ')';
        if (!kyTuHopLe) return false;
        if (i > 0 && c == ' ' && s[i - 1] == ' ') return false;

        if (i > 0 && c == s[i - 1]) {
            soLanLap++;
            if (soLanLap >= 4) return false;
        } else {
            soLanLap = 1;
        }
    }
    return true;
}

// Hãng sản xuất: 2-25 ký tự, bắt đầu bằng chữ cái, chỉ gồm chữ cái, khoảng
// trắng, '-' và '.'. Loại dữ liệu rác như "233223" hay "appleeee".
static bool hopLeHangSX(const string& s) {
    if (s.size() < 2 || s.size() > 25) return false;
    if (!laChuCai(s[0])) return false;

    int soLanLap = 1;
    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        if (!(laChuCai(c) || c == ' ' || c == '-' || c == '.')) return false;
        if (i > 0 && c == ' ' && s[i - 1] == ' ') return false;

        if (i > 0 && c == s[i - 1]) {
            soLanLap++;
            if (soLanLap >= 4) return false;
        } else {
            soLanLap = 1;
        }
    }
    return true;
}

// Chuẩn hóa hãng sản xuất về dạng tên riêng: "SAMSUNG" -> "Samsung", "d-link" -> "D-Link";
static string chuanHoaHang(const string& s) {
    string ketQua = s;
    bool batDauTu = true;
    for (int i = 0; i < (int)ketQua.size(); i++) {
        char c = ketQua[i];
        if (c == ' ' || c == '-' || c == '.') {
            batDauTu = true;
            continue;
        }
        if (!laChuCai(c)) continue;

        if (batDauTu) {
            if (c >= 'a' && c <= 'z') ketQua[i] = c - 32;
        } else {
            if (c >= 'A' && c <= 'Z') ketQua[i] = c + 32;
        }
        batDauTu = false;
    }
    return ketQua;
}

// Tình trạng: chỉ nhận "Moi" hoặc "Cu", không phân biệt hoa thường;
static bool hopLeTinhTrang(const string& s) {
    string thuong = chuyenThuong(s);
    return thuong == "moi" || thuong == "cu";
}

static string chuanHoaTinhTrang(const string& s) {
    if (chuyenThuong(s) == "moi") return "Moi";
    return "Cu";
}

// ============================================================================
// CONSTRUCTOR
// ============================================================================
Device::Device()
    : Entity(""), maIMEI1(""), maIMEI2("NONE"), tenMay(""), hangSX(""), ngaySanXuat(""), tinhTrang("") {}

Device::Device(string maThietBi, string maIMEI1, string maIMEI2, string tenMay,
               string hangSX, string ngaySanXuat, string tinhTrang)
    : Entity(maThietBi), maIMEI1(maIMEI1), maIMEI2(maIMEI2), tenMay(tenMay),
      hangSX(hangSX), ngaySanXuat(ngaySanXuat), tinhTrang(tinhTrang) {}

// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
// Khong override getID(): ma thiet bi chinh la ID cua Entity, getID() cua lop
// co so da du tra ve dung gia tri nay.
void Device::getInformation() {
    setID(InputHelper::getString("Nhap ma thiet bi (vi du: DE11): ", false));
    setMaIMEI1(InputHelper::getString("Nhap ma IMEI 1 (15 chu so, dung chuan Luhn): ", false));
    setMaIMEI2(InputHelper::getString("Nhap ma IMEI 2 (15 chu so hoac NONE): ", false));
    setTenMay(InputHelper::getString("Nhap ten may: ", false));
    setHangSX(InputHelper::getString("Nhap hang san xuat: ", false));
    Date d;
    d.input(InputHelper::getString("Nhap ngay san xuat (dd/mm/yyyy): ", false));
    setNgaySanXuat(d.output());
    setTinhTrang(InputHelper::getString("Nhap tinh trang (Moi / Cu): ", false));
}

// Cac cot: Ma(6) - IMEI1(17) - IMEI2(17) - Ten may(30) - Hang(12) - Ngay SX(12) - Tinh trang(12) = 106
void Device::display() const {
    cout << left << setw(6) << ID
         << setw(17) << maIMEI1
         << setw(17) << maIMEI2
         << setw(30) << tenMay
         << setw(12) << hangSX
         << setw(12) << ngaySanXuat
         << setw(12) << tinhTrang << endl;
}

string Device::toString() const {
    stringstream ss;
    ss << ID << "|" << maIMEI1 << "|" << maIMEI2 << "|" << tenMay << "|"
       << hangSX << "|" << ngaySanXuat << "|" << tinhTrang;
    return ss.str();
}

void Device::fromString(const string& line) {
    stringstream ss(line);

    getline(ss, ID, '|');
    getline(ss, maIMEI1, '|');
    getline(ss, maIMEI2, '|');
    getline(ss, tenMay, '|');
    getline(ss, hangSX, '|');
    getline(ss, ngaySanXuat, '|');
    getline(ss, tinhTrang);
}

// ============================================================================
// GETTER & SETTER
// ============================================================================
string Device::getMaIMEI1() const {
    return maIMEI1;
}

string Device::getMaIMEI2() const {
    return maIMEI2;
}

string Device::getTenMay() const {
    return tenMay;
}

string Device::getHangSX() const {
    return hangSX;
}

string Device::getNgaySanXuat() const {
    return ngaySanXuat;
}

string Device::getTinhTrang() const {
    return tinhTrang;
}

void Device::setMaIMEI1(const string& imei) {
    maIMEI1 = imei;
}

void Device::setMaIMEI2(const string& imei) {
    maIMEI2 = imei;
}

void Device::setTenMay(const string& ten) {
    tenMay = ten;
}

void Device::setHangSX(const string& hang) {
    hangSX = hang;
}

void Device::setNgaySanXuat(const string& ngay) {
    ngaySanXuat = ngay;
}

void Device::setTinhTrang(const string& tt) {
    tinhTrang = tt;
}

// ============================================================================
// CAC HAM HO TRO GIAO DIEN CONSOLE (IN TIEU DE BANG)
// ============================================================================
static void inTieuDeBangThietBi() {
    cout << string(106, '-') << endl;
    cout << left << setw(6) << "Ma TB"
         << setw(17) << "IMEI 1"
         << setw(17) << "IMEI 2"
         << setw(30) << "Ten May"
         << setw(12) << "Hang SX"
         << setw(12) << "Ngay SX"
         << setw(12) << "Tinh Trang" << endl;
    cout << string(106, '-') << endl;
}

// ============================================================================
// THAO TAC CRUD (khong co cap nhat: thiet bi da tao thi khong sua duoc nua)
// ============================================================================

// hiển thị danh sách;
static void xemDanhSachThietBi(Repository<Device>& repo) {
    vector<Device> list = repo.getAll();
    int soLuong = (int)list.size();
    if (soLuong == 0) {
        cout << "Danh sach thiet bi hien dang trong!\n";
        return;
    }
    cout << "\n=== DANH SACH THIET BI (" << soLuong << ") ===\n";
    inTieuDeBangThietBi();
    for (int i = 0; i < soLuong; i++) {
        list[i].display();
    }
    cout << string(106, '-') << endl;
}

// thêm mới thiết bị (mã DE tự sinh, IMEI nhập tay và kiểm tra Luhn);
static void themMoiThietBi(Repository<Device>& repo) {
    cout << "\n=== THEM MOI THIET BI ===\n";

    // 1. IMEI khe 1: bắt buộc, đúng 15 chữ số, đúng chuẩn Luhn và chưa
    //    thiết bị nào dùng;
    string imei1;
    while (true) {
        imei1 = InputHelper::getString("Nhap ma IMEI 1 (15 chu so, nhap 0 de thoat): ", false);
        if (imei1 == "0") {
            cout << "Da huy thao tac them moi.\n";
            return;
        }
        if (!hopLeIMEI(imei1)) {
            cout << "[Loi] Ma IMEI phai gom dung 15 chu so va phai dung chu so kiem tra Luhn!\n";
            continue;
        }
        if (imeiDaTonTai(repo, imei1)) {
            cout << "[Loi] Ma IMEI " << imei1 << " da duoc dung cho thiet bi khac!\n";
            continue;
        }
        break;
    }

    // 2. IMEI khe 2: có thể là NONE (máy dùng 1 SIM). Nếu nhập số thì phải
    //    đúng chuẩn Luhn, khác IMEI 1 và chưa thiết bị nào dùng;
    string imei2;
    while (true) {
        imei2 = InputHelper::getString("Nhap ma IMEI 2 (15 chu so hoac NONE, nhap 0 de thoat): ", false);
        if (imei2 == "0") {
            cout << "Da huy thao tac them moi.\n";
            return;
        }
        if (chuyenThuong(imei2) == "none") {
            imei2 = "NONE";
            break;
        }
        if (!hopLeIMEI(imei2)) {
            cout << "[Loi] Ma IMEI 2 phai gom dung 15 chu so dung chuan Luhn, hoac nhap NONE!\n";
            continue;
        }
        if (imei2 == imei1) {
            cout << "[Loi] Ma IMEI 2 phai khac ma IMEI 1!\n";
            continue;
        }
        if (imeiDaTonTai(repo, imei2)) {
            cout << "[Loi] Ma IMEI " << imei2 << " da duoc dung cho thiet bi khac!\n";
            continue;
        }
        break;
    }

    // 3. Tên máy;
    string tenMay;
    while (true) {
        tenMay = InputHelper::getString("Nhap ten may (vi du: iPhone 15, Galaxy S24, nhap 0 de thoat): ", false);
        if (tenMay == "0") {
            cout << "Da huy thao tac them moi.\n";
            return;
        }
        if (!hopLeTenMay(tenMay)) {
            cout << "[Loi] Ten may phai bat dau bang chu cai, khong chua ky tu la, khong go lap ky tu vo nghia!\n";
            continue;
        }
        break;
    }

    // 4. Hãng sản xuất;
    string hangSX;
    while (true) {
        hangSX = InputHelper::getString("Nhap hang san xuat (vi du: Apple, Samsung, D-Link, nhap 0 de thoat): ", false);
        if (hangSX == "0") {
            cout << "Da huy thao tac them moi.\n";
            return;
        }
        if (!hopLeHangSX(hangSX)) {
            cout << "[Loi] Hang san xuat phai bat dau bang chu cai, khong chua chu so hay ky tu la!\n";
            continue;
        }
        hangSX = chuanHoaHang(hangSX);
        break;
    }

    // 5. Ngày sản xuất: nhập và kiểm tra bằng lớp Date chung (giống Complaint);
    Date ngaySanXuat;
    while (true) {
        try {
            string ngay = InputHelper::getString("Nhap ngay san xuat (dd/mm/yyyy, nhap 0 de thoat): ", false);
            if (ngay == "0") {
                cout << "Da huy thao tac them moi.\n";
                return;
            }
            ngaySanXuat.input(ngay);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << endl;
        }
    }

    // 6. Tình trạng;
    string tinhTrang;
    while (true) {
        tinhTrang = InputHelper::getString("Nhap tinh trang (Moi / Cu, nhap 0 de thoat): ", false);
        if (tinhTrang == "0") {
            cout << "Da huy thao tac them moi.\n";
            return;
        }
        if (!hopLeTinhTrang(tinhTrang)) {
            cout << "[Loi] Tinh trang chi duoc nhap Moi hoac Cu!\n";
            continue;
        }
        tinhTrang = chuanHoaTinhTrang(tinhTrang);
        break;
    }

    // 7. Mã thiết bị được sinh sau khi đã nhập xong dữ liệu, tránh tăng số
    //    thứ tự khi người dùng hủy giữa chừng;
    string maThietBi = InputHelper::generateAutoID(repo, "DE", "data_counter/device_counter.txt");
    cout << "Ma thiet bi: " << maThietBi << endl;

    Device thietbi(maThietBi, imei1, imei2, tenMay, hangSX, ngaySanXuat.output(), tinhTrang);
    repo.add(thietbi);
    cout << "Them thiet bi " << maThietBi << " (IMEI 1: " << imei1 << ") thanh cong!\n";
}

// tìm kiếm thiết bị theo mã DE;
static void timKiemThietBi(Repository<Device>& repo) {
    cout << "\n=== TIM KIEM THIET BI THEO MA ===\n";

    string id;
    Device* p;
    while (true) {
        id = InputHelper::getString("Nhap ma thiet bi can tim (vi du DE01, nhap 0 de thoat): ", false);
        if (id == "0") return;

        if (!InputHelper::checkidvalid(id, "DE")) {
            cout << "[Loi] Ma thiet bi khong hop le (dang DE + chu so, vi du DE01)!\n";
            continue;
        }
        p = repo.findById(id);
        if (p == nullptr) {
            cout << "[Thong bao] Khong tim thay thiet bi co ma: " << id << endl;
            continue;
        }

        break;
    }

    cout << "\nKet qua tim thay:\n";
    inTieuDeBangThietBi();
    p->display();
    cout << string(106, '-') << endl;
}

// xóa thiết bị (chặn nếu đang được SIM sử dụng);
static void xoaThietBi(Repository<Device>& repo, Repository<Sim>& repo_sim) {
    cout << "\n=== XOA THIET BI ===\n";
    string id;
    Device* p;
    while (true) {
        id = InputHelper::getString("Nhap ma thiet bi can xoa (vi du DE01, nhap 0 de thoat): ", false);
        if (id == "0") return;

        if (!InputHelper::checkidvalid(id, "DE")) {
            cout << "[Loi] Ma thiet bi khong hop le (dang DE + chu so, vi du DE01)!\n";
            continue;
        }
        p = repo.findById(id);
        if (p == nullptr) {
            cout << "[Loi] Khong tim thay thiet bi co ma: " << id << endl;
            continue;
        }

        break;
    }

    // Kiểm tra chéo: IMEI (khe 1 hoặc khe 2) đang được SIM trong sims.txt tham
    // chiếu thì không cho xóa, tránh dữ liệu mồ côi;
    string imei1 = p->getMaIMEI1();
    string imei2 = p->getMaIMEI2();
    vector<Sim> listSim = repo_sim.getAll();
    int soSim = (int)listSim.size();
    bool dangDuocSuDung = false;
    for (int i = 0; i < soSim; i++) {
        string imeiCuaSim = listSim[i].getMaIMEI();
        bool trungKhe1 = (imeiCuaSim == imei1);
        bool trungKhe2 = (imei2 != "NONE" && imeiCuaSim == imei2);
        if (trungKhe1 || trungKhe2) {
            cout << "[Loi] Thiet bi " << id << " khong the xoa vi SIM "
                 << listSim[i].getSoSim() << " dang su dung IMEI " << imeiCuaSim << "!\n";
            dangDuocSuDung = true;
        }
    }
    if (dangDuocSuDung) {
        return;
    }

    cout << "\nThong tin can xoa:\n";
    inTieuDeBangThietBi();
    p->display();
    cout << string(106, '-') << endl;

    if (InputHelper::getConfirm("Ban co chac chan muon xoa thiet bi nay (y/n)? ")) {
        repo.remove(id);
        cout << "Xoa thiet bi thanh cong!\n";
    } else {
        cout << "Da huy thao tac xoa.\n";
    }
}

// ============================================================================
// MENU QUAN LY THIET BI
// ============================================================================
void menuThietBi() {
    Repository<Device> repo("data/devices.txt");
    Repository<Sim> repo_sim("data/sims.txt");
    try {
        repo.loadFromFile();
        repo_sim.loadFromFile();
    } catch (const exception& e) {
        cout << "[Loi] Khong nap duoc du lieu thiet bi: " << e.what() << endl;
        return;
    }

    int chon = -1;
    do {
        cout << "\n========================================\n";
        cout << "        QUAN LY THIET BI (IMEI)         \n";
        cout << "========================================\n";
        cout << "1. Xem danh sach thiet bi\n";
        cout << "2. Them thiet bi moi\n";
        cout << "3. Tim kiem thiet bi theo ma\n";
        cout << "4. Xoa thiet bi\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-4): ", 0, 4);

        switch (chon) {
            case 1: xemDanhSachThietBi(repo);        break;
            case 2: themMoiThietBi(repo);            break;
            case 3: timKiemThietBi(repo);            break;
            case 4: xoaThietBi(repo, repo_sim);      break;
            case 0: break;
        }
    } while (chon != 0);
}
