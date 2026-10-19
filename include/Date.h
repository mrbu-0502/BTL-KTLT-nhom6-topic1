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
    // 2. KIEM TRA NAM NHUAN
    // Nam nhuan la nam chia het cho 400, HOAC (chia het cho 4 nhung khong chia het cho 100)
    // ========================================================================
    bool leap() {
        return Nam % 400 == 0 || (Nam % 4 == 0 && Nam % 100 != 0);
    }

    // ========================================================================
    // 3. KIEM TRA NGAY THANG NAM CO HOP LE HAY KHONG
    // ========================================================================
    bool valid() {
        // Neu nam ngoai khoang (1 -> 2026), thang ngoai (1 -> 12), hoac ngay < 1 thi bao SAI ngay
        if (Nam < 1 || Nam > 2026 || Thang < 1 || Thang > 12 || Ngay < 1) {
            return false;
        }
        
        // Bang tra cuu so ngay toi da cua 12 thang (Bo qua vi tri so 0 cho de dem tu thang 1 -> 12)
        int soNgay[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Neu la nam nhuan thi thang 2 (vi tri so 2) se co 29 ngay
        if (leap()) {
            soNgay[2] = 29;
        }
        
        // Kiem tra xem ngay nhap vao co vuot qua so ngay toi da cua thang do khong
        return Ngay <= soNgay[Thang];
    }

  // ========================================================================
    // 4A. HAM NHAP TU CHUOI STRING CO SAN (KHONG CAN GO BAN PHIM)
    // Dong doi truyen chuoi "10/01/2024" vao -> Ham tach ra Ngay, Thang, Nam
    // Tra ve 1 neu chuoi dung dinh dang DD/MM/YYYY, tra ve 0 neu chuoi sai
    // ========================================================================
    int fromString(string chuoi) {
        char c1 = 0, c2 = 0;
        stringstream ss(chuoi);
        
        // Tach chuoi truyen vao thanh Ngay, Thang, Nam
        ss >> Ngay >> c1 >> Thang >> c2 >> Nam;
        
        // Kiem tra co dung 2 dau '/' va ngay thang hop le khong
        if (c1 == '/' && c2 == '/' && valid()) {
            return 1; // Thanh cong
        }
        return 0; // Chuoi bi sai dinh dang
    }

    // ========================================================================
    // 4B. NHAP NGAY THANG AN TOAN TU BAN PHIM (BAT NHAP LAI NEU SAI)
    // ========================================================================
    void input(string thongBao) {
        while (1) {
            cout << thongBao;
            string chuoi;
            getline(cin, chuoi); 
            
            // Goi lai ham fromString(chuoi) o tren de kiem tra luon cho ngan gon!
            if (fromString(chuoi) == 1) {
                break; 
            }
            cout << "sai dinh dang, hay nhap lai\n";
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
};
