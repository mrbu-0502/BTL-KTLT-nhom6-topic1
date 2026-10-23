#include "CDR.h"      
#include "InputHelper.h" 
#include "Repository.h"
#include "Sim.h"

using namespace std;

// Cac ham kiem tra dinh dang (Validation)
static bool hopLeMaCDR(const string& s) {
    if (s.length() < 4) return false;
    if (s.substr(0, 3) != "CDR") return false;
    for (size_t i = 3; i < s.length(); i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

static bool hopLeSDT(const string& s) {
    if (s.length() != 10 || s[0] != '0') return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

static bool hopLeThoiDiem(const string& s) {
    // Dinh dang chuan: DD/MM/YYYY HH:MM:SS (dung 19 ky tu)
    if (s.length() != 19) return false;
    if (s[2] != '/' || s[5] != '/' || s[10] != ' ' || s[13] != ':' || s[16] != ':') return false;
    for (int i = 0; i < 19; i++) {
        if (i == 2 || i == 5 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(s[i])) return false;
    }
    int ngay = stoi(s.substr(0, 2));
    int thang = stoi(s.substr(3, 2));
    int nam = stoi(s.substr(6, 4));
    int gio = stoi(s.substr(11, 2));
    int phut = stoi(s.substr(14, 2));
    int giay = stoi(s.substr(17, 2));

    if (nam < 2000 || nam > 2100) return false;
    if (thang < 1 || thang > 12) return false;
    if (ngay < 1 || ngay > 31) return false;
    if (gio < 0 || gio > 23 || phut < 0 || phut > 59 || giay < 0 || giay > 59) return false;
    return true;
}

static bool hopLeSoDuong(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    try {
        return stoll(s) > 0;
    } catch (...) {
        return false;
    }
}

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
// 3. NHAP 6 THONG TIN CUOC GOI TU BAN PHIM (Chan loi ngoai le & khoa ngoai)
// ============================================================================
void CDR::getInformation() {
    Repository<Sim> simRepo("data/sims.txt");
    simRepo.loadFromFile();

    // 1. Kiem tra Ma CDR (Bat buoc dang CDR + so, VD: CDR01)
    while (true) {
        MaCDR = InputHelper::getString("Nhap ma CDR (VD: CDR01): ");
        if (!hopLeMaCDR(MaCDR)) {
            cout << "Loi: Ma CDR phai bat dau bang 'CDR' theo sau la cac chu so (VD: CDR01)!\n";
            continue;
        }
        break;
    }
    ID = MaCDR;

    // 2. Kiem tra So dien thoai goi (Dung 10 so, bat dau bang 0 va ton tai trong kho SIM)
    while (true) {
        SoGoi = InputHelper::getString("Nhap so dien thoai goi (10 chu so, bat dau bang 0): ");
        if (!hopLeSDT(SoGoi)) {
            cout << "Loi: So dien thoai phai co dung 10 chu so va bat dau bang so 0!\n";
            continue;
        }
        if (simRepo.findById(SoGoi) == nullptr) {
            cout << "Loi khoa ngoai: So goi [" << SoGoi << "] khong ton tai trong danh sach SIM cua he thong!\n";
            continue;
        }
        break;
    }

    // 3. Kiem tra So dien thoai nhan (Dung 10 so, bat dau bang 0, khac SoGoi)
    while (true) {
        SoNhan = InputHelper::getString("Nhap so dien thoai nhan (10 chu so, bat dau bang 0): ");
        if (!hopLeSDT(SoNhan)) {
            cout << "Loi: So dien thoai nhan phai co dung 10 chu so va bat dau bang so 0!\n";
            continue;
        }
        if (SoNhan == SoGoi) {
            cout << "Loi: So dien thoai nhan khong duoc trung voi so dien thoai goi!\n";
            continue;
        }
        break;
    }

    // 4. Kiem tra Thoi diem goi (Dung dinh dang DD/MM/YYYY HH:MM:SS)
    while (true) {
        ThoiDiem = InputHelper::getString("Nhap thoi diem goi (DD/MM/YYYY HH:MM:SS, VD: 01/01/2026 08:30:15): ");
        if (!hopLeThoiDiem(ThoiDiem)) {
            cout << "Loi: Thoi diem goi sai dinh dang hoac ngay gio khong hop le!\n";
            continue;
        }
        break;
    }

    // 5. Kiem tra Thoi luong cuoc goi (So nguyen duong > 0)
    while (true) {
        SoGiay = InputHelper::getString("Nhap thoi luong cuoc goi (giay > 0): ");
        if (!hopLeSoDuong(SoGiay)) {
            cout << "Loi: Thoi luong phai la so nguyen duong lon hon 0!\n";
            continue;
        }
        break;
    }

    // 6. Kiem tra Cuoc phi (So >= 0)
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
            cout << "Loi: Cuoc phi phai la gia tri so hop le (>= 0)! Vui long nhap lai.\n";
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
// 7. MENU QUAN LY CUOC GOI
// ============================================================================
void menuCuocGoi() {
    Repository<CDR> repo("data/cdrs.txt");
    repo.loadFromFile();

    int chon = -1;
    while (chon != 0) {
        cout << "\n========== QUAN LY CUOC GOI (CDR) ==========\n";
        cout << "1. Xem toan bo danh sach cuoc goi\n";
        cout << "2. Them ban ghi cuoc goi moi (Kiem tra khoa ngoai SIM)\n";
        cout << "3. Tra cuu cuoc goi theo lua chon\n";
        cout << "4. Sua thong tin cuoc goi (Kiem tra khoa ngoai SIM)\n";
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
        // Chuc nang 2: Them moi
        else if (chon == 2) {
            cout << "\n--- THEM CUOC GOI MOI ---\n";
            CDR c;
            c.getInformation();
            if (repo.findById(c.getID()) != nullptr) {
                cout << "Loi: Ma CDR nay da ton tai trong he thong!\n";
            } else {
                repo.add(c);
                repo.saveToFile();
                cout << "Them cuoc goi thanh cong (da lien ket khoa ngoai SIM)!\n";
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
                string tuKhoa = InputHelper::getString("Nhap SDT hoac Ngay muon xem (VD: 0981000001, 01/01/2026): ");
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
                cout << "\nNhap thong tin moi (Nhan Enter de giu nguyen thong tin cu):\n";
                
                string soGoiMoi = InputHelper::getStringOrDefault("So dien thoai goi", p->getSoGoi());
                Repository<Sim> simRepo("data/sims.txt");
                simRepo.loadFromFile();
                if (simRepo.findById(soGoiMoi) == nullptr) {
                    cout << "Loi khoa ngoai: So dien thoai goi [" << soGoiMoi << "] khong ton tai trong danh sach SIM!\n";
                    cout << "Cap nhat that bai!\n";
                } else {
                    string soNhanMoi = InputHelper::getStringOrDefault("So dien thoai nhan", p->getSoNhan());
                    string thoiDiemMoi = InputHelper::getStringOrDefault("Thoi diem goi", p->getThoiDiem());
                    string soGiayMoi = InputHelper::getStringOrDefault("Thoi luong giay", p->getSoGiay());
                    string cuocPhiMoi = InputHelper::getStringOrDefault("Cuoc phi", p->getCuocPhi());

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
