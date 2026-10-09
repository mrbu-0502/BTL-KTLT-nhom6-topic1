#pragma once
#include <iostream>
#include <string>
#include <sstream>

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
class date {
private:
    int x, y, m;

public:
    date(int x = 1, int y = 1, int m = 2000) : x(x), y(y), m(m) {}

    // ============================================================================
    // 1. KIEM TRA NAM NHUAN
    // ============================================================================
    bool leap() {
        return m % 400 == 0 || (m % 4 == 0 && m % 100 != 0);
    }

    // ============================================================================
    // 2. KIEM TRA NGAY HOP LE (CHAN NAM LON HON 2026)
    // ============================================================================
    bool valid() {
        if (m < 1 || m > 2026 || y < 1 || y > 12 || x < 1) return false;
        int n[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (leap()) n[2] = 29;
        return x <= n[y];
    }

    // ============================================================================
    // 3. NHAP NGAY THANG AN TOAN
    // ============================================================================
    void input(string p) {
        while (true) {
            cout << p;
            string a;
            getline(cin, a);
            char c1 = 0, c2 = 0;
            stringstream ss(a);
            ss >> x >> c1 >> y >> c2 >> m;
            
            if (c1 == '/' && c2 == '/' && valid()) {
                break; 
            }
            cout << "sai dinh dang, hay nhap lai\n";
        }
    }

    // ============================================================================
    // 4. XUAT CHUOI DINH DANG DD/MM/YYYY
    // ============================================================================
    string output() {
        string s1 = (x < 10 ? "0" : "") + to_string(x);
        string s2 = (y < 10 ? "0" : "") + to_string(y);
        return s1 + "/" + s2 + "/" + to_string(m);
    }
};
