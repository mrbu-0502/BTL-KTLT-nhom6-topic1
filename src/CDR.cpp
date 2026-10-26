// ===============================================================
// Hoc vien Cong nghe Buu chinh Vien thong
// Hoc phan: Ky thuat lap trinh
// De tai 1: Quan ly thue bao di dong
// Tac gia: Kieu Duc Hiep
// Use Case: Quan ly CDR
// ===============================================================
#include "CDR.h"
#include "Sim.h"
#include "Date.h"
#include "InputHelper.h"
#include "Exceptions.h"
#include "Repository.h"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
static const int DON_GIA_GIAY = 25; // 25 VND/giay = 1500 VND/phut

// ============================================================================
// HAM BO TRO
// ============================================================================
static Date today() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    return Date(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

static bool laChuSo(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(),
                                [](unsigned char c) { return isdigit(c) != 0; });
}

// Kiem tra Ma CDR (CDR + so > 0)
static void kiemTraMaCDR(const string& value) {
    if (value.size() < 4 || value.substr(0, 3) != "CDR")
        throw InvalidDataException("Ma CDR phai bat dau bang CDR va co so theo sau (VD: CDR01)!");
    if (!laChuSo(value.substr(3)) || stoi(value.substr(3)) <= 0)
        throw InvalidDataException("Sau CDR phai la cac chu so lon hon 0!");
}

// Kiem tra va chuan hoa Thoi diem goi: "DD/MM/YYYY HH:MM:SS"
// Tra ve doi tuong Date (ngay goi) de doi chieu voi ngay kich hoat SIM
static Date kiemTraVaChuanHoaThoiDiem(string& thoiDiem) {
    if (thoiDiem.size() != 19 || thoiDiem[10] != ' ' || thoiDiem[13] != ':' || thoiDiem[16] != ':') {
        throw InvalidDataException("Thoi diem goi phai dung dinh dang DD/MM/YYYY HH:MM:SS (VD: 25/10/2026 08:30:15)!");
    }

    string phanNgay = thoiDiem.substr(0, 10);
    string sGio  = thoiDiem.substr(11, 2);
    string sPhut = thoiDiem.substr(14, 2);
    string sGiay = thoiDiem.substr(17, 2);
    Date callDate;

    callDate.input(phanNgay);

    //Kiem tra khong duoc vuot qua ngay hien tai he thong
    Date currentDate = today();
    if (callDate.compare(currentDate) > 0) {
        throw InvalidDataException("Ngay goi (" + callDate.output() + ") khong duoc vuot qua ngay hien tai (" + currentDate.output() + ")!");
    }
    //Kiem tra Gio:Phut:Giay
    if (!laChuSo(sGio) || !laChuSo(sPhut) || !laChuSo(sGiay)) {
        throw InvalidDataException("Gio, phut, giay phai la cac chu so!");
    }
    int gio = stoi(sGio), phut = stoi(sPhut), giay = stoi(sGiay);
    if (gio < 0 || gio > 23 || phut < 0 || phut > 59 || giay < 0 || giay > 59) {
        throw InvalidDataException("Gio (00-23), Phut (00-59), Giay (00-59) khong hop le!");
    }
    thoiDiem = callDate.output() + " " + sGio + ":" + sPhut + ":" + sGiay;
    return callDate;
}
// Kiem tra Thoi luong goi (1 - 86400 giay)
static int kiemTraSoGiay(const string& value) {
    if (value.empty() || value.size() > 5 || !laChuSo(value)) {
        throw InvalidDataException("Thoi luong cuoc goi phai la so nguyen tu 1 den 86400 giay!");
    }
    int giay = stoi(value);
    if (giay < 1 || giay > 86400) {
        throw InvalidDataException("Thoi luong cuoc goi phai nam trong khoang [1 - 86400] giay!");
    }
    return giay;
}
// Kiem tra SIM goi: Phai ton tai trong kho SIM va dang o trang thai HoatDong
static Sim* kiemTraSimGoi(Repository<Sim>& simRepo, const string& soGoi) {
    Sim* sim = simRepo.findById(soGoi);
    if (sim == nullptr) {
        throw InvalidDataException("So thue bao goi (" + soGoi + ") khong ton tai trong kho SIM!");
    }
    if (sim->getTrangThai() != "HoatDong") {
        throw InvalidDataException("So thue bao goi (" + soGoi + ") dang bi khoa, khong the thuc hien cuoc goi!");
    }
    return sim;
}

