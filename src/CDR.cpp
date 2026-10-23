#include "CDR.h"      
#include "InputHelper.h" 
#include "Repository.h"
#include "Sim.h" 

using namespace std;

// ============================================================================
// 1. HAM KHOI TAO: Gan tat ca 6 thuoc tinh ban dau bang rong ""
// ============================================================================
CDR::CDR() {
    MaCDR = "";
    SoGoi = "";
    SoNhan = "";
    ThoiDiem = "";
    SoGiay = "";
    CuocPhi = "";
}

// ============================================================================
// 2. QUAN LY MA DINH DANH (ID) VA GETTER / SETTER KHOA NGOAI
// ============================================================================
string CDR::getID() const {
    return MaCDR;
}

void CDR::setID(const string& value) {
    ID = value;
    MaCDR = value;
}

string CDR::getMaCDR() const { return MaCDR; }
string CDR::getSoGoi() const { return SoGoi; }
string CDR::getSoNhan() const { return SoNhan; }
string CDR::getThoiDiem() const { return ThoiDiem; }
string CDR::getSoGiay() const { return SoGiay; }
string CDR::getCuocPhi() const { return CuocPhi; }

void CDR::setMaCDR(const string& value) { MaCDR = value; ID = value; }
void CDR::setSoGoi(const string& value) { SoGoi = value; }
void CDR::setSoNhan(const string& value) { SoNhan = value; }
void CDR::setThoiDiem(const string& value) { ThoiDiem = value; }
void CDR::setSoGiay(const string& value) { SoGiay = value; }
void CDR::setCuocPhi(const string& value) { CuocPhi = value; }

// ============================================================================
// 3. NHAP 6 THONG TIN CUOC GOI TU BAN PHIM (Co kiem tra ngoai le & chuan dinh dang)
// ============================================================================
void CDR::getInformation() {
    // 1. Nhap ma CDR (Khong duoc de trong va khong chua dau '|')
    while (true) {
        MaCDR = InputHelper::getString("Nhap ma CDR (VD: CDR01): ");
        if (MaCDR.empty() || MaCDR.find('|') != string::npos) {
            cout << "Loi: Ma CDR khong duoc de trong va khong duoc chua ky tu '|'! Vui long nhap lai.\n";
            continue;
        }
        break;
    }
    ID = MaCDR;

    // 2. Kiem tra so dien thoai goi (10-11 chu so, hoan toan la so)
    while (true) {
        SoGoi = InputHelper::getString("Nhap so dien thoai goi (10-11 chu so): ");
        if (SoGoi.length() < 10 || SoGoi.length() > 11) {
            cout << "Loi: So dien thoai phai co tu 10 den 11 chu so! Vui long nhap lai.\n";
            continue;
        }
        bool hopLe = true;
        for (char c : SoGoi) {
            if (!isdigit(c)) {
                hopLe = false;
                break;
            }
        }
        if (!hopLe) {
            cout << "Loi: So dien thoai chi duoc chua cac chu so (0-9)! Vui long nhap lai.\n";
            continue;
        }
        break;
    }

    // 3. Kiem tra so dien thoai nhan (10-11 chu so, hoan toan la so)
    while (true) {
        SoNhan = InputHelper::getString("Nhap so dien thoai nhan (10-11 chu so): ");
        if (SoNhan.length() < 10 || SoNhan.length() > 11) {
            cout << "Loi: So dien thoai phai co tu 10 den 11 chu so! Vui long nhap lai.\n";
            continue;
        }
        bool hopLe = true;
        for (char c : SoNhan) {
            if (!isdigit(c)) {
                hopLe = false;
                break;
            }
        }
        if (!hopLe) {
            cout << "Loi: So dien thoai chi duoc chua cac chu so (0-9)! Vui long nhap lai.\n";
            continue;
        }
        break;
    }

    // 4. Nhap thoi diem goi
    ThoiDiem = InputHelper::getString("Nhap thoi diem goi (VD: 01/01/2026 08:30:15): ");

    // 5. Kiem tra thoi luong cuoc goi (Phai la so nguyen)
    while (true) {
        SoGiay = InputHelper::getString("Nhap thoi luong cuoc goi (giay): ");
        bool hopLe = true;
        if (SoGiay.empty()) {
            hopLe = false;
        } else {
            for (char c : SoGiay) {
                if (!isdigit(c)) {
                    hopLe = false;
                    break;
                }
            }
        }
        if (!hopLe) {
            cout << "Loi: Thoi luong giay phai la mot so nguyen hop le! Vui long nhap lai.\n";
            continue;
        }
        break;
    }

    // 6. Kiem tra cuoc phi (Phai la so)
    while (true) {
        CuocPhi = InputHelper::getString("Nhap cuoc phi (VND): ");
        bool hopLe = true;
        if (CuocPhi.empty()) {
            hopLe = false;
        } else {
            for (char c : CuocPhi) {
                if (!isdigit(c) && c != '.') {
                    hopLe = false;
                    break;
                }
            }
        }
        if (!hopLe) {
            cout << "Loi: Cuoc phi phai la gia tri so hop le! Vui long nhap lai.\n";
            continue;
        }
        break;
    }
}

