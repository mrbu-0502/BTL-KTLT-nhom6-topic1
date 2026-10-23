#include "Device.h"     
#include "InputHelper.h" 
#include "Repository.h"

using namespace std;

// ============================================================================
// 1. HAM KHOI TAO: Dat tat ca 5 thuoc tinh ban dau la chuoi rong ""
// ============================================================================
Device::Device() {
    MaIMEI = "";
    TenMay = "";
    HangSX = "";
    NamSX = "";
    TinhTrang = "";
}

// ============================================================================
// 2. QUAN LY MA DINH DANH (ID) CUA THIET BI
// ============================================================================
string Device::getID() const {
    return MaIMEI;
}

void Device::setID(const string& value) {
    ID = value;
    MaIMEI = value;
}

// ============================================================================
// 3. NHAP DU LIEU TU BAN PHIM (Co kiem tra ngoai le cho tat ca cac truong)
// ============================================================================
void Device::getInformation() {
    // 1. Kiem tra Ma IMEI: Dung 15 chu so
    while (true) {
        MaIMEI = InputHelper::getString("Nhap ma IMEI (dung 15 chu so): ");
        if (MaIMEI.length() != 15) {
            cout << "Loi: Ma IMEI phai co dung 15 chu so! Vui long nhap lai.\n";
            continue;
        }
        bool hopLe = true;
        for (char c : MaIMEI) {
            if (!isdigit(c)) {
                hopLe = false;
                break;
            }
        }
        if (!hopLe) {
            cout << "Loi: Ma IMEI chi duoc chua cac ky tu so (0-9)! Vui long nhap lai.\n";
            continue;
        }
        break;
    }
    ID = MaIMEI;

    // 2. Kiem tra Ten may: Khong rong, khong chua dau '|'
    while (true) {
        TenMay = InputHelper::getString("Nhap ten may: ");
        if (TenMay.empty() || TenMay.find('|') != string::npos) {
            cout << "Loi: Ten may khong hop le! Vui long nhap lai.\n";
            continue;
        }
        break;
    }

    // 3. Kiem tra Hang san xuat: Khong rong, khong chua dau '|'
    while (true) {
        HangSX = InputHelper::getString("Nhap hang san xuat: ");
        if (HangSX.empty() || HangSX.find('|') != string::npos) {
            cout << "Loi: Hang san xuat khong hop le! Vui long nhap lai.\n";
            continue;
        }
        break;
    }

    // 4. Kiem tra Nam san xuat: Phai la 4 chu so (1990 - 2026)
    while (true) {
        NamSX = InputHelper::getString("Nhap nam san xuat (1990-2026): ");
        bool hopLe = (NamSX.length() == 4);
        for (char c : NamSX) {
            if (!isdigit(c)) hopLe = false;
        }
        if (hopLe) {
            int nam = stoi(NamSX);
            if (nam < 1990 || nam > 2026) hopLe = false;
        }
        if (!hopLe) {
            cout << "Loi: Nam san xuat phai la so co 4 chu so (tu 1990 den 2026)!\n";
            continue;
        }
        break;
    }

    // 5. Kiem tra Tinh trang: Chi nhan "Moi" hoac "Cu"
    while (true) {
        TinhTrang = InputHelper::getString("Nhap tinh trang (Moi/Cu): ");
        if (TinhTrang != "Moi" && TinhTrang != "Cu") {
            cout << "Loi: Tinh trang chi duoc nhap 'Moi' hoac 'Cu'!\n";
            continue;
        }
        break;
    }
}

// ============================================================================
// 4. HIEN THI THONG TIN THIET BI RA MAN HINH CONSOLE
// ============================================================================
void Device::display() const {
    cout << "IMEI: " << MaIMEI 
         << " | May: " << TenMay 
         << " | Hang: " << HangSX 
         << " | Nam: " << NamSX 
         << " | Tinh trang: " << TinhTrang << endl;
}

// ============================================================================
// 5. DONG GOI DU LIEU THANH 1 DONG TEXT DE GHI VAO FILE devices.txt
// ============================================================================
string Device::toString() const {
    return MaIMEI + "|" + TenMay + "|" + HangSX + "|" + NamSX + "|" + TinhTrang;
}

// ============================================================================
// 6. TACH DONG TEXT TU FILE devices.txt RA THANH 5 BIEN RIENG BIET
// ============================================================================
void Device::fromString(const string& line) {
    int p1 = line.find('|');
    MaIMEI = line.substr(0, p1); 
    
    int p2 = line.find('|', p1 + 1);
    TenMay = line.substr(p1 + 1, p2 - p1 - 1);
    
    int p3 = line.find('|', p2 + 1);
    HangSX = line.substr(p2 + 1, p3 - p2 - 1);
    
    int p4 = line.find('|', p3 + 1);
    NamSX = line.substr(p3 + 1, p4 - p3 - 1);
    
    TinhTrang = line.substr(p4 + 1);
    ID = MaIMEI; 
}

// ============================================================================
// 7. MENU QUAN LY THIET BI (Cho khach hang lua chon xem va thao tac)
// ============================================================================
void menuThietBi() {
    Repository<Device> repo("data/devices.txt");
    repo.loadFromFile();

    int chon = -1;
    while (chon != 0) {
        cout << "\n========== QUAN LY THIET BI (IMEI) ==========\n";
        cout << "1. Xem toan bo danh sach thiet bi\n";
        cout << "2. Them thiet bi moi\n";
        cout << "3.