// Kiem tra ngay goi phai >= ngay kich hoat SIM
static void kiemTraNgayGoiVoiSim(Sim* sim, Date& callDate) {
    Date simDate;
    simDate.input(sim->getNgayKichHoat());
    if (callDate.compare(simDate) < 0) {
        throw InvalidDataException("Ngay goi (" + callDate.output() + ") khong duoc som hon ngay kich hoat SIM (" + simDate.output() + ")!");
    }
}
// ============================================================================
// CONSTRUCTOR
// ============================================================================
CDR::CDR() {
    MaCDR = "";
    SoGoi = "";
    SoNhan = "";
    ThoiDiem = "";
    SoGiay = "0";
    CuocPhi = "0";
}

// ============================================================================
// KE THUA TU ENTITY
// ============================================================================
string CDR::getID() const {
    return MaCDR;
}

void CDR::setID(const string& value) {
    kiemTraMaCDR(value);
    ID = value;
    MaCDR = value;
}

void CDR::getInformation() {
    string ma = InputHelper::getString("Nhap ma CDR (CDR + so): ", false);
    setID(ma);

    string goi = InputHelper::getString("Nhap so dien thoai goi (10 so): ", false);
    if (!InputHelper::checkPhoneNumber(goi))
        throw InvalidDataException("SDT goi phai gom 10 chu so va bat dau bang 0!");
    SoGoi = goi;

    string nhan = InputHelper::getString("Nhap so dien thoai nhan (10 so): ", false);
    if (!InputHelper::checkPhoneNumber(nhan))
        throw InvalidDataException("SDT nhan phai gom 10 chu so va bat dau bang 0!");
    if (nhan == goi)
        throw InvalidDataException("SDT nhan khong duoc trung voi SDT goi!");
    SoNhan = nhan;

    string td = InputHelper::getString("Nhap thoi diem goi (DD/MM/YYYY HH:MM:SS): ", false);
    kiemTraVaChuanHoaThoiDiem(td);
    ThoiDiem = td;

    string sg = InputHelper::getString("Nhap thoi luong goi (1 - 86400 giay): ", false);
    int giay = kiemTraSoGiay(sg);
    SoGiay = to_string(giay);
    CuocPhi = to_string(giay * DON_GIA_GIAY);
}

void CDR::display() const {
    cout << left
         << setw(10) << MaCDR    << " | "
         << setw(13) << SoGoi    << " | "
         << setw(13) << SoNhan   << " | "
         << setw(21) << ThoiDiem << " | "
         << right << setw(12) << SoGiay  << " | "
         << right << setw(14) << CuocPhi << '\n' << left;
}

string CDR::toString() const {
    ostringstream out;
    out << MaCDR << "|" << SoGoi << "|" << SoNhan << "|"
        << ThoiDiem << "|" << SoGiay << "|" << CuocPhi;
    return out.str();
}

void CDR::fromString(const string& line) {
    stringstream ss(line);
    if (!getline(ss, MaCDR, '|') ||
        !getline(ss, SoGoi, '|') ||
        !getline(ss, SoNhan, '|') ||
        !getline(ss, ThoiDiem, '|') ||
        !getline(ss, SoGiay, '|') ||
        !getline(ss, CuocPhi, '|')) {
        throw InvalidDataException("Dong du lieu CDR sai dinh dang: " + line);
    }
    if (!CuocPhi.empty() && CuocPhi.back() == '\r') {
        CuocPhi.pop_back();
    }
    ID = MaCDR;
}

