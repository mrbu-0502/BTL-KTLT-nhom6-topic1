// ===============================================================
// Hoc vien Cong nghe Buu chinh Vien thong
// Hoc phan: Ky thuat lap trinh
// De tai 1: Quan ly thue bao di dong
// Tac gia: Kieu Duc Hiep
// Use Case: Quan ly thiet bi
// ===============================================================
#include "Device.h"
#include "Sim.h"
#include "Date.h"
#include "Repository.h"
#include "InputHelper.h"
#include "Exceptions.h"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// CAC HAM BO TRO & KIEM TRA DU LIEU DUNG CHUNG
// ============================================================================
static Date today() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    return Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

static bool laChuCai(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool laChuSo(char c) {
    return c >= '0' && c <= '9';
}
static string chuyenThuong(const string& s) {
    string ketQua = s;
    transform(ketQua.begin(), ketQua.end(), ketQua.begin(),
              [](unsigned char c) { return tolower(c); });
    return ketQua;
}
// Kiem tra chuoi so bang thuat toan Luhn (dung cho chu so kiem tra cuoi cua IMEI)
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
// IMEI hop le: dung 15 chu so va vuot qua chu so kiem tra Luhn
static bool hopLeIMEI(const string& s) {
    if (s.size() != 15) return false;
    for (char c : s) {
        if (!laChuSo(c)) return false;
    }
    return kiemTraLuhn(s);
}
// Kiem tra cheo: IMEI nay da duoc thiet bi khac dung o khe 1 hoac khe 2 chua (bo qua maTBBoQua khi cap nhat)
static bool imeiDaTonTai(Repository<Device>& repo, const string& imei, const string& maTBBoQua = "") {
    if (imei == "NONE") return false;
    for (const auto& item : repo.getAll()) {
        if (item.getID() == maTBBoQua) continue;
        if (item.getMaIMEI1() == imei || item.getMaIMEI2() == imei) {
            return true;
        }
    }
    return false;
}
// Ten may: 2-40 ky tu, bat dau bang chu cai, khong go lap 4 ky tu lien tiep
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
// Hang san xuat: 2-25 ky tu, bat dau bang chu cai, chi gom chu cai, khoang trang, '-' va '.'
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
// Chuan hoa hang san xuat ve dang ten rieng
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

// ============================================================================
// CONSTRUCTOR
// ============================================================================
Device::Device()
    : Entity(""), maIMEI1(""), maIMEI2("NONE"), tenMay(""), hangSX(""), ngaySanXuat(""), tinhTrang("Moi") {}

Device::Device(string maThietBi, string maIMEI1, string maIMEI2, string tenMay,
               string hangSX, string ngaySanXuat, string tinhTrang)
    : Entity(maThietBi) {
    setMaIMEI1(maIMEI1);
    setMaIMEI2(maIMEI2);
    setTenMay(tenMay);
    setHangSX(hangSX);
    setNgaySanXuat(ngaySanXuat);
    setTinhTrang(tinhTrang);
}

// ============================================================================
// GETTER & SETTER
// ============================================================================
string Device::getMaIMEI1() const { return maIMEI1; }
string Device::getMaIMEI2() const { return maIMEI2; }
string Device::getTenMay() const { return tenMay; }
string Device::getHangSX() const { return hangSX; }
string Device::getNgaySanXuat() const { return ngaySanXuat; }
string Device::getTinhTrang() const { return tinhTrang; }

void Device::setMaIMEI1(const string& imei) {
    if (!hopLeIMEI(imei)) {
        throw InvalidDataException("Ma IMEI 1 phai gom dung 15 chu so va thoa man thuat toan Luhn!");
    }
    maIMEI1 = imei;
}

void Device::setMaIMEI2(const string& imei) {
    if (chuyenThuong(imei) == "none") {
        maIMEI2 = "NONE";
        return;
    }
    if (!hopLeIMEI(imei)) {
        throw InvalidDataException("Ma IMEI 2 phai gom dung 15 chu so dung chuan Luhn, hoac nhap NONE!");
    }
    if (!maIMEI1.empty() && imei == maIMEI1) {
        throw InvalidDataException("Ma IMEI 2 khong duoc trung voi Ma IMEI 1 (" + maIMEI1 + ")!");
    }
    maIMEI2 = imei;
}

void Device::setTenMay(const string& ten) {
    if (!hopLeTenMay(ten)) {
        throw InvalidDataException("Ten may (2-40 ky tu) phai bat dau bang chu cai, khong chua ky tu la hoac lap ky tu vo nghia!");
    }
    tenMay = ten;
}

void Device::setHangSX(const string& hang) {
    if (!hopLeHangSX(hang)) {
        throw InvalidDataException("Hang san xuat (2-25 ky tu) phai bat dau bang chu cai, khong chua chu so hay ky tu la!");
    }
    hangSX = chuanHoaHang(hang);
}

void Device::setNgaySanXuat(const string& ngay) {
    Date d;
    d.input(ngay);
    Date currentDate = today();
    if (d.compare(currentDate) > 0) {
        throw InvalidDataException("Ngay san xuat (" + d.output() + ") khong duoc vuot qua ngay hien tai (" + currentDate.output() + ")!");
    }
    ngaySanXuat = d.output();
}

void Device::setTinhTrang(const string& tt) {
    string thuong = chuyenThuong(tt);
    if (thuong != "moi" && thuong != "cu") {
        throw InvalidDataException("Tinh trang chi duoc nhap Moi hoac Cu!");
    }
    tinhTrang = (thuong == "moi") ? "Moi" : "Cu";
}

// ============================================================================
// PHUONG THUC KE THUA TU ENTITY
// ============================================================================
void Device::getInformation() {
    string id = InputHelper::getString("Nhap ma thiet bi (DE + so): ", false);
    if (!InputHelper::checkidvalid(id, "DE"))
        throw InvalidDataException("Ma thiet bi phai co dang DE + chu so (VD: DE01)!");
    setID(id);
    setMaIMEI1(InputHelper::getString("Nhap ma IMEI 1 (15 chu so, dung chuan Luhn): ", false));
    setMaIMEI2(InputHelper::getString("Nhap ma IMEI 2 (15 chu so hoac NONE): ", false));
    setTenMay(InputHelper::getString("Nhap ten may: ", false));
    setHangSX(InputHelper::getString("Nhap hang san xuat: ", false));
    setNgaySanXuat(InputHelper::getString("Nhap ngay san xuat (DD/MM/YYYY): ", false));
    setTinhTrang(InputHelper::getString("Nhap tinh trang (Moi / Cu): ", false));
}
void Device::display() const {
    cout << left
         << setw(8)  << ID          << " | "
         << setw(16) << maIMEI1     << " | "
         << setw(16) << maIMEI2     << " | "
         << setw(26) << tenMay      << " | "
         << setw(12) << hangSX      << " | "
         << setw(12) << ngaySanXuat << " | "
         << tinhTrang << '\n';
}
string Device::toString() const {
    ostringstream ss;
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
    getline(ss, tinhTrang, '|');
    if (!tinhTrang.empty() && tinhTrang.back() == '\r') {
        tinhTrang.pop_back();
    }
}
// ============================================================================
// IN TIEU DE & DANH SACH BANG
// ============================================================================
static void printTitleDevice() {
    cout << '\n' << string(116, '-') << '\n';
    cout << left
         << setw(8)  << "Ma TB"      << " | "
         << setw(16) << "IMEI 1"     << " | "
         << setw(16) << "IMEI 2"     << " | "
         << setw(26) << "Ten May"    << " | "
         << setw(12) << "Hang SX"    << " | "
         << setw(12) << "Ngay SX"    << " | "
         << "Tinh Trang" << '\n';
    cout << string(116, '-') << '\n';
}
static void listDevice(Repository<Device>& repo) {
    vector<Device> items = repo.getAll();
    if (items.empty()) {
        cout << "Danh sach thiet bi hien dang trong!\n";
        return;
    }
    printTitleDevice();
    for (const auto& item : items) {
        item.display();
    }
    cout << string(116, '-') << '\n';
    cout << "Tong: " << items.size() << " ban ghi.\n";
}
// ============================================================================
// THEM MOI THIET BI
// ============================================================================
static void addDevice(Repository<Device>& repo) {
    Device item;
    cout << "\n--- THEM MOI THIET BI (Nhap 0 o bat ky buoc nao de huy) ---\n";
    // 1. Nhap Ma IMEI 1
    while (true) {
        try {
            string text = InputHelper::getString("Nhap ma IMEI 1 (15 chu so chuan Luhn | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setMaIMEI1(text);
            if (imeiDaTonTai(repo, item.getMaIMEI1())) {
                throw InvalidDataException("Ma IMEI " + item.getMaIMEI1() + " da duoc dung cho thiet bi khac!");
            }
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 2. Nhap Ma IMEI 2
    while (true) {
        try {
            string text = InputHelper::getString("Nhap ma IMEI 2 (15 chu so chuan Luhn hoac NONE | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setMaIMEI2(text);
            if (imeiDaTonTai(repo, item.getMaIMEI2())) {
                throw InvalidDataException("Ma IMEI " + item.getMaIMEI2() + " da duoc dung cho thiet bi khac!");
            }
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 3. Nhap Ten may
    while (true) {
        try {
            string text = InputHelper::getString("Nhap ten may (VD: iPhone 15, Galaxy S24 | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setTenMay(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 4. Nhap Hang san xuat
    while (true) {
        try {
            string text = InputHelper::getString("Nhap hang san xuat (VD: Apple, Samsung | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setHangSX(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 5. Nhap Ngay san xuat
    while (true) {
        try {
            string text = InputHelper::getString("Nhap ngay san xuat (DD/MM/YYYY | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setNgaySanXuat(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 6. Nhap Tinh trang
    while (true) {
        try {
            string text = InputHelper::getString("Nhap tinh trang (Moi / Cu | 0 de quay lai): ", false);
            if (text == "0") { cout << "Da huy thao tac them.\n"; return; }
            item.setTinhTrang(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 7. Tu dong sinh ma
    try {
        string maThietBi = InputHelper::generateAutoID(repo, "DE", "data_counter/device_counter.txt");
        item.setID(maThietBi);
        repo.add(item);
        cout << "--> Them thiet bi " << maThietBi << " thanh cong!\n";
        printTitleDevice();
        item.display();
    } catch (const exception& error) {
        cout << "Loi: " << error.what() << '\n';
    }
}

// ============================================================================
// TIM KIEM THIET BI DA NANG (MA TB / IMEI / TEN MAY / HANG SX)
// ============================================================================
static void findDevice(Repository<Device>& repo) {
    while (true) {
        string keyword = InputHelper::getString("Nhap Ma TB (DE+so), IMEI, Ten may hoac Hang SX can tim (0 de quay lai): ", false);
        if (keyword == "0") return;

        string keyLower = chuyenThuong(keyword);
        vector<Device> results;
        for (const auto& item : repo.getAll()) {
            if (item.getID() == keyword ||
                item.getMaIMEI1() == keyword ||
                item.getMaIMEI2() == keyword ||
                chuyenThuong(item.getTenMay()).find(keyLower) != string::npos ||
                chuyenThuong(item.getHangSX()).find(keyLower) != string::npos) {
                results.push_back(item);
            }
        }
        if (results.empty()) {
            cout << "Khong tim thay thiet bi nao khop voi '" << keyword << "'. Vui long thu lai.\n";
            continue;
        }
        printTitleDevice();
        for (const auto& item : results) {
            item.display();
        }
        cout << string(116, '-') << '\n';
        cout << "Tim thay " << results.size() << " ket qua.\n";
        return;
    }
}

// ============================================================================
// CAP NHAT THONG TIN THIET BI
// ============================================================================
static void updateDevice(Repository<Device>& repo, Repository<Sim>& repo_sim) {
    Device* found = nullptr;
    string code;
    while (true) {
        code = InputHelper::getString("Nhap ma thiet bi can cap nhat (DE + so | 0 de quay lai): ", false);
        if (code == "0") return;
        if (!InputHelper::checkidvalid(code, "DE")) {
            cout << "Loi: Ma thiet bi phai co dang DE + chu so (VD: DE01)!\n";
            continue;
        }
        found = repo.findById(code);
        if (found != nullptr) break;
        cout << "Khong tim thay ma " << code << ". Vui long nhap lai.\n";
    }

    printTitleDevice();
    found->display();

    Device update = *found;

    // 1. Cap nhat IMEI 1 (Chan sua neu IMEI 1 cu dang duoc SIM su dung)
    while (true) {
        string text = InputHelper::getString("Nhap ma IMEI 1 moi (15 so chuan Luhn | 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            for (const auto& sim : repo_sim.getAll()) {
                if (sim.getMaIMEI() == found->getMaIMEI1()) {
                    throw InvalidDataException("Khong the doi IMEI 1 vi SIM " + sim.getSoSim() + " dang gan voi IMEI nay!");
                }
            }
            Device temp = update;
            temp.setMaIMEI1(text);
            if (temp.getMaIMEI2() != "NONE" && temp.getMaIMEI1() == temp.getMaIMEI2()) {
                throw InvalidDataException("Ma IMEI 1 moi khong duoc trung voi Ma IMEI 2 hien tai (" + temp.getMaIMEI2() + ")!");
            }
            if (imeiDaTonTai(repo, temp.getMaIMEI1(), code)) {
                throw InvalidDataException("Ma IMEI " + temp.getMaIMEI1() + " da duoc dung cho thiet bi khac!");
            }
            update.setMaIMEI1(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 2. Cap nhat IMEI 2 (Chan sua neu IMEI 2 cu dang duoc SIM su dung)
    while (true) {
        string text = InputHelper::getString("Nhap ma IMEI 2 moi (15 so chuan Luhn hoac NONE | 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            if (found->getMaIMEI2() != "NONE") {
                for (const auto& sim : repo_sim.getAll()) {
                    if (sim.getMaIMEI() == found->getMaIMEI2()) {
                        throw InvalidDataException("Khong the doi IMEI 2 vi SIM " + sim.getSoSim() + " dang gan voi IMEI nay!");
                    }
                }
            }
            Device temp = update;
            temp.setMaIMEI2(text);
            if (imeiDaTonTai(repo, temp.getMaIMEI2(), code)) {
                throw InvalidDataException("Ma IMEI " + temp.getMaIMEI2() + " da duoc dung cho thiet bi khac!");
            }
            update.setMaIMEI2(temp.getMaIMEI2());
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 3. Cap nhat Ten may
    while (true) {
        string text = InputHelper::getString("Nhap ten may moi (0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            update.setTenMay(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 4. Cap nhat Hang san xuat
    while (true) {
        string text = InputHelper::getString("Nhap hang san xuat moi (0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            update.setHangSX(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 5. Cap nhat Ngay san xuat
    while (true) {
        string text = InputHelper::getString("Nhap ngay san xuat moi (DD/MM/YYYY | 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            update.setNgaySanXuat(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 6. Cap nhat Tinh trang
    while (true) {
        string text = InputHelper::getString("Nhap tinh trang moi (Moi / Cu | 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            update.setTinhTrang(text);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    repo.update(code, update);
    cout << "--> Cap nhat thiet bi thanh cong!\n";
    printTitleDevice();
    update.display();
}

// ============================================================================
// XOA THIET BI (CHAN NEU DANG DUOC SIM SU DUNG)
// ============================================================================
static void removeDevice(Repository<Device>& repo, Repository<Sim>& repo_sim) {
    while (true) {
        string id = InputHelper::getString("Nhap ma thiet bi can xoa (DE + so | 0 de quay lai): ", false);
        if (id == "0") return;

        try {
            if (!InputHelper::checkidvalid(id, "DE")) {
                throw InvalidDataException("Ma thiet bi khong hop le (dang DE + chu so, VD: DE01)!");
            }
            Device* p = repo.findById(id);
            if (p == nullptr) {
                cout << "Khong tim thay thiet bi co ma " << id << ". Vui long nhap lai.\n";
                continue;
            }

            printTitleDevice();
            p->display();

            // Kiem tra cheo: IMEI (khe 1 hoac khe 2) dang duoc SIM tham chieu thi khong cho xoa
            string imei1 = p->getMaIMEI1();
            string imei2 = p->getMaIMEI2();
            for (const auto& sim : repo_sim.getAll()) {
                string imeiCuaSim = sim.getMaIMEI();
                if (imeiCuaSim == imei1 || (imei2 != "NONE" && imeiCuaSim == imei2)) {
                    cout << "Loi nghiep vu: Thiet bi " << id << " khong the xoa vi SIM "
                         << sim.getSoSim() << " dang su dung IMEI " << imeiCuaSim << "!\n";
                    return;
                }
            }

            if (!InputHelper::getConfirm("Ban co chac chan muon xoa vinh vien thiet bi nay? (y/n): ")) {
                cout << "Da huy thao tac xoa.\n";
                return;
            }

            repo.remove(id);
            cout << "--> Da xoa thiet bi " << id << " khoi he thong.\n";
            return;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
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
        cout << "Canh bao: " << e.what() << '\n';
        return;
    }

    int chon = -1;
    do {
        cout << "\n========================================\n";
        cout << "        QUAN LY THIET BI (IMEI)         \n";
        cout << "========================================\n";
        cout << "1. Xem danh sach thiet bi\n";
        cout << "2. Them thiet bi moi\n";
        cout << "3. Tim kiem thiet bi (Ma TB / IMEI / Ten / Hang)\n";
        cout << "4. Cap nhat thiet bi\n";
        cout << "5. Xoa thiet bi\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (chon) {
            case 1: listDevice(repo);               break;
            case 2: addDevice(repo);                break;
            case 3: findDevice(repo);               break;
            case 4: updateDevice(repo, repo_sim);   break;
            case 5: removeDevice(repo, repo_sim);   break;
            case 0: break;
        }
    } while (chon != 0);
}