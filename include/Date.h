#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h> 

using namespace std;

/**
 * ============================================================================
 * MODULE BASE - DATE HELPER (TI?N ÍCH X? LÝ VÀ CHU?N HÓA NGÀY THÁNG)
 * Ngu?i ph? trách: Ki?u Ð?c Hi?p
 * M?c dích:
 *   - X? lý, ki?m tra nam nhu?n và tính h?p l? c?a ngày, tháng, nam.
 *   - Nh?p chu?i ngày tháng an toàn t? bàn phím, ch?ng l?i trôi l?nh.
 *   - T? d?ng chu?n hóa d? li?u d?u ra thành d?nh d?ng chung DD/MM/YYYY.
 * ============================================================================
 */
class date {
private:
    int x, y, m;

public:
    date(int x = 1, int y = 1, int m = 2000) : x(x), y(y), m(m) {}

    // ============================================================================
    // 1. KI?M TRA NAM NHU?N
    // ============================================================================
    bool leap() {
        return m % 400 == 0 || (m % 4 == 0 && m % 100 != 0);
    }

    // ============================================================================
    // 2. KI?M TRA NGÀY H?P L? (CH?N NAM L?N HON 2026)
    // ============================================================================
    bool valid() {
        if (m < 1 || m > 2026 || y < 1 || y > 12 || x < 1) return false;
        int n[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (leap()) n[2] = 29;
        return x <= n[y];
    }

    // ============================================================================
    // 3. NH?P NGÀY THÁNG AN TOÀN
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
    // 4. XU?T CHU?I Ð?NH D?NG DD/MM/YYYY
    // ============================================================================
    string output() {
        string s1 = (x < 10 ? "0" : "") + to_string(x);
        string s2 = (y < 10 ? "0" : "") + to_string(y);
        return s1 + "/" + s2 + "/" + to_string(m);
    }
};

#endif