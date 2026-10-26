/**
 * ==============================================================================
 * MODULE BASE - Date.h 
 * Người phụ trách: Kiều Đức Hiệp
 * ==============================================================================
 * 
 */
#pragma once 
#include "Exceptions.h"
#include <iostream>
#include <string>
#include <sstream> 
#include "Exceptions.h"
using namespace std;

/**
 * ============================================================================
 * MODULE BASE - DATE (TIEN ICH XU LY VA CHUAN HOA NGAY THANG)
 * Nguoi phu trach: Kieu Duc Hiep
 * Muc dich:
 *   - Xu ly, kiem tra nam nhuan va tinh hop le cua ngay, thang, nam.
 *   - Nhap chuoi ngay thang an toan tu ban phim, chong loi troi lenh.
 *   - Tu dong chuan hoa du lieu dau ra thanh dinh dang chung DD/MM/YYYY.
 * ============================================================================
 */
class Date {
private:
    // Khai bao 3 thuoc tinh ro rang trong vung bao mat (private)
    int Ngay;   // Luu ngay (1 -> 31)
    int Thang;  // Luu thang (1 -> 12)
    int Nam;    // Luu nam (1 -> 2026)

public:
    // ========================================================================
    // 1. HAM KHOI TAO (CONSTRUCTOR)
    // Neu khong truyen gi vao, mac dinh se tao ra ngay 01/01/2000
    // ========================================================================
    Date(int d = 1, int m = 1, int y = 2000) {
        Ngay = d;
        Thang = m;
        Nam = y;
    }

    // ========================================================================
    // 2. KIEM TRA NAM NHUAN (Tra ve 1 neu la nam nhuan, 0 neu khong phai)
    // Nam nhuan la nam chia het cho 400, HOAC (chia het cho 4 nhung khong chia het cho 100)
    // ========================================================================
    int leap() {
        if (Nam % 400 == 0 || (Nam % 4 == 0 && Nam % 100 != 0)) {
            return 1; // 1 nghia la Dung (nam nhuan)
        }
        return 0; // 0 nghia la Sai (nam thuong)
    }

    // ========================================================================
    // 3. KIEM TRA NGAY THANG NAM HOP LE (Tra ve 1 neu hop le, 0 neu sai)
    // ========================================================================
    int valid() {
        // Neu nam ngoai khoang (1 -> 2026), thang ngoai (1 -> 12), hoac ngay < 1 thi bao SAI (0)
        if (Nam < 1 || Nam > 2026 || Thang < 1 || Thang > 12 || Ngay < 1) {
            return 0;
        }
        
        // Bang tra cuu so ngay toi da cua 12 thang (Bo qua vi tri so 0 cho de dem tu thang 1 -> 12)
        int soNgay[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Neu la nam nhuan (leap() == 1) thi thang 2 (vi tri so 2) se co 29 ngay
        if (leap()) {
            soNgay[2] = 29;
        }
        
        // Neu Ngay <= so ngay toi da cua thang thi tra ve 1 (Dung), nguoc lai tra ve 0 (Sai)
        if (Ngay <= soNgay[Thang]) {
            return 1;
        }
        return 0;
    }

    // ========================================================================
    // 4. NHAP NGAY THANG AN TOAN TU BAN PHIM (BAT NHAP LAI NEU SAI)
    // ========================================================================
void input(string sm) {
    while (true) {
        int d, m, y;
        char c1, c2;
        stringstream ss(sm);
        if (!(ss >> d >> c1 >> m >> c2 >> y) || c1 != '/' || c2 != '/') {
            throw InvalidDataException("Ngay phai co dang DD/MM/YYYY!");
        }
        ss >> ws;
        if (!ss.eof()) {
            throw InvalidDataException("Ngay khong duoc co ky tu thua!");
        }
        Date temp(d, m, y);
        if (!temp.valid()) {
            throw InvalidDataException("Ngay thang nam khong hop le!");
        }
        Ngay = d;
        Thang = m;
        Nam = y;
        return; 
    }
}
    // ========================================================================
    // 5. XUAT RA CHUOI CHUAN DINH DANG DD/MM/YYYY
    // Neu ngay hoac thang nho hon 10 thi tu dong cong them so "0" o dang truoc (VD: 5 -> "05")
    // ========================================================================
    string output() {
        string s1 = (Ngay < 10 ? "0" : "") + to_string(Ngay);
        string s2 = (Thang < 10 ? "0" : "") + to_string(Thang);
        return s1 + "/" + s2 + "/" + to_string(Nam);
    }

    // ========================================================================
    // 6. SO SANH 2 NGAY (Tra ve -1 neu TRUOC, 0 neu BANG, 1 neu SAU)
    // ========================================================================
    int compare(Date b) {
        // B1: So sanh Nam truoc
        if (Nam < b.Nam) return -1;
        if (Nam > b.Nam) return 1;

        // B2: Neu cung Nam thi so sanh Thang
        if (Thang < b.Thang) return -1;
        if (Thang > b.Thang) return 1;

        // B3: Neu cung Thang thi so sanh Ngay
        if (Ngay < b.Ngay) return -1;
        if (Ngay > b.Ngay) return 1;

        // B4: Neu Ngay, Thang, Nam deu bang nhau
        return 0;
    }

    // ========================================================================
    // 7. NHAP NGAY THANG TU CHUOI STRING CO SAN 
    // ========================================================================
    void input_string(const string& s) {
        stringstream ss(s);
        char c1, c2;
        ss >> Ngay >> c1 >> Thang >> c2 >> Nam;
    }
};