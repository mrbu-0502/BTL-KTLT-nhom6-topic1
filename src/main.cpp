#include <iostream>
#include <limits>

using namespace std;

// Khai báo trước các hàm menu con của 10 use case
// Sau này khi từng bạn làm xong thì chỉ cần nối code vào đây
void menuGoiCuoc() {
    cout << "\n--- [USE CASE 1: QUAN LY GOI CUOC] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuKhachHang() {
    cout << "\n--- [USE CASE 2: QUAN LY KHACH HANG] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuLoaiHinh() {
    cout << "\n--- [USE CASE 3: QUAN LY LOAI HINH THUE BAO] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuKhieuNai() {
    cout << "\n--- [USE CASE 4: QUAN LY KHIEU NAI] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuThietBi() {
    cout << "\n--- [USE CASE 5: QUAN LY THIET BI] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuCuocGoi() {
    cout << "\n--- [USE CASE 6: QUAN LY CUOC GOI (CDR)] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuSim() {
    cout << "\n--- [USE CASE 7: QUAN LY SIM] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuHoaDon() {
    cout << "\n--- [USE CASE 8: QUAN LY HOA DON] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuNapTien() {
    cout << "\n--- [USE CASE 9: QUAN LY NAP TIEN] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void menuHopDong() {
    cout << "\n--- [USE CASE 10: QUAN LY HOP DONG] ---\n";
    cout << "(Chuc nang dang duoc hoan thien...)\n";
}

void hienThiMenuChinh() {
    cout << "\n======================================================\n";
    cout << "   HE THONG QUAN LY THUE BAO DI DONG - NHOM 6        \n";
    cout << "======================================================\n";
    cout << "1.  Quan ly Goi cuoc (Package)\n";
    cout << "2.  Quan ly Khach hang (Customer)\n";
    cout << "3.  Quan ly Loai hinh thue bao (SubType)\n";
    cout << "4.  Quan ly Khieu nai (Complaint)\n";
    cout << "5.  Quan ly Thiet bi (Device)\n";
    cout << "6.  Quan ly Cuoc goi (CDR)\n";
    cout << "7.  Quan ly SIM (Sim)\n";
    cout << "8.  Quan ly Hoa don (Bill)\n";
    cout << "9.  Quan ly Nap tien (Recharge)\n";
    cout << "10. Quan ly Hop dong (Contract)\n";
    cout << "0.  Thoat chuong trinh\n";
    cout << "======================================================\n";
    cout << "Nhap lua chon cua ban (0-10): ";
}

int main() {
    int luaChon = -1;

    do {
        hienThiMenuChinh();
        if (!(cin >> luaChon)) {
            cout << "\n[Loi] Vui long chi nhap so nguyen hop le!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (luaChon) {
            case 1:  menuGoiCuoc();  break;
            case 2:  menuKhachHang(); break;
            case 3:  menuLoaiHinh();  break;
            case 4:  menuKhieuNai();  break;
            case 5:  menuThietBi();   break;
            case 6:  menuCuocGoi();   break;
            case 7:  menuSim();       break;
            case 8:  menuHoaDon();    break;
            case 9:  menuNapTien();   break;
            case 10: menuHopDong();   break;
            case 0:
                cout << "\nCam on ban da su dung chuong trinh. Tam biet!\n";
                break;
            default:
                cout << "\n[Loi] Lua chon khong hop le. Vui long chon tu 0 den 10.\n";
                break;
        }
    } while (luaChon != 0);

    return 0;
}