// ============================================================================
// IN TIEU DE & DANH SACH BANG
// ============================================================================
static void printTitleCDR() {
    cout << '\n' << string(98, '-') << '\n';
    cout << left
         << setw(10) << "Ma CDR"          << " | "
         << setw(13) << "So goi"          << " | "
         << setw(13) << "So nhan"         << " | "
         << setw(21) << "Thoi diem goi"   << " | "
         << right << setw(12) << "So giay (s)" << " | "
         << right << setw(14) << "Cuoc phi (VND)" << '\n' << left;
    cout << string(98, '-') << '\n';
}

static void listCDR(Repository<CDR>& repo) {
    vector<CDR> items = repo.getAll();
    if (items.empty()) {
        cout << "Danh sach cuoc goi hien dang trong!\n";
        return;
    }
    printTitleCDR();
    for (const auto& item : items) {
        item.display();
    }
    cout << string(98, '-') << '\n';
    cout << "Tong: " << items.size() << " ban ghi.\n";
}

// ============================================================================
// THEM BAN GHI CUOC GOI MOI
// ============================================================================
static void addCDR(Repository<CDR>& cdrRepo, Repository<Sim>& simRepo) {
    cout << "\n--- THEM BAN GHI CUOC GOI MOI (Nhap 0 o bat ky buoc nao de huy) ---\n";
    string soGoi, soNhan, thoiDiem, soGiayStr, cuocPhiStr;
    Sim* simGoi = nullptr;

    // 1. Nhap So dien thoai goi (Phai ton tai trong kho SIM va HoatDong)
    while (true) {
        try {
            soGoi = InputHelper::getString("Nhap so dien thoai goi (10 so | 0 de quay lai): ", false);
            if (soGoi == "0") { cout << "Da huy thao tac them.\n"; return; }
            InputHelper::xuly_84(soGoi);
            if (!InputHelper::checkPhoneNumber(soGoi))
                throw InvalidDataException("SDT goi phai gom 10 chu so va bat dau bang 0!");
            simGoi = kiemTraSimGoi(simRepo, soGoi);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 2. Nhap So dien thoai nhan (10 so, khong trung voi so goi)
    while (true) {
        try {
            soNhan = InputHelper::getString("Nhap so dien thoai nhan (10 so | 0 de quay lai): ", false);
            if (soNhan == "0") { cout << "Da huy thao tac them.\n"; return; }
            InputHelper::xuly_84(soNhan);
            if (!InputHelper::checkPhoneNumber(soNhan))
                throw InvalidDataException("SDT nhan phai gom 10 chu so va bat dau bang 0!");
            if (soNhan == soGoi)
                throw InvalidDataException("SDT nhan khong duoc trung voi SDT goi (" + soGoi + ")!");
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 3. Nhap Thoi diem goi (>= Ngay kich hoat SIM va <= Hom nay)
    while (true) {
        try {
            thoiDiem = InputHelper::getString("Thoi diem goi (DD/MM/YYYY HH:MM:SS | 0 de quay lai): ", false);
            if (thoiDiem == "0") { cout << "Da huy thao tac them.\n"; return; }
            Date callDate = kiemTraVaChuanHoaThoiDiem(thoiDiem);
            kiemTraNgayGoiVoiSim(simGoi, callDate);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 4. Nhap Thoi luong goi  & Tu dong tinh cuoc
    while (true) {
        try {
            int giay = InputHelper::getInt("Thoi luong cuoc goi (1 - 86400 giay | 0 de quay lai): ", 0, 86400);
            if (giay == 0) { cout << "Da huy thao tac them.\n"; return; }
            soGiayStr = to_string(giay);
            cuocPhiStr = to_string(giay * DON_GIA_GIAY);
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
    // 5. Tu dong sinh ma CDR
    try {
        string maCDR = InputHelper::generateAutoID(cdrRepo, "CDR", "data_counter/cdr_counter.txt");
        CDR item;
        item.fromString(maCDR + "|" + soGoi + "|" + soNhan + "|" + thoiDiem + "|" + soGiayStr + "|" + cuocPhiStr);
        cdrRepo.add(item);
        cout << "--> Them cuoc goi " << maCDR << " thanh cong (Cuoc phi tu dong tinh: " << cuocPhiStr << " VND)!\n";
        printTitleCDR();
        item.display();
    } catch (const exception& error) {
        cout << "Loi: " << error.what() << '\n';
    }
}

// ============================================================================
// TIM KIEM CUOC GOI DA NANG (MA CDR / SDT GOI / SDT NHAN / NGAY GOI)
// ============================================================================
static void findCDR(Repository<CDR>& repo) {
    while (true) {
        string keyword = InputHelper::getString("Nhap Ma CDR, SDT goi/nhan hoac Ngay (DD/MM/YYYY) can tim (0 de quay lai): ", false);
        if (keyword == "0") return;

        vector<CDR> results;
        for (const auto& item : repo.getAll()) {
            if (item.getID() == keyword || item.toString().find(keyword) != string::npos) {
                results.push_back(item);
            }
        }
        if (results.empty()) {
            cout << "Khong tim thay ban ghi cuoc goi nao khop voi '" << keyword << "'. Vui long thu lai.\n";
            continue;
        }
        printTitleCDR();
        for (const auto& item : results) {
            item.display();
        }
        cout << string(98, '-') << '\n';
        cout << "Tim thay " << results.size() << " ket qua.\n";
        return;
    }
}

// ============================================================================
// CAP NHAT THONG TIN CUOC GOI
// ============================================================================
static void updateCDR(Repository<CDR>& cdrRepo, Repository<Sim>& simRepo) {
    CDR* found = nullptr;
    string code;
    while (true) {
        code = InputHelper::getString("Nhap ma CDR can cap nhat (CDR + so | 0 de quay lai): ", false);
        if (code == "0") return;
        try {
            kiemTraMaCDR(code);
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
            continue;
        }
        found = cdrRepo.findById(code);
        if (found != nullptr) break;
        cout << "Khong tim thay ma " << code << ". Vui long nhap lai.\n";
    }

    printTitleCDR();
    found->display();

    // Tach du lieu cu tu ban ghi dang sua
    stringstream ss(found->toString());
    string maCu, soGoiMoi, soNhanMoi, thoiDiemMoi, soGiayMoi, cuocPhiMoi;
    getline(ss, maCu, '|');
    getline(ss, soGoiMoi, '|');
    getline(ss, soNhanMoi, '|');
    getline(ss, thoiDiemMoi, '|');
    getline(ss, soGiayMoi, '|');
    getline(ss, cuocPhiMoi, '|');

    // 1. Cap nhat So goi
    while (true) {
        string text = InputHelper::getString("Nhap SDT goi moi (10 so | 0 de giu nguyen): ", false);
        if (text == "0") break;
        try {
            InputHelper::xuly_84(text);
            if (!InputHelper::checkPhoneNumber(text))
                throw InvalidDataException("SDT goi phai gom 10 chu so va bat dau bang 0!");
            kiemTraSimGoi(simRepo, text);
            soGoiMoi = text;
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 2. Cap nhat So nhan (Kiem tra cheo khong duoc trung voi soGoiMoi)
    while (true) {
        string text = InputHelper::getString("Nhap SDT nhan moi (10 so | 0 de giu nguyen): ", false);
        try {
            string candidateNhan = (text == "0") ? soNhanMoi : text;
            InputHelper::xuly_84(candidateNhan);
            if (!InputHelper::checkPhoneNumber(candidateNhan))
                throw InvalidDataException("SDT nhan phai gom 10 chu so va bat dau bang 0!");
            if (candidateNhan == soGoiMoi)
                throw InvalidDataException("SDT nhan (" + candidateNhan + ") khong duoc trung voi SDT goi (" + soGoiMoi + ")! Vui long nhap so nhan khac.");
            soNhanMoi = candidateNhan;
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 3. Cap nhat Thoi diem goi (Kiem tra cheo voi ngay kich hoat cua soGoiMoi ke ca khi bam 0)
    while (true) {
        string text = InputHelper::getString("Thoi diem goi moi (DD/MM/YYYY HH:MM:SS | 0 de giu nguyen): ", false);
        try {
            string candidateTD = (text == "0") ? thoiDiemMoi : text;
            Date callDate = kiemTraVaChuanHoaThoiDiem(candidateTD);
            Sim* simGoi = simRepo.findById(soGoiMoi);
            if (simGoi == nullptr)
                throw InvalidDataException("So SIM goi " + soGoiMoi + " khong ton tai!");
            kiemTraNgayGoiVoiSim(simGoi, callDate);
            thoiDiemMoi = candidateTD;
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    // 4. Cap nhat Thoi luong goi & Tu dong tinh lai Cuoc phi
    while (true) {
        try {
            int giay = InputHelper::getInt("Thoi luong goi moi (1 - 86400 giay | 0 de giu nguyen): ", 0, 86400);
            if (giay != 0) {
                soGiayMoi = to_string(giay);
                cuocPhiMoi = to_string(giay * DON_GIA_GIAY);
            }
            break;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }

    CDR updated;
    updated.fromString(code + "|" + soGoiMoi + "|" + soNhanMoi + "|" + thoiDiemMoi + "|" + soGiayMoi + "|" + cuocPhiMoi);
    cdrRepo.update(code, updated);
    cout << "--> Cap nhat ban ghi cuoc goi thanh cong!\n";
    printTitleCDR();
    updated.display();
}

// ============================================================================
// XOA BAN GHI CUOC GOI
// ============================================================================
static void removeCDR(Repository<CDR>& cdrRepo) {
    while (true) {
        string text = InputHelper::getString("Nhap ma CDR can xoa (CDR + so | 0 de quay lai): ", false);
        if (text == "0") return;
        try {
            kiemTraMaCDR(text);
            CDR* found = cdrRepo.findById(text);
            if (found == nullptr) {
                cout << "Khong tim thay ma " << text << ". Vui long nhap lai.\n";
                continue;
            }
            printTitleCDR();
            found->display();
            if (!InputHelper::getConfirm("Ban co chac muon xoa ban ghi cuoc goi nay? (y/n): ")) {
                cout << "Da huy thao tac xoa.\n";
                return;
            }
            cdrRepo.remove(text);
            cout << "--> Da xoa ban ghi cuoc goi " << text << " khoi he thong.\n";
            return;
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
}

// ============================================================================
// MENU QUAN LY CUOC GOI (CDR)
// ============================================================================
void menuCuocGoi() {
    Repository<CDR> cdrRepo("data/cdrs.txt");
    Repository<Sim> simRepo("data/sims.txt");
    try {
        cdrRepo.loadFromFile();
        simRepo.loadFromFile();
    } catch (const exception& e) {
        cout << "Canh bao: " << e.what() << '\n';
        return;
    }

    int choice = -1;
    do {
        cout << "\n========================================\n";
        cout << "        QUAN LY CUOC GOI (CDR)\n";
        cout << "========================================\n";
        cout << "1. Xem danh sach cuoc goi\n";
        cout << "2. Them ban ghi cuoc goi moi\n";
        cout << "3. Tim kiem cuoc goi (Ma CDR / SDT / Ngay)\n";
        cout << "4. Cap nhat thong tin cuoc goi\n";
        cout << "5. Xoa ban ghi cuoc goi\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "========================================\n";
        choice = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        switch (choice) {
            case 1: listCDR(cdrRepo); break;
            case 2: addCDR(cdrRepo, simRepo); break;
            case 3: findCDR(cdrRepo); break;
            case 4: updateCDR(cdrRepo, simRepo); break;
            case 5: removeCDR(cdrRepo); break;
            case 0: break;
        }
    } while (choice != 0);
}