// ============================================================================
// 4. HIEN THI BAN GHI CUOC GOI RA MAN HINH
// ============================================================================
void CDR::display() const {
    cout << "Ma CDR: " << MaCDR 
         << " | So goi: " << SoGoi 
         << " | So nhan: " << SoNhan 
         << " | Thoi diem: " << ThoiDiem 
         << " | Thoi luong: " << SoGiay << "s"
         << " | Cuoc phi: " << CuocPhi << " VND" << endl;
}

// ============================================================================
// 5. GHEP 6 BIEN THANH 1 DONG CHUOI DE GHI FILE data/cdrs.txt
// ============================================================================
string CDR::toString() const {
    return MaCDR + "|" + SoGoi + "|" + SoNhan + "|" + ThoiDiem + "|" + SoGiay + "|" + CuocPhi;
}

// ============================================================================
// 6. TACH CHUOI 6 KHUC TU FILE data/cdrs.txt
// ============================================================================
void CDR::fromString(const string& line) {
    int p1 = line.find('|');
    MaCDR = line.substr(0, p1);
    
    int p2 = line.find('|', p1 + 1);
    SoGoi = line.substr(p1 + 1, p2 - p1 - 1);
    
    int p3 = line.find('|', p2 + 1);
    SoNhan = line.substr(p2 + 1, p3 - p2 - 1);
    
    int p4 = line.find('|', p3 + 1);
    ThoiDiem = line.substr(p3 + 1, p4 - p3 - 1);
    
    int p5 = line.find('|', p4 + 1);
    SoGiay = line.substr(p4 + 1, p5 - p4 - 1);
    
    CuocPhi = line.substr(p5 + 1);
    ID = MaCDR; 
}

