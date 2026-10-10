/**
 * ==============================================================================
 * MODULE BASE - INPUT HELPER (TIỆN ÍCH NHẬP LIỆU AN TOÀN TRÊN CONSOLE)
 * Người phụ trách: Nguyễn Văn Cường (B24DCVT053)
 * Mục đích:
 *   - Xử lý nhập số, nhập chuỗi an toàn, chống trôi lệnh (cin / getline).
 *   - Không bị treo hoặc văng ứng dụng khi người dùng nhập sai kiểu dữ liệu.
 * ==============================================================================
 * 
 */

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
using namespace std;

class InputHelper {
public:
    // ==========================================================================
    // 1. DỌN BỘ NHỚ ĐỆM (CLEAR BUFFER)
    // ==========================================================================
    static void clearBuffer() {
        cin.clear();
        cin.ignore(100000, '\n');
    }

    // ==========================================================================
    // 2. NHẬP SỐ NGUYÊN AN TOÀN (GET INT)
    // ==========================================================================
    static int getInt(const string& CauThongBao, int minVal = numeric_limits<int>::min(), int maxVal = numeric_limits<int>::max()) {
        while (true) {
            cout << CauThongBao;
            int value;
            if (cin >> value) {
                clearBuffer();
                if (value >= minVal && value <= maxVal) {
                    return value;
                }
                cout << "[Loi] Gia tri phai trong khoang [" << minVal << ", " << maxVal << "]! Vui long nhap lai.\n";
            } else {
                cout << "[Loi] Vui long chi nhap so nguyen hop le!\n";
                clearBuffer();
            }
        }
    }

    // ==========================================================================
    // 3. NHẬP SỐ THỰC AN TOÀN (GET DOUBLE)
    // ==========================================================================
    static double getDouble(const string& CauThongBao, double minVal = 0.0, double maxVal = numeric_limits<double>::max()) {
        while (true) {
            cout << CauThongBao;
            double value;
            if (cin >> value) {
                clearBuffer();
                if (value >= minVal && value <= maxVal) {
                    return value;
                }
                cout << "[Loi] Gia tri phai lon hon hoac bang " << minVal << "! Vui long nhap lai.\n";
            } else {
                cout << "[Loi] Vui long chi nhap so thuc hop le!\n";
                clearBuffer();
            }
        }
    }

    // ==========================================================================
    // 4. NHẬP CHUỖI AN TOÀN - CHỐNG TRÔI LỆNH (GET STRING)
    // ==========================================================================
    static string getString(const string& CauThongBao, bool allowEmpty = false) {
        while (true) {
            cout << CauThongBao;
            string s;
            getline(cin, s);
            if (s.empty() && !allowEmpty) {
                cout << "[Loi] Du lieu khong duoc de trong! Vui long nhap lai.\n";
                continue;
            }
            return s;
        }
    }

    // ==========================================================================
    // 5. NHẬP XÁC NHẬN CÓ / KHÔNG (GET CONFIRM Y/N)
    // ==========================================================================
    static bool getConfirm(const string& CauThongBao) {
        cout << CauThongBao;
        while (true) {
            string s;
            getline(cin, s);
            if (!s.empty()) {
                char confirm = s[0];
                if (confirm == 'Y' || confirm == 'y') return true;
                if (confirm == 'N' || confirm == 'n') return false;
            }
            cout << "Vui long chi nhap (y/n): ";
        }
    }

    // ==========================================================================
    // 6. TIỆN ÍCH HỖ TRỢ CHỨC NĂNG SỬA (UPDATE - GIỮ NGUYÊN GIÁ TRỊ CŨ KHI BỎ TRỐNG)
    // ==========================================================================
    static string getStringOrDefault(const string& CauThongBao, const string& oldValue) {
        cout << CauThongBao << "(Hien tai: " << oldValue << "): ";
        string newstring;
        getline(cin, newstring);
        if (newstring.empty()) return oldValue;
        return newstring;
    }

    static double getDoubleOrDefault(const string& CauThongBao, double oldValue) {
        while (true) {
            cout << CauThongBao << "(Hien tai: " << oldValue << "): ";
            string newstring;
            getline(cin, newstring);
            if (newstring.empty()) return oldValue;

            stringstream ss(newstring);
            double newvalue;
            char extra;
            if (ss >> newvalue && !(ss >> extra)) {
                if (newvalue >= 0.0) {
                    return newvalue;
                } else {
                    cout << "[Loi] Gia tri khong duoc am! Vui long nhap lai.\n";
                }
            } else {
                cout << "[Loi] Vui long nhap so thuc hop le hoac nhan Enter de giu nguyen!\n";
            }
        }
    }

    static int getIntOrDefault(const string& CauThongBao, int oldValue) {
        while (true) {
            cout << CauThongBao << "(Hien tai: " << oldValue << "): ";
            string newstring;
            getline(cin, newstring);
            if (newstring.empty()) return oldValue;

            stringstream ss(newstring);
            int newvalue;
            char extra;
            if (ss >> newvalue && !(ss >> extra)) {
                return newvalue;
            } else {
                cout << "[Loi] Vui long nhap so nguyen hop le hoac nhan Enter de giu nguyen!\n";
            }
        }
    }
};
