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
#include<cctype>
#include <sstream>
using namespace std;

class InputHelper {
public:

    static void clearBuffer() {
        cin.clear();
        cin.ignore(100000, '\n');
    }


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


    static long long getLongLong(const string& CauThongBao, long long minVal = 0, long long maxVal = numeric_limits<long long>::max()) {
        while (true) {
            cout << CauThongBao;
            long long value;
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
    // NHẬP XÁC NHẬN CÓ / KHÔNG (GET CONFIRM Y/N)
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
    // NHẬP GIÁ TRỊ MỚI HOẶC GIỮ NGUYÊN GIÁ TRỊ CŨ (GET DEFAULT OR NEW)
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

    static long long getLongLongOrDefault(const string& CauThongBao, long long oldValue) {
        while (true) {
            cout << CauThongBao << "(Hien tai: " << oldValue << "): ";
            string newstring;
            getline(cin, newstring);
            if (newstring.empty()) return oldValue;

            stringstream ss(newstring);
            long long newvalue;
            char extra;
            if (ss >> newvalue && !(ss >> extra)) {
                if (newvalue >= 0) {
                    return newvalue;
                } else {
                    cout << "[Loi] Gia tri khong duoc am! Vui long nhap lai.\n";
                }
            } else {
                cout << "[Loi] Vui long nhap so nguyen hop le hoac nhan Enter de giu nguyen!\n";
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
                if (newvalue >= 0) {
                    return newvalue;
                } else {
                    cout << "[Loi] Gia tri khong duoc am! Vui long nhap lai.\n";
                }
            } else {
                cout << "[Loi] Vui long nhap so nguyen hop le hoac nhan Enter de giu nguyen!\n";
            }
        }
    }

    //kiem tra xem sdt co hop le khong: 10 so va so 0 o dau;
    static bool checkPhoneNumber(const string& PhoneNumber){
        if(PhoneNumber[0] != '0') return false;
        if(PhoneNumber.size() != 10) return false;
        else{
            for(int i = 0; i < 10; i++){
                if(!isdigit(PhoneNumber[i])) return false;
            }
        }
        return true;
    }

    //doi phan dau +84 thanh 0;
    static void xuly_84(string& phone){
        if(phone[0] == '+'){
            phone = "0" + phone.substr(3);
        }
    }

    //kiem tra ma id xem co dung dinh dang khong (vi du HDxxxx, kiem tra xem cac chu dau co phai check khong);
    static bool checkidvalid(const string& id, const string& check){
        if (id.length() < check.length() || check.empty()) return false;
        for (size_t i = 0; i < check.length(); i++) {
            if (id[i] != check[i]) return false;
        }
        return true;
    }
};
