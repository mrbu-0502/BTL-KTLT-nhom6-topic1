#include <iostream>
#include "Device.h"
#include "InputHelper.h"
#include "Repository.h"

using namespace std;

// 1. Kiem tra chuoi chi chua chu so (Tra ve 1 neu dung, 0 neu sai)
int laSoDevice(const string& s) {
    if (s == "") return 0;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }
    return 1;
}

// 2. Kiem tra Ma IMEI: Dung 15 chu so va khong phai 15 so giong het nhau
int hopLeIMEI(const string& s) {
    if (s.length() != 15 || laSoDevice(s) == 0) return 0;
    int trungHet = 1;
    for (int i = 1; i < 15; i++) {
        if (s[i] != s[0]) trungHet = 0;
    }
    if (trungHet == 1) return 0;
    return 1;
}

// 3. Kiem tra Ten may: Bat dau bang chu cai, khong go lap 3 ky tu (chan "2233", "iphoneeee")
int hopLeTenMay(const string& s) {
    if (s.length() < 2 || s.length() > 30) return 0;
    char d = s[0];
    if (!((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z'))) return 0;

    for (int i = 0; i < (int)s.length(); i++) {
        char c = s[i];
        int hopLe = 0;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == ' ' || c == '+' || c == '-') {
            hopLe = 1;
        }
        if (hopLe == 0) return 0;
        if (i >= 2 && s[i] == s[i - 1] && s[i] == s[i - 2]) return 0;
        if (i >= 1 && s[i] == ' ' && s[i - 1] == ' ') return 0;
    }
    return 1;
}

// 4. Kiem tra Hang SX: Chi nhan ten hang dien thoai co that (chan "233223", "appleeee")
int hopLeHangSX(string& s) {
    if (s == "") return 0;
    string thuong = "";
    for (int i = 0; i < (int)s.length(); i++) {
        char c = s[i];
        if (c >= 'A' && c <= 'Z') c = c + 32;
        thuong += c;
    }
    string dsHang = "|apple|samsung|xiaomi|oppo|vivo|realme|nokia|huawei|honor|sony|asus|google|lg|motorola|viettel|vsmart|bphone|tecno|infinix|oneplus|htc|lenovo|zte|masstel|nothing|blackberry|vertu|";
    if (dsHang.find("|" + thuong + "|") == string::npos) return 0;

    s = thuong;
    s[0] = s[0] - 32; // Viet hoa chu cai dau (VD: "apple" -> "Apple")
    return 1;
}

// ============================================================================
// 1. HAM KHOI TAO
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
// 3. NHAP DU LIEU TU BAN PHIM (Chan sai dinh dang & trung ma ngay tai cho)
// ============================================================================
void Device::getInformation() {
    if (MaIMEI == "") {
        Repository<Device> repoCheck("data/devices.txt");
        repoCheck.loadFromFile();
        while (1) {
            MaIMEI = InputHelper::getString("Nhap ma IMEI (dung 15 chu so): ");
            if (hopLeIMEI(MaIMEI) == 0) {
                cout << "Loi: Ma IMEI phai gom 15 chu so (0-9) va khong duoc trung 15 so!\n";
                continue;
            }
            if (repoCheck.findById(MaIMEI) != nullptr) {
                cout << "Loi: Ma IMEI [" << MaIMEI << "] da ton tai! Vui long nhap ma khac.\n";
                continue;
            }
            break;
        }
        ID = MaIMEI;
    }

    while (1) {
        TenMay = InputHelper::getString("Nhap ten may (VD: iPhone 15, Galaxy S24): ");
        if (hopLeTenMay(TenMay) == 1) break;
        cout << "Loi: Ten may phai bat dau bang chu cai va khong go lap ky tu vo nghia!\n";
    }

    while (1) {
        HangSX = InputHelper::getString("Nhap hang san xuat (VD: Apple, Samsung, Xiaomi...): ");
        if (hopLeHangSX(HangSX) == 1) break;
        cout << "Loi: Hang san xuat khong hop le! Hay nhap dung ten hang (Apple, Samsung, Xiaomi, Oppo, Vivo, Nokia...).\n";
    }

    while (1) {
        NamSX = InputHelper::getString("Nhap nam san xuat (1980-2026): ");
        if (NamSX.length() == 4 && laSoDevice(NamSX) == 1 && NamSX >= "1980" && NamSX <= "2026") break;
        cout << "Loi: Nam san xuat phai la 4 chu so tu 1980 den 2026!\n";
    }

    while (1) {
        TinhTrang = InputHelper::getString("Nhap tinh trang (Moi/Cu): ");
        if (TinhTrang == "Moi" || TinhTrang == "moi") {
            TinhTrang = "Moi";
            break;
        }
        if (TinhTrang == "Cu" || TinhTrang == "cu") {
            TinhTrang = "Cu";
            break;
        }
        cout << "Loi: Tinh trang chi duoc nhap 'Moi' hoac 'Cu'!\n";
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
// 7. MENU QUAN LY THIET BI
// ============================================================================
void menuThietBi() {
    Repository<Device> repo("data/devices.txt");
    repo.loadFromFile();

    int chon = -1;
    while (chon != 0) {
        cout << "\n========== QUAN LY THIET BI (IMEI) ==========\n";
        cout << "1. Xem toan bo danh sach thiet bi\n";
        cout << "2. Them thiet bi moi\n";
        cout << "3. Tim kiem & loc thiet bi theo lua chon\n";
        cout << "4. Sua thong tin thiet bi\n";
        cout << "5. Xoa thiet bi\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "=============================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        if (chon == 1) {
            vector<Device> ds = repo.getAll();
            if (ds.empty()) {
                cout << "Danh sach thiet bi dang trong!\n";
            } else {
                cout << "\n--- DANH SACH THIET BI ---\n";
                for (int i = 0; i < (int)ds.size(); i++) {
                    ds[i].display();
                }
            }
        }
        else if (chon == 2) {
            cout << "\n--- THEM THIET BI MOI ---\n";
            Device d;
            d.getInformation();
            repo.add(d);
            repo.saveToFile();
            cout << "Them thiet bi thanh cong!\n";
        }
        else if (chon == 3) {
            cout << "\n--- BAN MUON XEM THEO TIEU CHI NAO? ---\n";
            cout << "1. Tim chinh xac theo Ma IMEI\n";
            cout << "2. Loc theo Ten may / Hang SX / Tinh trang (Moi, Cu)\n";
            cout << "0. Quay lai\n";
            int kieu = InputHelper::getInt("Chon kieu xem (0-2): ", 0, 2);

            if (kieu == 1) {
                string ma = InputHelper::getString("Nhap ma IMEI can tim: ");
                Device* p = repo.findById(ma);
                if (p == nullptr) {
                    cout << "Khong tim thay thiet bi!\n";
                } else {
                    p->display();
                }
            } else if (kieu == 2) {
                string tuKhoa = InputHelper::getString("Nhap tu khoa muon xem (VD: iPhone, Apple, Moi): ");
                vector<Device> ds = repo.getAll();
                int dem = 0;
                for (int i = 0; i < (int)ds.size(); i++) {
                    if (ds[i].toString().find(tuKhoa) != string::npos) {
                        ds[i].display();
                        dem++;
                    }
                }
                if (dem == 0) cout << "Khong co thiet bi nao phu hop!\n";
            }
        }
        else if (chon == 4) {
            cout << "\n--- SUA THONG TIN THIET BI ---\n";
            string ma = InputHelper::getString("Nhap ma IMEI can sua: ");
            Device* p = repo.findById(ma);
            if (p == nullptr) {
                cout << "Khong tim thay ma IMEI nay!\n";
            } else {
                cout << "Thong tin cu:\n";
                p->display();
                cout << "\nNhap thong tin moi:\n";
                Device moi;
                moi.setID(ma);
                moi.getInformation();
                repo.update(ma, moi);
                repo.saveToFile();
                cout << "Cap nhat thanh cong!\n";
            }
        }
        else if (chon == 5) {
            cout << "\n--- XOA THIET BI ---\n";
            string ma = InputHelper::getString("Nhap ma IMEI can xoa: ");
            if (repo.findById(ma) == nullptr) {
                cout << "Khong tim thay ma IMEI nay!\n";
            } else {
                repo.remove(ma);
                repo.saveToFile();
                cout << "Da xoa thiet bi thanh cong!\n";
            }
        }
    }
}