// ============================================================================
// 7. MENU QUAN LY CUOC GOI (Co kiem tra khoa ngoai SoGoi)
// ============================================================================
void menuCuocGoi() {
    Repository<CDR> repo("data/cdrs.txt");
    repo.loadFromFile(); // Doc du lieu tu file cdrs.txt len

    int chon = -1;
    while (chon != 0) {
        cout << "\n========== QUAN LY CUOC GOI (CDR) ==========\n";
        cout << "1. Xem toan bo danh sach cuoc goi\n";
        cout << "2. Them ban ghi cuoc goi moi (Co kiem tra khoa ngoai)\n";
        cout << "3. Tra cuu cuoc goi theo lua chon\n";
        cout << "4. Sua thong tin cuoc goi (Co kiem tra khoa ngoai)\n";
        cout << "5. Xoa ban ghi cuoc goi\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "============================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

        // Chuc nang 1: Xem tat ca
        if (chon == 1) {
            vector<CDR> ds = repo.getAll();
            if (ds.empty()) {
                cout << "Danh sach cuoc goi dang trong!\n";
            } else {
                cout << "\n--- DANH SACH CUOC GOI ---\n";
                for (int i = 0; i < (int)ds.size(); i++) {
                    ds[i].display();
                }
            }
        }
        // Chuc nang 2: Them moi (Co kiem tra khoa ngoai SIM)
        else if (chon == 2) {
            cout << "\n--- THEM CUOC GOI MOI ---\n";
            CDR c;
            c.getInformation();

            // 1. Kiem tra xem ma CDR da ton tai chua
            if (repo.findById(c.getID()) != nullptr) {
                cout << "Loi: Ma CDR nay da ton tai!\n";
            } else {
                // 2. Kiem tra khoa ngoai: SoGoi phai ton tai trong he thong SIM
                Repository<Sim> simRepo("data/sims.txt");
                simRepo.loadFromFile();

                if (simRepo.findById(c.getSoGoi()) == nullptr) {
                    cout << "Loi khoa ngoai: So dien thoai goi [" << c.getSoGoi() << "] chua ton tai trong kho SIM!\n";
                    cout << "Them cuoc goi that bai. Vui long them so SIM nay truoc.\n";
                } else {
                    repo.add(c);
                    repo.saveToFile();
                    cout << "Them cuoc goi thanh cong va da lien ket khoa ngoai SoGoi hop le!\n";
                }
            }
        }
        // Chuc nang 3: Tra cuu
        else if (chon == 3) {
            cout << "\n--- BAN MUON TRA CUU THEO TIEU CHI NAO? ---\n";
            cout << "1. Tim chinh xac theo Ma CDR\n";
            cout << "2. Tra cuu theo So dien thoai hoac Ngay goi\n";
            cout << "0. Quay lai\n";
            int kieu = InputHelper::getInt("Chon kieu tra cuu (0-2): ", 0, 2);

            if (kieu == 1) {
                string ma = InputHelper::getString("Nhap ma CDR can tim: ");
                CDR* p = repo.findById(ma);
                if (p == nullptr) {
                    cout << "Khong tim thay ban ghi cuoc goi!\n";
                } else {
                    p->display();
                }
            } else if (kieu == 2) {
                string tuKhoa = InputHelper::getString("Nhap SDT hoac Ngay muon xem (VD: 0981000001, 2024): ");
                vector<CDR> ds = repo.getAll();
                int dem = 0;
                for (int i = 0; i < (int)ds.size(); i++) {
                    if (ds[i].toString().find(tuKhoa) != string::npos) {
                        ds[i].display();
                        dem++;
                    }
                }
                if (dem == 0) cout << "Khong co cuoc goi nao phu hop!\n";
            }
        }
        // Chuc nang 4: Sua thong tin
        else if (chon == 4) {
            cout << "\n--- SUA THONG TIN CUOC GOI ---\n";
            string ma = InputHelper::getString("Nhap ma CDR can sua: ");
            CDR* p = repo.findById(ma);
            if (p == nullptr) {
                cout << "Khong tim thay ma CDR nay!\n";
            } else {
                cout << "Thong tin cu:\n";
                p->display();
                cout << "\nNhap thong tin moi (Nhan Enter de giu nguyen):\n";
                
                string soGoiMoi = InputHelper::getStringOrDefault("So dien thoai goi: ", p->getSoGoi());
                // Kiem tra khoa ngoai cho so goi moi
                Repository<Sim> simRepo("data/sims.txt");
                simRepo.loadFromFile();
                if (simRepo.findById(soGoiMoi) == nullptr) {
                    cout << "Loi khoa ngoai: So dien thoai goi moi [" << soGoiMoi << "] chua ton tai trong he thong SIM!\n";
                    cout << "Cap nhat that bai!\n";
                } else {
                    string soNhanMoi = InputHelper::getStringOrDefault("So dien thoai nhan: ", p->getSoNhan());
                    string thoiDiemMoi = InputHelper::getStringOrDefault("Thoi diem goi: ", p->getThoiDiem());
                    string soGiayMoi = InputHelper::getStringOrDefault("Thoi luong giay: ", p->getSoGiay());
                    string cuocPhiMoi = InputHelper::getStringOrDefault("Cuoc phi: ", p->getCuocPhi());

                    CDR moi;
                    moi.setID(ma);
                    moi.setMaCDR(ma);
                    moi.setSoGoi(soGoiMoi);
                    moi.setSoNhan(soNhanMoi);
                    moi.setThoiDiem(thoiDiemMoi);
                    moi.setSoGiay(soGiayMoi);
                    moi.setCuocPhi(cuocPhiMoi);

                    repo.update(ma, moi);
                    repo.saveToFile();
                    cout << "Cap nhat cuoc goi thanh cong!\n";
                }
            }
        }
        // Chuc nang 5: Xoa
        else if (chon == 5) {
            cout << "\n--- XOA CUOC GOI ---\n";
            string ma = InputHelper::getString("Nhap ma CDR can xoa: ");
            if (repo.findById(ma) == nullptr) {
                cout << "Khong tim thay ma CDR nay!\n";
            } else {
                repo.remove(ma);
                repo.saveToFile();
                cout << "Da xoa cuoc goi thanh cong!\n";
            }
        }
    }
}
