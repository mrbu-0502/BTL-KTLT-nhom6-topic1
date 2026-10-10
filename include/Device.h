#pragma once 
#include <iostream>
#include <string>
#include "Entity.h" // Nhap ban ve cua class cha vao day de ke thua

using namespace std;

// "class Device : public Entity" nghia la: 
// Tao ra mot ban ve Thiet bi, va ke thua toan bo noi quy tu ban ve Entity cua Khanh
class Device : public Entity {

// =========================================================================
// PHAN PRIVATE (VUNG KHEP KIN - BAO MAT DU LIEU)
// Nhung bien o trong vung nay thi cac file khac khong the tu y sua doi duoc
// =========================================================================
private:
    // 5 bien nay tuong ung voi 5 cot du lieu trong file devices.txt moi nhat
    string a; // Dung de luu Ma IMEI (VD: 861234567890123)
    string b; // Dung de luu Ten may (VD: iPhone 13)
    string c; // Dung de luu Hang san xuat (VD: Apple)
    string d; // Dung de luu Nam san xuat (VD: 2021)
    string e; // Dung de luu Tinh trang may (VD: Moi)

// =========================================================================
// PHAN PUBLIC (VUNG CONG KHAI - CHO PHEP BEN NGOAI SU DUNG)
// Chua cac chuc nang (ham) ma nguoi dung hoac cac class khac co the goi toi
// =========================================================================
public:
    // 1. Ham khoi tao (Constructor)
    // Chuc nang: Tao ra mot cai dien thoai trong rong ban dau
    Device();
    
    // ---------------------------------------------------------------------
    // 5 HAM DUOI DAY BAT BUOC PHAI CO CHU "override" (GHI DE) O CUOI
    // Y nghia: "Toi xin phep ghi de len 5 cai ham rong tuech cua class Entity 
    //           de tu viet code xu ly rieng cho Thiet bi cua toi"
    // ---------------------------------------------------------------------
    
    // 2. Ham dong bo ID he thong (Khanh yeu cau)
    void setID(const string& value) override;

    // 3. Ham nhap 5 thong tin tu ban phim an toan
    void getInformation() override;

    // 4. Ham in ra 5 thong tin len man hinh console
    void display() const override;

    // 5. Ham noi 5 bien a, b, c, d, e thanh 1 chuoi (ngan cach boi dau |) de luu file
    string toString() const override;

    // 6. Ham cat dong chu trong file txt ra thanh 5 khuc roi nhet lai vao 5 bien
    void fromString(const string& line) override;
};
