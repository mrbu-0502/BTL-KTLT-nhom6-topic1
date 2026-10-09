#pragma once
#include <iostream>
#include <string>
#include "Entity.h" // Nhúng class Entity th?t c?a nhóm vào dây

using namespace std;

// =========================================================
// CLASS DEVICE (K? th?a Entity)
// =========================================================
class Device : public Entity {
private:
    string a; // a: Mã IMEI
    string b; // b: Tên thi?t b?

public:
    // 1. Kh?i t?o
    Device() { 
        a = ""; 
        b = ""; 
    }
    
    Device(string x, string y) { 
        a = x; 
        b = y; 
    }

    // 2. Getter / Setter truy xu?t an toàn
    string getA() const { return a; }
    void setA(string x) { a = x; }
    
    string getB() const { return b; }
    void setB(string y) { b = y; }

    // 3. Ghi dè (override) các hàm t? class Entity theo chu?n OOP c?a nhóm
    void setId(const string& x) override {
        ID = x;
        a = x; // Dùng luôn ID làm mã IMEI
    }

    void getDetail() const override {
        cout << "IMEI: " << a << " | Ten thiet bi: " << b << endl;
    }

    string toString() const override {
        // Tr? v? chu?i phân cách b?i d?u | d? chu?n b? ghi ra file text devices.txt
        return a + "|" + b; 
    }

    // 4. Hàm nh?p th? công co b?n
    void nhap() {
        cout << "Nhap ma IMEI: ";
        getline(cin, a);
        cout << "Nhap ten thiet bi: ";
        getline(cin, b);
    }
};
