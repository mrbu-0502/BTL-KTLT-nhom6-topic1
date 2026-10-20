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
// 3. NHAP DU LIEU TU BAN PHIM
// ============================================================================
void Device::getInformation() {
    MaIMEI = InputHelper::getString("Nhap ma IMEI: ");
    ID = MaIMEI;

    TenMay = InputHelper::getString("Nhap ten may: ");
    HangSX = InputHelper::getString("Nhap hang san xuat: ");
    NamSX = InputHelper::getString("Nhap nam san xuat: ");
    TinhTrang = InputHelper::getString("Nhap tinh trang (Moi/Cu): ");
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
    repo.loadFromFile(); // Doc du lieu tu file devices.txt len

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

        // Chuc nang 1: Xem tat ca
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
        // Chuc nang 2: Them moi
        else if (chon == 2) {
            cout << "\n--- THEM THIET BI MOI ---\n";
            Device d;
            d.getInformation();
            if (repo.findById(d.getID()) != nullptr) {
                cout << "Loi: Ma IMEI nay da ton tai!\n";
            } else {
                repo.add(d);
                repo.saveToFile();
                cout << "Them thiet bi thanh cong!\n";
            }
        }
        // Chuc nang 3: Menu con cho khach hang chon kieu xem / tim kiem
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
        // Chuc nang 4: Sua thong tin
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
                moi.getInformation();
                moi.setID(ma); // Giu nguyen ma IMEI cu
                repo.update(moi);
                repo.saveToFile();
                cout << "Cap nhat thanh cong!\n";
            }
        }
        // Chuc nang 5: Xoa
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

