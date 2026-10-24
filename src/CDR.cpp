#include <iostream>
#include <fstream>
#include "CDR.h"
#include "InputHelper.h"
#include "Repository.h"

using namespace std;

// 1. Kiem tra chuoi chi chua chu so (Tra ve 1 neu dung, 0 neu sai)
int laSoCDR(const string& s) {
    if (s == "") return 0;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }
    return 1;
}

// 2. Doi chuoi so sang so nguyen int (thay the stoi de khong bao gio bi loi)
int doiSangSo(const string& s) {
    int kq = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        kq = kq * 10 + (s[i] - '0');
    }
    return kq;
}

// 3. Kiem tra KHOA NGOAI: SDT goi phai co trong file data/sims.txt
int coTrongKhoSim(const string& sdt) {
    ifstream f("data/sims.txt");
    string dong;
    while (getline(f, dong)) {
        if (dong.find(sdt) != string::npos) return 1;
    }
    return 0;
}

// Kiem tra file data/sims.txt co du lieu hay khong
int khoSimCoDuLieu() {
    ifstream f("data/sims.txt");
    string dong;
    while (getline(f, dong)) {
        if (dong.length() >= 10) return 1;
    }
    return 0;
}

// 4. Kiem tra Ma CDR: Bat dau bang "CDR" kem theo so > 0 (VD: CDR01)
int hopLeMaCDR(string& s) {
    if (s.length() < 4 || s.length() > 9) return 0;
    string dau = s.substr(0, 3);
    if (dau == "cdr") {
        s = "CDR" + s.substr(3);
        dau = "CDR";
    }
    if (dau != "CDR") return 0;
    string so = s.substr(3);
    if (laSoCDR(so) == 0) return 0;
    if (doiSangSo(so) <= 0) return 0;
    return 1;
}

// 5. Kiem tra SDT di dong Viet Nam: 10 so, bat dau bang 03, 05, 07, 08, 09
int hopLeSDT(const string& s) {
    if (s.length() != 10 || laSoCDR(s) == 0) return 0;
    if (s[0] != '0') return 0;
    char c = s[1];
    if (c == '3' || c == '5' || c == '7' || c == '8' || c == '9') return 1;
    return 0;
}

// 6. Kiem tra Thoi diem: DD/MM/YYYY HH:MM:SS (Kiem tra nam nhuan & so ngay)
int hopLeThoiDiem(const string& s) {
    if (s.length() != 19) return 0;
    if (s[2] != '/' || s[5] != '/' || s[10] != ' ' || s[13] != ':' || s[16] != ':') return 0;

    string sNgay = s.substr(0, 2), sThang = s.substr(3, 2), sNam = s.substr(6, 4);
    string sGio = s.substr(11, 2), sPhut = s.substr(14, 2), sGiay = s.substr(17, 2);

    if (laSoCDR(sNgay) == 0 || laSoCDR(sThang) == 0 || laSoCDR(sNam) == 0 ||
        laSoCDR(sGio) == 0 || laSoCDR(sPhut) == 0 || laSoCDR(sGiay) == 0) return 0;

    int ngay = doiSangSo(sNgay), thang = doiSangSo(sThang), nam = doiSangSo(sNam);
    int gio = doiSangSo(sGio), phut = doiSangSo(sPhut), giay = doiSangSo(sGiay);

    if (nam < 1980 || nam > 2026 || thang < 1 || thang > 12) return 0;

    int ngayToiDa = 31;
    if (thang == 4 || thang == 6 || thang == 9 || thang == 11) {
        ngayToiDa = 30;
    } else if (thang == 2) {
        int nhuan = 0;
        if (nam % 400 == 0 || (nam % 4 == 0 && nam % 100 != 0)) nhuan = 1;
        if (nhuan == 1) ngayToiDa = 29;
        else ngayToiDa = 28;
    }

    if (ngay < 1 || ngay > ngayToiDa) return 0;
    if (gio < 0 || gio > 23 || phut < 0 || phut > 59 || giay < 0 || giay > 59) return 0;
    return 1;
}

// ============================================================================
// 1. HAM KHOI TAO
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
// 2. QUAN LY MA DINH DANH (ID)
// ============================================================================
string CDR::getID() const {
    return MaCDR;
}

void CDR::setID(const string& value) {
    ID = value;
    MaCDR = value;
}

// ============================================================================
// 3. NHAP THONG TIN CUOC GOI & TU DONG TINH CUOC PHI
// ============================================================================
void CDR::getInformation() {
    // Buoc 1: Nhap va kiem tra trung Ma CDR ngay tai cho
    if (MaCDR == "") {
        Repository<CDR> repoCheck("data/cdrs.txt");
        repoCheck.loadFromFile();
        while (1) {
            MaCDR = InputHelper::getString("Nhap ma CDR (VD: CDR01): ");
            if (hopLeMaCDR(MaCDR) == 0) {
                cout << "Loi: Ma CDR phai bat dau bang 'CDR' kem theo so lon hon 0 (VD: CDR01)!\n";
                continue;
            }
            if (repoCheck.findById(MaCDR) != nullptr) {
                cout << "Loi: Ma CDR [" << MaCDR << "] da ton tai! Vui long nhap ma khac.\n";
                continue;
            }
            break;
        }
        ID = MaCDR;
    }

    // Buoc 2: Nhap SDT goi va kiem tra KHOA NGOAI trong kho SIM ngay lap tuc
    while (1) {
        SoGoi = InputHelper::getString("Nhap so dien thoai goi: ");
        if (hopLeSDT(SoGoi) == 0) {
            cout << "Loi: SDT goi phai co 10 chu so va bat dau bang 03, 05, 07, 08, 09!\n";
            continue;
        }
        if (coTrongKhoSim(SoGoi) == 0) {
            cout << "Loi khoa ngoai: SDT goi [" << SoGoi << "] chua co trong kho SIM (data/sims.txt)!\n";
            continue;
        }
        break;
    }

    // Buoc 3: Nhap SDT nhan
    while (1) {
        SoNhan = InputHelper::getString("Nhap so dien thoai nhan: ");
        if (hopLeSDT(SoNhan) == 0) {
            cout << "Loi: SDT nhan phai co 10 chu so va bat dau bang 03, 05, 07, 08, 09!\n";
            continue;
        }
        if (SoNhan == SoGoi) {
            cout << "Loi: SDT nhan khong duoc trung voi SDT goi!\n";
            continue;
        }
        break;
    }

    // Buoc 4: Nhap thoi diem goi
    while (1) {
        ThoiDiem = InputHelper::getString("Nhap thoi diem goi (VD: 01/01/2024 08:30:15): ");
        if (hopLeThoiDiem(ThoiDiem) == 1) break;
        cout << "Loi: Thoi diem goi sai dinh dang DD/MM/YYYY HH:MM:SS (1980-2026)!\n";
    }

    // Buoc 5: Nhap thoi luong cuoc goi (1 - 86400 giay)
    while (1) {
        SoGiay = InputHelper::getString("Nhap thoi luong cuoc goi (giay): ");
        if (SoGiay.length() <= 5 && laSoCDR(SoGiay) == 1) {
            int giay = doiSangSo(SoGiay);
            if (giay >= 1 && giay <= 86400) break;
        }
        cout << "Loi: Thoi luong cuoc goi phai tu 1 den 86400 giay!\n";
    }

    // Buoc 6: CONG THUC TU DONG TINH CUOC PHI (25 VND/giay = 1500 VND/phut)
    int donGia = 25;
    int thanhTien = doiSangSo(SoGiay) * donGia;
    CuocPhi = to_string(thanhTien);
    cout << "=> Cuoc phi tu dong tinh (" << donGia << " VND/giay): " << CuocPhi << " VND\n";
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
        cout << "2. Them ban ghi cuoc goi moi (Co khoa ngoai)\n";
        cout << "3. Tra cuu cuoc goi theo lua chon\n";
        cout << "4. Sua thong tin cuoc goi\n";
        cout << "5. Xoa ban ghi cuoc goi\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "============================================\n";
        chon = InputHelper::getInt("Nhap lua chon cua ban (0-5): ", 0, 5);

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
        else if (chon == 2) {
            cout << "\n--- THEM CUOC GOI MOI ---\n";
            if (khoSimCoDuLieu() == 0) {
                cout << "Loi khoa ngoai: File data/sims.txt dang trong! Vui long them SIM truoc.\n";
            } else {
                CDR c;
                c.getInformation();
                repo.add(c);
                repo.saveToFile();
                cout << "Them cuoc goi thanh cong va da lien ket khoa ngoai!\n";
            }
        }
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
                string tuKhoa = InputHelper::getString("Nhap SDT hoac Ngay muon xem (VD: 0981000001, 2026-03): ");
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
        else if (chon == 4) {
            cout << "\n--- SUA THONG TIN CUOC GOI ---\n";
            string ma = InputHelper::getString("Nhap ma CDR can sua: ");
            CDR* p = repo.findById(ma);
            if (p == nullptr) {
                cout << "Khong tim thay ma CDR nay!\n";
            } else if (khoSimCoDuLieu() == 0) {
                cout << "Loi khoa ngoai: File data/sims.txt dang trong!\n";
            } else {
                cout << "Thong tin cu:\n";
                p->display();
                cout << "\nNhap thong tin moi:\n";
                CDR moi;
                moi.setID(ma);
                moi.getInformation();
                repo.update(ma, moi);
                repo.saveToFile();
                cout << "Cap nhat thanh cong!\n";
            }
        }
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
